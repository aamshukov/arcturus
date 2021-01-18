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
#include <ir/code.inl>
#include <ir/basic_block.hpp>
#include <ir/basic_block.inl>
#include <ir/control_flow_graph.hpp>
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
#include <arcturus_control_flow_graph.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

void arcturus_control_flow_graph::build_hir(typename arcturus_control_flow_graph::code_type& code0)
{
    auto& code(*code0);

    auto instruction = code.instructions();

    if(instruction != code.end_instruction())
    {
        // phase I (collect leaders)
        //  Первая команда (instruction) промежуточного кода является лидером.
        //  Любая команда (instruction), являющаяся целевой для условного или безусловного перехода, является лидером.
        //  Любая команда (instruction), следующая непосредственно за условным или безусловным переходом or 'return', является лидером.
        (*instruction).flags |= arcturus_quadruple::flags_type::leader;

        instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());

        for(; instruction != code.end_instruction();)
        {
            if((*instruction).operation == arcturus_operation_code_traits::operation_code::if_true_hir ||
               (*instruction).operation == arcturus_operation_code_traits::operation_code::if_false_hir ||
               (*instruction).operation == arcturus_operation_code_traits::operation_code::goto_hir)
            {
                // Любая команда (instruction), являющаяся целевой для условного или безусловного перехода, является лидером.
                (*std::get<1>((*instruction).result)).flags |= arcturus_quadruple::flags_type::leader;

                // Любая команда (instruction), следующая непосредственно за условным или безусловным переходом ... , является лидером.
                instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());

                if(instruction != code.end_instruction())
                {
                    (*instruction).flags |= arcturus_quadruple::flags_type::leader;
                }
            }
            else if((*instruction).operation == arcturus_operation_code_traits::operation_code::function_return_hir)
            {
                // Любая команда (instruction), следующая непосредственно за ... or 'return', является лидером.
                instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());

                if(instruction != code.end_instruction())
                {
                    (*instruction).flags |= arcturus_quadruple::flags_type::leader;
                }
            }
            else
            {
                instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());
            }
        }

        // phase II (build basic blocks)
        //  Базовый блок каждого лидера определяется как содержащий самого лидера и все команды до (но не включая) следующего лидера или
        //  до конца промежуточной программы.
        id_type id = 0;

        const char_type* label(L"BB-%ld");

        instruction = code.instructions(); // restart

        basic_blocks_type blocks;

        // entry point
        auto entry_block(factory::create<basic_block<instruction_type>>(id, format(label, id))); // id = 0

        blocks.emplace_back(entry_block);

        id++;

        auto current_block(factory::create<basic_block<instruction_type>>(id, format(label, id))); // the first block because of Первая команда (instruction) ...

        blocks.emplace_back(current_block);

        std::unordered_map<id_type, basic_block_type> inst_block_map; // maps instruction to block

        do
        {
            (*current_block).code().add_instruction(instruction);

            inst_block_map.insert({ (*instruction).id, current_block });

            instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());

            if(((*instruction).flags & arcturus_quadruple::flags_type::leader) == arcturus_quadruple::flags_type::leader)
            {
                id++;

                current_block = factory::create<basic_block<instruction_type>>(id, format(label, id));

                blocks.emplace_back(current_block);
            }
        }
        while(instruction != code.end_instruction());

        // exit point
        id++;

        auto exit_block(factory::create<basic_block<instruction_type>>(id, format(label, id)));

        blocks.emplace_back(exit_block);

        // phase III (build CFG)
        add_vertex(entry_block);
        root() = entry_block;
        add_vertex(exit_block);

        // link entry point with the first block - there is an edge from Entry to each initial BB
        // arcturus/art has only one entry point (as an alternative to Fortran 77's multiple entry points)
        add_edge(entry_block, blocks[1], 0.0); // [1] - either a block or the virtual exit_block

        for(std::size_t i = 1; i < blocks.size() - 1; i++) // 1 and -1 because the first and the last blocks are virtual entry/exit points
        {
            auto block(blocks[i]);

            add_vertex(block);

            instruction = std::static_pointer_cast<arcturus_quadruple>((*(*block).code().instructions()).prev()); // get the last instruction in a block

            if(instruction != block->code().start_instruction())
            {
                basic_block_type target_block;

                // there is a branch from last instruction in B1 to the leader of B2 ...
                if((*instruction).operation == arcturus_operation_code_traits::operation_code::if_true_hir ||
                    (*instruction).operation == arcturus_operation_code_traits::operation_code::if_false_hir ||
                    (*instruction).operation == arcturus_operation_code_traits::operation_code::goto_hir)
                {
                    auto target_instruction(std::get<1>((*instruction).result));
                    target_block = inst_block_map[(*target_instruction).id];

                    // link block -> target_block
                    add_vertex(target_block);
                    add_edge(block, target_block, 0.0);
                }

                // B2 immediately follows B1, and B1 DOES NOT end in an unconditional branch ...
                if((*instruction).operation != arcturus_operation_code_traits::operation_code::goto_hir)
                {
                    target_block = blocks[i + 1];

                    // link block -> target_block
                    add_vertex(target_block);
                    add_edge(block, target_block, 0.0);
                }
            }
        }

        // link exit point with the final block(s) - there is an edge from each final BB to Exit
        //  ... final basic blocks are basic blocks either with no successors or with a 'return' statement
        for(auto& block : blocks)
        {
            if((*block).adjacencies().empty() || 
               (*instruction).operation != arcturus_operation_code_traits::operation_code::function_return_hir)
            {
                add_edge(block, exit_block, 0.0);
            }
        }
    }
}

void arcturus_control_flow_graph::build_mir(typename arcturus_control_flow_graph::code_type& /*code*/)
{
}

void arcturus_control_flow_graph::build_lir(typename arcturus_control_flow_graph::code_type& /*code*/)
{
}

void arcturus_control_flow_graph::generate_graphviz_file(const string_type& file_name)
{
    log_info(L"Generating graphviz file ...");

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate graphviz file: stream is either open or in a bad condition.");
        }

        const char_type* indent(get_indent(4));

        stream << L"digraph CFG" << std::endl;
        stream << L"{" << std::endl;

        for(const auto& vertex : vertices())
        {
            stream << indent << ((*vertex).label().empty() ? std::to_wstring((*vertex).id()) : (*vertex).label()) << L" node [shape = circle];" << std::endl;
        }

        for(const auto& edge : edges())
        {
            const auto& vertex_u((*edge).endpoints()[0]);
            const auto& vertex_v((*edge).endpoints()[1]);

            // generate label for U
            string_type u_label(((*vertex_u).label().empty() ? std::to_wstring((*vertex_u).id()) : (*vertex_u).label()));

            // generate label for V
            string_type v_label(((*vertex_v).label().empty() ? std::to_wstring((*vertex_v).id()) : (*vertex_v).label()));

            stream << indent << u_label << L" -> " << v_label  << L";" << std::endl;
        }

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate graphviz file: error occurred.");
    }

    log_info(L"Generated graphviz file.");
}

END_NAMESPACE
