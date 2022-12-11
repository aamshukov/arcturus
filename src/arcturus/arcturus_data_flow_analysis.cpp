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

#include <frontend/grammar/grammar_symbol.hpp>
#include <frontend/grammar/grammar_rule.hpp>
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
#include <frontend/type/abstract_type.hpp>

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
#include <ir/interference_vertex.hpp>
#include <ir/interference_graph.hpp>
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
#include <arcturus_type_traits.hpp>
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

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)
USING_NAMESPACE(backend)

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
    // 'Compilers : principles, techniques, and tools' by Alfred V. Aho, Ravi Sethi, Jeffrey D. Ullman. 2nd ed. pp.607
    for(auto& block : (*cfg).vertices())
    {
        (*block).ins().clear();
        (*block).outs().clear();
    }

    std::size_t ins_count = 0;
    std::size_t ins_curr_count = 0;

    for(;;)
    {
        ins_curr_count = 0;

        for(auto& block : (*cfg).vertices())
        {
            if((*block).label() == L"exit")
                continue;

            // ... clumsy C++ sets' routines
            std::set<basic_block_type, vertex_lt_key_comparator<basic_block<arcturus_quadruple>>> successors;

            (*cfg).collect_successors(block, successors);

            for(auto& successor : successors)
            {
                // OUT[B] = U IN[S]
                //           S a successor of B
                (*block).outs().insert((*successor).ins().begin(), (*successor).ins().end());
            }

            // IN[B] = use[B] U (OUT[B] - def[B])
            std::vector<symbol_type> outs;

            std::for_each((*block).outs().begin(), (*block).outs().end(), [&outs](const auto& out) { outs.push_back(out); });
            std::sort(outs.begin(), outs.end(), symtable::symbol::symbol_key_comparator());

            std::vector<symbol_type> defs;

            std::for_each((*block).defs().begin(), (*block).defs().end(), [&defs](const auto& def) { defs.push_back(def); });
            std::sort(defs.begin(), defs.end(), symtable::symbol::symbol_key_comparator());

            symbols_type diffs;

            std::set_difference(outs.begin(), // ... (OUT[B] - def[B])
                                outs.end(),
                                defs.begin(),
                                defs.end(),
                                std::inserter(diffs, diffs.begin()),
                                symtable::symbol::symbol_key_comparator());

            std::vector<symbol_type> uses;

            std::for_each((*block).uses().begin(), (*block).uses().end(), [&uses](const auto& use) { uses.push_back(use); });
            std::sort(uses.begin(), uses.end(), symtable::symbol::symbol_key_comparator());
            std::set_union(uses.begin(), // IN[B] = use[B] U diffs ...
                           uses.end(),
                           diffs.begin(), // already sorted
                           diffs.end(),
                           std::inserter((*block).ins(), (*block).ins().begin()),
                           symtable::symbol::symbol_key_comparator());

            ins_curr_count += (*block).ins().size();
        }

        if(ins_curr_count == ins_count)
        {
            break;
        }

        ins_count = ins_curr_count;
    }
}

typename arcturus_data_flow_analysis::interference_graph_type arcturus_data_flow_analysis::build_interference_graph(const typename arcturus_data_flow_analysis::control_flow_graph_type& cfg)
{
    // 'Engineering a Compiler' by Keith D. Cooper, Linda Torczon, p.p. 701
    interference_graph_type result {factory::create<interference_graph>() };

    // for each basic block
    for(auto& block : (*cfg).vertices())
    {
        symbols_type live_now;

        std::for_each((*block).outs().begin(), (*block).outs().end(), [&live_now](const auto& out) { live_now.emplace(out); });

        auto& code((*block).code());

        for(auto instruction = (*code.end_instruction()).prev(); // in reverse order - OPn, OPn-1, OP n-1 ... OP1 in basic block
            instruction != code.start_instruction();
            instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).prev()))
        {
            //?? treatment of MOVE instructions ...
            if((*instruction).operation == arcturus_operation_code_traits::operation_code::binary_op_add_mir ||
               (*instruction).operation == arcturus_operation_code_traits::operation_code::binary_op_subtract_mir ||
               (*instruction).operation == arcturus_operation_code_traits::operation_code::binary_op_multiply_mir ||
               (*instruction).operation == arcturus_operation_code_traits::operation_code::binary_op_divide_mir)
            {
                auto& lr_a_sym { (*instruction).argument1.first };
                auto& lr_b_sym { (*instruction).argument2.first };
                auto& lr_c_sym { std::get<arcturus_quadruple::argument_type>((*instruction).result).first }; // implicit def()

                auto lr_c { factory::create<interference_vertex>(lr_c_sym) };

                (*result).add_vertex(lr_c);

                std::for_each(live_now.begin(),
                              live_now.end(),
                              [&lr_a_sym, &lr_b_sym, &lr_c_sym, &lr_c, &result](const auto& live_now_sym)
                              {
                                  if(live_now_sym != lr_a_sym && live_now_sym != lr_b_sym && live_now_sym != lr_c_sym) // except LRa and LRb and LRc
                                  {
                                      auto lr_j { factory::create<interference_vertex>(live_now_sym) };

                                      (*result).add_vertex(lr_j);
                                      (*result).add_edge(lr_c, lr_j, 0.0);
                                  }
                              });

                live_now.erase(lr_c_sym);

                live_now.emplace(lr_a_sym);
                live_now.emplace(lr_b_sym);
            }
        }
    }

    return result;
}

void arcturus_data_flow_analysis::generate_interference_graph_graphviz_file(const typename arcturus_data_flow_analysis::interference_graph_type& interference_graph,
                                                                            const string_type& file_name)
{
    log_info(L"Generating interference graph graphviz file ...");

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate interference graph graphviz file: stream is either open or in a bad condition.");
        }

        const char_type *indent(get_indent(4));

        stream << L"digraph CFG" << std::endl;
        stream << L"{" << std::endl;

        for(const auto& vertex : (*interference_graph).vertices())
        {
            string_type label((*vertex).label() + L"<br/>");
            stream << indent << (*vertex).id() << L" [shape=box label=<" << label << L">];" << std::endl;
        }

        for(const auto& edge : (*interference_graph).edges())
        {
            const auto& vertex_u((*edge).endpoints()[0]);
            const auto& vertex_v((*edge).endpoints()[1]);

            // generate label for U
            string_type u_label(std::to_wstring((*vertex_u).id()));

            // generate label for V
            string_type v_label(std::to_wstring((*vertex_v).id()));

            stream << indent << u_label << L" -> " << v_label  << L";" << std::endl;
        }

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate interference graph graphviz file: error occurred.");
    }

    log_info(L"Generated interference graph graphviz file.");
}

END_NAMESPACE
