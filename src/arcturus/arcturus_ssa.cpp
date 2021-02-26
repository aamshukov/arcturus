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
    if((*cfg).vertices().size() > 128)
        graph_algorithms<basic_block<arcturus_quadruple>>::compute_dominators_lengauer_tarjan(g);
    else
        graph_algorithms<basic_block<arcturus_quadruple>>::compute_dominators(g);

    // phase II (build dominance tree)
    dominance_tree_type dt;

    graph_algorithms<basic_block<arcturus_quadruple>>::build_dominance_tree(g, dt);

    // phase III (compute dominance frontiers)
    graph_algorithms<basic_block<arcturus_quadruple>>::compute_dominance_frontiers(g, dt);

    // phase IV (placement of 𝛗-functions and renaming)
    for(auto& assignment : (*cfg).assignments()) // assignements = defs, assignment = def
    {
        // phase IV (place 𝛗 functions)
        place_phi_functions(assignment.first, cfg);

        // phase V (rename)
        rename_variables(assignment.first, dt, cfg);
    }
}

void arcturus_ssa::place_phi_functions(const typename arcturus_ssa::symbol_type& v, typename arcturus_ssa::control_flow_graph_type& cfg)
{
    // 'Crafting a Compiler' by Charles N. Fischer, Ron K. Cytron, Richard J LeBlanc
    // 'Modern Compiler Implementation in Java' 2nd edition, by Andrew W. Appel
    // 'An Efficient Method of Computing Static Single Assignment Form' by Ron Cytron, etc.
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
                auto phi(make_phi_instruction(v, predecessors));

                (*dynamic_cast<basic_block<arcturus_quadruple>*>(y.get())).code().insert_instruction(phi);

                if(((*y).flags() & vertex::flag::processed) != vertex::flag::processed)
                {
                    worklist.emplace(std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>(y));
                }
            }
        }
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

void arcturus_ssa::rename_variables(const typename arcturus_ssa::symbol_type& v,
                                    const typename arcturus_ssa::dominance_tree_type& dt,
                                    const typename arcturus_ssa::control_flow_graph_type& cfg)
{
    // 'Crafting a Compiler' by Charles N. Fischer, Ron K. Cytron, Richard J LeBlanc
    // 'Modern Compiler Implementation in Java' 2nd edition, by Andrew W. Appel
    // 'An Efficient Method of Computing Static Single Assignment Form' by Ron Cytron, etc.
    entry_type e;

    e.version = 0;
    e.stack.push(0);

    rename_variables(v, dt, cfg, e);
}

void arcturus_ssa::rename_variables(const typename arcturus_ssa::symbol_type& v,
                                    const typename arcturus_ssa::dominance_tree_type& x,
                                    const typename arcturus_ssa::control_flow_graph_type& cfg,
                                    typename arcturus_ssa::entry_type& e)
{
    auto contains_def = 0;

    auto& code((*std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>((*x).vertex())).code());

    for(auto instruction = code.instructions();
        instruction != code.end_instruction();
        instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next()))
    {
        // consider only assignements which are 'v = x op y' or 𝛗-function
        if(arcturus_quadruple::is_assignment((*instruction).operation) ||
          (*instruction).operation == arcturus_operation_code_traits::operation_code::phi)
        {
            // RHS - only 'ordinary' (non 𝛗-functions) assignments' RHS
            if(arcturus_quadruple::is_assignment((*instruction).operation))
            {
                for(auto k = 0; k < 2; k++) // 2 - as of form ... x op y
                {
                    auto& rhs_arg(k == 0 ? (*instruction).argument1 : (*instruction).argument2);
                    auto& rhs_var(rhs_arg.first);

                    if(rhs_var == v)
                    {
                        rhs_arg.second = e.stack.top(); // bump up version
                    }
                }
            }

            // LHS - all assignments including 𝛗-functions
            auto& lhs_arg((*instruction).operation == arcturus_operation_code_traits::operation_code::phi ?
                          (*instruction).argument1 : std::get<arcturus_quadruple::argument_type>((*instruction).result));
            auto& lhs_var(lhs_arg.first);

            if(lhs_var == v)
            {
                lhs_arg.second = e.version; // bump up version

                e.stack.push(e.version);

                e.version++;

                contains_def++; // pop at the end
            }
        }
    }

    // process successors in CFG
    std::set<basic_block_type, vertex_lt_key_comparator<basic_block<arcturus_quadruple>>> successors;

    (*cfg).collect_successors(std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>((*x).vertex()), successors);

    for(const auto& successor : successors)
    {
        std::size_t k = 0;
        std::size_t j = 0; // j = whichPred ... j-th parameter of 𝛗-function must correspond to j-th predecessor in control flow graph

        std::set<basic_block_type, vertex_lt_key_comparator<basic_block<arcturus_quadruple>>> predecessors;

        (*cfg).collect_predecessors(std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>(successor), predecessors);

        for(const auto& predecessor: predecessors)
        {
            if(predecessor == (*x).vertex())
            {
                j = k;
                break;
            }

            k++;
        }

        auto& successor_code((*std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>(successor)).code());

        for(auto instruction = successor_code.instructions();
            instruction != successor_code.end_instruction();
            instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next()))
        {
            if((*instruction).operation == arcturus_operation_code_traits::operation_code::phi)
            {
                auto& lhs_arg((*instruction).argument1); // arg1 must be valid because it is assignment
                auto& lhs_var(lhs_arg.first);

                if(lhs_var == v)
                {
                    auto& phi_params(std::get<arcturus_quadruple::phi_params_type>((*instruction).result));

                    phi_params[j].first.second = e.stack.top(); // bump up version

                    break;
                }
            }
        }
    }

    // recursively walk dominance tree
    for(auto y : (*x).kids())
    {
        rename_variables(v, std::dynamic_pointer_cast<dominance_tree>(y), cfg, e);
    }

    // clenup
    if(contains_def > 0)
    {
        e.stack.pop();
    }
}

