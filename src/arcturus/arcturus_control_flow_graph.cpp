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

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)
USING_NAMESPACE(backend)

arcturus_control_flow_graph::arcturus_control_flow_graph()
{
}

arcturus_control_flow_graph::~arcturus_control_flow_graph()
{
}

void arcturus_control_flow_graph::collect_basic_blocks(typename arcturus_control_flow_graph::code_type& code0)
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

        for(; instruction != code.end_instruction();)
        {
            if((*instruction).operation == arcturus_operation_code_traits::operation_code::if_true_mir ||
               (*instruction).operation == arcturus_operation_code_traits::operation_code::if_false_mir ||
               (*instruction).operation == arcturus_operation_code_traits::operation_code::goto_mir)
            {
                // Любая команда (instruction), являющаяся целевой для условного или безусловного перехода, является лидером.
                (*std::get<1>((*instruction).result)).flags |= arcturus_quadruple::flags_type::leader;

                // Любая команда (instruction), следующая непосредственно за условным или безусловным переходом ... , является лидером.
                instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next());

                if(instruction != code.end_instruction())
                {
                    (*instruction).flags |= arcturus_quadruple::flags_type::leader;
                }
            }
            else if((*instruction).operation == arcturus_operation_code_traits::operation_code::function_return_mir)
            {
                // Любая команда (instruction), следующая непосредственно за ... or 'return', является лидером.
                instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next());

                if(instruction != code.end_instruction())
                {
                    (*instruction).flags |= arcturus_quadruple::flags_type::leader;
                }
            }
            else
            {
                instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next());
            }
        }

        // phase II (build basic blocks)
        //  Базовый блок каждого лидера определяется как содержащий самого лидера и все команды до (но не включая) следующего лидера или
        //  до конца промежуточной программы.
        id_type id = 0;

        const char_type *label(L"BB-%ld");

        basic_blocks_type blocks;

        std::unordered_map<id_type, basic_block_type> inst_block_map; // maps instruction to block

        // entry point
        auto entry_block(factory::create<basic_block<instruction_type>>(id, L"entry"));

        root() = entry_block;

        blocks.emplace_back(entry_block);

        id++;

        auto current_block(factory::create<basic_block<instruction_type>>(id, format(label, id))); // the first block because of Первая команда (instruction) ...

        blocks.emplace_back(current_block);

        instruction = code.instructions(); // restart

        do
        {
            auto next_instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next());

            inst_block_map.insert({ (*instruction).id, current_block });

            if(arcturus_quadruple::is_assignment((*instruction).operation))
            {
                const auto& symbol(std::get<0>((*instruction).result));

                auto it(my_assignments.find(symbol.first));

                if(it == my_assignments.end())
                {
                    my_assignments[symbol.first] = { current_block };
                }
                else
                {
                    (*it).second.emplace_back(current_block);
                }
            }

            (*current_block).code().add_instruction(instruction); // unlink from the current-block.code and link to the new block.code

            instruction = next_instruction; // restore iterator for the current-block.code

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

        auto exit_block(factory::create<basic_block<instruction_type>>(id, L"exit"));

        blocks.emplace_back(exit_block);

        // phase III (build CFG)
        for(auto& block : blocks)
        {
            add_vertex(block);
        }

        // link entry point with the first block - there is an edge from Entry to each initial BB
        // arcturus/art has only one entry point (as an alternative to Fortran 77's multiple entry points)
        add_edge(entry_block, blocks[1], 0.0); // [1] - either a block or the virtual exit_block

        for(std::size_t i = 1; i < blocks.size() - 1; i++) // 1 and -1 because the first and the last blocks are virtual entry/exit points
        {
            auto block(blocks[i]);

            instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*(*block).code().end_instruction()).prev()); // get the last instruction in a block

            if(instruction != (*block).code().start_instruction())
            {
                // there is a branch from last instruction in B1 to the leader of B2 ...
                if((*instruction).operation == arcturus_operation_code_traits::operation_code::if_true_mir ||
                    (*instruction).operation == arcturus_operation_code_traits::operation_code::if_false_mir ||
                    (*instruction).operation == arcturus_operation_code_traits::operation_code::goto_mir)
                {
                    // link block -> target_block
                    auto target_instruction(std::get<1>((*instruction).result));
                    auto& target_block(inst_block_map[(*target_instruction).id]);

                    add_edge(block, target_block, 0.0);
                }

                // B2 immediately follows B1, and B1 DOES NOT end in an unconditional branch ...
                if((*instruction).operation != arcturus_operation_code_traits::operation_code::goto_mir &&
                   (*instruction).operation != arcturus_operation_code_traits::operation_code::function_return_mir)
                {
                    // link block -> target_block
                    auto& target_block(blocks[i + 1]);
                    add_edge(block, target_block, 0.0);
                }
            }
        }

        // For technical reasons related to the representation of control dependences, there is also an edge from Entry to Exit
        add_edge(entry_block, exit_block, 0.0);

        // link exit point with the final block(s) - there is an edge from each final BB to Exit
        //  ... final basic blocks are basic blocks either with no successors or with a 'return' statement
        for(auto& block : blocks)
        {
            if(block == exit_block)
                continue;

            bool link_to_exit_block = (*block).adjacencies().empty();

            if(!link_to_exit_block)
            {
                for(auto instr = (*block).code().instructions();
                    instr != (*block).code().end_instruction();
                    instr = std::dynamic_pointer_cast<arcturus_quadruple>((*instr).next()))
                {
                    if((*instr).operation == arcturus_operation_code_traits::operation_code::function_return_mir)
                    {
                        link_to_exit_block = true;
                        break;
                    }
                }
            }

            if(link_to_exit_block)
            {
                add_edge(block, exit_block, 0.0);
            }
        }
    }
}

void arcturus_control_flow_graph::flat_basic_blocks(typename arcturus_control_flow_graph::code_type& code)
{
    //??
    code;
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

        const char_type *indent(get_indent(4));

        stream << L"digraph CFG" << std::endl;
        stream << L"{" << std::endl;

        for(const auto& vertex : vertices())
        {
            string_type label((*vertex).label() + L"<br/>");

            auto bb = std::dynamic_pointer_cast<basic_block<arcturus_quadruple>>(vertex);

            for(auto instruction = (*bb).code().instructions();
                instruction != (*bb).code().end_instruction();
                instruction = std::dynamic_pointer_cast<arcturus_quadruple>((*instruction).next()))
            {
                label += (*instruction).to_string() + L"<br/>";
            }

            stream << indent << (*vertex).id() << L" [shape=box label=<" << label << L">];" << std::endl;
        }

        for(const auto& edge : edges())
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
        log_exception(ex, L"Failed to generate graphviz file: error occurred.");
    }

    log_info(L"Generated graphviz file.");
}

END_NAMESPACE
