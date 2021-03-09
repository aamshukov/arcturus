//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/bitset.hpp>

#include <core/list.hpp>
#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/vertex.hpp>
#include <core/dominator_vertex.hpp>
#include <core/edge.hpp>
#include <core/graph.hpp>

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

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <symtable/symbol.hpp>
#include <symtable/scope/scope.hpp>
#include <symtable/symbol_table.hpp>

#include <frontend/parser/parse_tree_base.hpp>
#include <frontend/parser/parse_tree.hpp>
#include <frontend/parser/parse_dag.hpp>
#include <frontend/parser/parse_context.hpp>
#include <frontend/parser/parser.hpp>
#include <frontend/parser/recursive_descent_parser.hpp>

#include <frontend/semantics/semantics.hpp>

#include <ir/quadruple.hpp>
#include <ir/code.hpp>
#include <ir/basic_block.hpp>
#include <ir/control_flow_graph.hpp>
#include <ir/data_flow_analysis.hpp>
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
#include <arcturus_ir.hpp>
#include <arcturus_pass.hpp>
#include <arcturus_data_flow_analysis.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

arcturus_data_flow_analysis::arcturus_data_flow_analysis()
{
}

arcturus_data_flow_analysis::~arcturus_data_flow_analysis()
{
}

void arcturus_data_flow_analysis::collect_liveness_def_use_sets(typename arcturus_data_flow_analysis::control_flow_graph_type& cfg)
{
    // 'Engineering a Compiler' by Keith D. Cooper, Linda Torczon
    //  ... assume block b has k operations of form 'x <-- y op z'
    for(auto& block : (*cfg).vertices())
    {
        auto& defs((*block).defs());
        auto& uses((*block).uses());

        defs.clear();
        uses.clear();

        auto& code((*block).code());

        for(auto instruction = code.instructions();
            instruction != code.end_instruction();
            instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next()))
        {
            if(arcturus_quadruple::is_x_y_op_z((*instruction).operation))
            {
                auto& arg1((*instruction).argument1.first);
                auto& arg2((*instruction).argument2.first);

                auto& result(std::get<arcturus_quadruple::argument_type>((*instruction).result).first);

                if(arg1 != nullptr) // && defs.find(arg1) == defs.end())
                    uses.emplace(arg1);

                if(arg2 != nullptr) // && defs.find(arg2) == defs.end())
                    uses.emplace(arg2);

                if(result != nullptr)
                    defs.emplace(result);
            }
        }
    }
}

void arcturus_data_flow_analysis::calculate_liveness_in_outs_sets(typename arcturus_data_flow_analysis::control_flow_graph_type& cfg)
{
    // 'Compilers : principles, techniques, and tools' by Alfred V. Aho, Ravi Sethi, Jeffrey D. Ullman. 2nd ed.
    for(auto& block : (*cfg).vertices())
    {
        (*block).ins().clear();
        (*block).outs().clear();
    }

    for(;;)
    {
        for(auto& block : (*cfg).vertices())
        {
            if((*block).label() == L"exit")
                continue;

            std::set<basic_block_type, vertex_lt_key_comparator<basic_block<arcturus_quadruple>>> successors;

            (*cfg).collect_successors(block, successors);

            for(auto& successor : successors)
            {
                // OUT[B] = U IN[S]
                //           S a successor of B
                symbols_type outs((*block).outs());

                std::set_union((*successor).ins().begin(),
                               (*successor).ins().end(),
                               outs.begin(),
                               outs.end(),
                               std::inserter((*block).outs(), (*block).outs().begin()));

                // IN[B] = use[B] U (OUT[B] - def[B])
                symbols_type diffs;

                std::set_difference((*block).outs().begin(), // ... (OUT[B] - def[B])
                                    (*block).outs().end(),
                                    (*block).defs().begin(),
                                    (*block).defs().end(),
                                    std::inserter(diffs, diffs.begin()));

                auto ins_count = (*block).ins().size();

                std::set_union((*block).uses().begin(), // IN[B] = use[B] U ...
                               (*block).uses().end(),
                               diffs.begin(),
                               diffs.end(),
                               std::inserter((*block).ins(), (*block).ins().begin()));

                if((*block).ins().size() == ins_count)
                {
                    goto _exit;
                }
            }
        }
    }

_exit:
    ;
}

END_NAMESPACE