typename arcturus_ssa::arcturus_instruction_type arcturus_ssa::make_phi_instruction(const typename arcturus_ssa::symbol_type& v,
                                                                                    const typename arcturus_ssa::basic_blocks_type& predecessors)
{
    arcturus_quadruple::phi_params_type params;

    for(auto& predecessor : predecessors)
    {
        params.emplace_back(std::make_pair(std::make_pair(v, 0), predecessor));
    }

    arcturus_instruction_type result(factory::create<arcturus_quadruple>(0,
                                                                         arcturus_operation_code_traits::operation_code::phi,
                                                                         std::make_pair(v, 0),
                                                                         params));
    return result;
}

void arcturus_ssa::destruct_ssa_form(typename arcturus_ssa::control_flow_graph_type& cfg)
{
    // 'Translating Out of Static Single Assignment Form' by Vugranam C. Sreedhar, Roy Dz-Ching Ju2, David M. Gillies3, and Vatsa Santhanam
    // ... method I
    //
    // two issues are not covered by this implementation - as of 'Towards an SSA based compiler back-end' by Benoit Boissinot:
    //  1) Depending on the branching instruction, the copies cannot always be inserted at the very end of the block, i.e., after all variables uses and defnitions.
    //     For example, for a 𝛗-function after a conditional branch that uses a variable 'u', the copies must be inserted before the use of 'u'.
    //  2) When the basic block contains variables defned after the point of copy insertion. This confguration is possible
    //     in embedded environments, where some DSP-like branching instructions have a behavior similar to a hardware loop:
    //     in addition to the condition, a counter 'u' is decremented by the instruction itself ...
    using op = arcturus_operation_code_traits::operation_code;

    for(const auto& vertex : (*cfg).vertices())  // vertex is basic block
    {
        auto& code((*vertex).code());

        for(auto instruction = code.instructions();
            instruction != code.end_instruction();
            instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next()))
        {
            if((*instruction).operation != arcturus_operation_code_traits::operation_code::phi)
                continue;

            auto& phi_params(std::get<arcturus_quadruple::phi_params_type>((*instruction).result));

            // phase I (applying method I - translate SSA into CSSA (Conventional SSA))
            for(const auto& phi_param : phi_params)
            {
                auto predecessor(std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>(phi_param.second));

                // x1' = x1
                const auto& x1(phi_param.first.first);
                auto x1_ver(phi_param.first.second);

                auto x1p(factory::create<arcturus_symbol>()); // x1', todo: should be cloned from x1
                (*x1p).name() = (*phi_param.first.first).name() + cp_type{'\''};

                auto x1_x1p(factory::create<arcturus_quadruple>(0,
                                                                op::assignment_hir,
                                                                std::make_pair(x1, x1_ver),
                                                                std::make_pair(x1p, x1_ver)));
                (*predecessor).code().add_instruction(x1_x1p);
            }

            // x3' = x3
            auto& x3((*instruction).argument1.first); // arg1 must be valid because it is assignment
            auto x3_ver((*instruction).argument1.second);

            auto x3p(factory::create<arcturus_symbol>()); // x3', todo: should be cloned from x3
            (*x3p).name() = (*x3).name() + cp_type{'\''};

            auto x3_x3p(factory::create<arcturus_quadruple>(0,
                                                            op::assignment_hir,
                                                            std::make_pair(x3, x3_ver),
                                                            std::make_pair(x3p, x3_ver)));
            (*vertex).code().insert_instruction(x3_x3p, std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next()));

            // add ' to all members of the 𝛗-instruction
            (*x3).name() += cp_type{'\''};

            for(auto& phi_param : phi_params)
            {
                (*phi_param.first.first).name() += cp_type{'\''};
            }

            // phase II (eliminate 𝛗-functions)
            for(const auto& phi_param : phi_params)
            {
                // add to each predecessor x3' = x1'
                auto& x1p_phi(phi_param.first.first);
                auto& x1p_phi_ver(phi_param.first.second);

                auto& x3p_phi((*instruction).argument1.first); // arg1 already has been processed and has x3'
                auto& x3p_phi_ver((*instruction).argument1.second);

                auto& pred_phi(phi_param.second); // predeccessor

                auto x3p_phi_x1p(factory::create<arcturus_quadruple>(0,
                                                                     op::assignment_hir,
                                                                     std::make_pair(x1p_phi, x1p_phi_ver),
                                                                     std::make_pair(x3p_phi, x3p_phi_ver)));

                (*std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>(pred_phi)).code().add_instruction(x3p_phi_x1p);
            }

            // remove 𝛗-function
            (*vertex).code().remove_instruction(instruction);
        }
    }
}

END_NAMESPACE
