//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/list.hpp>
#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/vertex.hpp>
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

void arcturus_control_flow_graph::build_hir(typename arcturus_control_flow_graph::code_type& code)
{
    auto instruction = code.instructions();

    if(instruction != code.end_of_instructions())
    {
        // phase I (collect leaders)
        //  Первая команда (instruction) промежуточного кода является лидером.
        //  Любая команда (instruction), являющаяся целевой для условного или безусловного перехода, является лидером.
        //  Любая команда (instruction), следующая непосредственно за условным или безусловным переходом or 'return', является лидером.
        (*instruction).flags |= arcturus_quadruple::flags_type::leader;

        instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());

        for(; instruction != code.end_of_instructions();)
        {
            if((*instruction).operation == arcturus_operation_code_traits::operation_code::if_true_hir ||
                (*instruction).operation == arcturus_operation_code_traits::operation_code::if_false_hir ||
                (*instruction).operation == arcturus_operation_code_traits::operation_code::goto_hir)
            {
                // Любая команда (instruction), являющаяся целевой для условного или безусловного перехода, является лидером.
                (*std::get<1>((*instruction).result)).flags |= arcturus_quadruple::flags_type::leader;

                // Любая команда (instruction), следующая непосредственно за условным или безусловным переходом ... , является лидером.
                instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());

                if(instruction != code.end_of_instructions())
                {
                    (*instruction).flags |= arcturus_quadruple::flags_type::leader;
                }
            }
            else if((*instruction).operation == arcturus_operation_code_traits::operation_code::function_return_hir)
            {
                // Любая команда (instruction), следующая непосредственно за ... or 'return', является лидером.
                instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());

                if(instruction != code.end_of_instructions())
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

        auto block(factory::create<basic_block<instruction_type>>(id, format(label, id))); // the first block because of Первая команда (instruction) ...

        auto current_block(block);

        do
        {
            (*current_block).code().add_instruction(instruction);

            instruction = std::static_pointer_cast<arcturus_quadruple>((*instruction).next());

            if(((*instruction).flags & arcturus_quadruple::flags_type::leader) == arcturus_quadruple::flags_type::leader)
            {
                // link the current block

                // create a new block
                id++;
                current_block = factory::create<basic_block<instruction_type>>(id, format(label, id));
            }
        }
        while(instruction != code.end_of_instructions());




                //std::wcout << arcturus_quadruple::opcode_name((*it).operation) << std::endl;
        //??code.remove_instruction(instruction);
            //if(((*code.instructions()).flags & arcturus_quadruple::flags_type::leader) == arcturus_quadruple::flags_type::leader)


        // phase III (build CFG)
    }
}

void arcturus_control_flow_graph::build_mir(typename arcturus_control_flow_graph::code_type& code)
{
    //??
    code;
}

void arcturus_control_flow_graph::build_lir(typename arcturus_control_flow_graph::code_type& code)
{
    //??
    code;
}

END_NAMESPACE
