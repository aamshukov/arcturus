//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/bitset.hpp>

#include <core/list.hpp>
#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/vertex.hpp>
#include <core/dominator_vertex.hpp>
#include <core/dominance_tree.hpp>
#include <core/edge.hpp>
#include <core/graph.hpp>
#include <core/graph_algorithms.hpp>

#include <core/timer.hpp>

#include <content/data_provider.hpp>
#include <content/file_data_provider.hpp>

#include <frontend/grammar/symbol.hpp>
#include <frontend/grammar/rule.hpp>
#include <frontend/grammar/grammar.hpp>
#include <frontend/grammar/grammar_algorithm.hpp>
#include <frontend/grammar/grammar_visualization.hpp>

#include <frontend/fsa/fsa_transition.hpp>
#include <frontend/fsa/fsa_state.hpp>
#include <frontend/fsa/fsa_state_set.hpp>
#include <frontend/fsa/fsa.hpp>
#include <frontend/fsa/fsa_algorithm.hpp>
#include <frontend/fsa/fsa_re.hpp>
#include <frontend/fsa/fsa_visualization.hpp>
#include <frontend/fsa/fsa_codegen.hpp>

#include <content/content.hpp>

#include <frontend/lexical_analyzer/token.hpp>

#include <frontend/lexical_analyzer/lexical_content.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.inl>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <symtable/symbol.hpp>
#include <symtable/scope/scope.hpp>
#include <symtable/symbol_table.hpp>
#include <symtable/symbol_table.inl>

#include <frontend/parser/parse_tree_base.hpp>
#include <frontend/parser/parse_tree.hpp>
#include <frontend/parser/parse_dag.hpp>
#include <frontend/parser/parse_context.hpp>
#include <frontend/parser/parser.hpp>
#include <frontend/parser/parser.inl>
#include <frontend/parser/recursive_descent_parser.hpp>
#include <frontend/parser/recursive_descent_parser.inl>

#include <frontend/semantics/semantics.hpp>
#include <frontend/semantics/semantics.inl>

#include <ir/quadruple.hpp>
#include <ir/code.hpp>
#include <ir/basic_block.hpp>
#include <ir/basic_block.inl>
#include <ir/control_flow_graph.hpp>
#include <ir/ssa.hpp>
#include <ir/ssa.inl>
#include <ir/ir_visitor.hpp>
#include <ir/ir.hpp>
#include <ir/ir.inl>

#include <backend/optimization/pass.hpp>

#include <backend/codegen/activation_record.hpp>
#include <backend/codegen/amd64/activation_record_amd64.hpp>
#include <backend/codegen/codegen.hpp>

#include <backend/optimization/pass.hpp>

#include <arcturus_configurator.hpp>
#include <arcturus_token.hpp>
#include <arcturus_type.hpp>
#include <arcturus_scalar_type.hpp>
#include <arcturus_array_type.hpp>
#include <arcturus_struct_type.hpp>
#include <arcturus_enum_type.hpp>
#include <arcturus_func_type.hpp>
#include <arcturus_symbol.hpp>
#include <arcturus_lexical_analyzer.hpp>
#include <arcturus_parse_tree.hpp>
#include <arcturus_parse_context.hpp>
#include <arcturus_parser.hpp>
#include <arcturus_quadruple.hpp>
#include <arcturus_control_flow_graph.hpp>
#include <arcturus_ssa.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

// 𝛗

void arcturus_ssa::build_ssa_form(typename arcturus_ssa::control_flow_graph_type& cfg)
{
    auto g(std::dynamic_pointer_cast<graph<basic_block<arcturus_quadruple>>>(cfg));

    // phase I (compute dominators)
    graph_algorithms<basic_block<arcturus_quadruple>>::compute_dominators(g);

    // phase II (dominance tree)
    std::shared_ptr<dominance_tree> dt;

    graph_algorithms<basic_block<arcturus_quadruple>>::build_dominance_tree(g, dt);

    // phase III (dominance frontiers)
    graph_algorithms<basic_block<arcturus_quadruple>>::compute_dominance_frontiers(g, dt);

    for(auto& assignment : (*cfg).assignments()) // assignements = defs, assignment = def
    {
        // phase IV (place 𝛗 functions)
        place_phi_functions(assignment.first, cfg);

        // phase V (rename)
        rename_variables(assignment.first, cfg);
    }

    // clean up 'exit' block
    std::set<basic_block_type, vertex_lt_key_comparator<basic_block<arcturus_quadruple>>> successors;

    (*cfg).collect_successors((*cfg).root(), successors);

    for(auto& successor : successors)
    {
        if((*successor).label() == L"exit")
        {
            (*successor).code().clear();
            break;
        }
    }
}

void arcturus_ssa::place_phi_functions(const typename arcturus_ssa::symbol_type& v, typename arcturus_ssa::control_flow_graph_type& cfg)
{
    // 'Crafting a Compiler' by Charles N. Fischer, Ron K. Cytron, Richard J LeBlanc
    for(auto& vertex : (*cfg).vertices())
    {
        (*vertex).flags() = vertex::flag::clear;
    }

    std::queue<basic_block_type> worklist;

    for(auto& assignment : (*cfg).assignments()[v]) // assignements = defs, assignment = def
    {
        (*assignment).flags() |= vertex::flag::processed;

        worklist.emplace(assignment);
    }

    while(!worklist.empty())
    {
        auto x(worklist.front());
        worklist.pop();

        for(auto& y : (*x).frontiers())
        {
            if(((*y).flags() & vertex::flag::hasphi) != vertex::flag::hasphi)
            {
                (*y).flags() |= vertex::flag::hasphi;

                std::set<basic_block_type, vertex_lt_key_comparator<basic_block<arcturus_quadruple>>> predecessors;

                (*cfg).collect_predecessors(std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>(y), predecessors);

                // add 𝛗 to Y
                auto phi(make_phi_instruction(v, predecessors.size()));

                (*dynamic_cast<basic_block<arcturus_quadruple>*>(y.get())).code().insert_instruction(phi);

                if(((*y).flags() & vertex::flag::processed) != vertex::flag::processed)
                {
                    worklist.emplace(std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>(y));
                }
            }
        }
    }
}

void arcturus_ssa::rename_variables(const typename arcturus_ssa::symbol_type& v, typename arcturus_ssa::control_flow_graph_type& cfg)
{
    // 'Crafting a Compiler' by Charles N. Fischer, Ron K. Cytron, Richard J LeBlanc

    v;
    cfg; //??
}

typename arcturus_ssa::arcturus_instruction_type arcturus_ssa::make_phi_instruction(const typename arcturus_ssa::symbol_type& v, id_type n)
{
    std::vector<std::pair<symbol_type, id_type>> params;

    for(auto k = 0; k < n; k++)
    {
        params.emplace_back(std::make_pair(v, 0));
    }

    arcturus_instruction_type result(factory::create<arcturus_quadruple>(0,
                                                                         arcturus_operation_code_traits::operation_code::phi,
                                                                         std::make_pair(v, 0),
                                                                         params));
    return result;
}

END_NAMESPACE
