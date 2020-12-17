//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
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

#include <controller/controller.hpp>

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
#include <arcturus_ir.hpp>
#include <arcturus_pass.hpp>
#include <arcturus_controller.hpp>

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)
USINGNAMESPACE(orchestration)
USINGNAMESPACE(arcturus)

using arcturus_instruction = arcturus_quadruple;
using arcturus_code = code<arcturus_instruction>;

void build_code_aho_ullman(arcturus_code& code)
{
    //  1) i = 1 
    //  2) j = 1 
    //  3) tl = 10 * i 
    //  4) t2 = tl + j 
    //  5) t3 = 8 * t2 
    //  6) t4 = t3 - 88 
    //  7) a[t4] = 0.0 
    //  8) j = j + 1 
    //  9) if j <= 10 goto (3) 
    // 10) i = i + 1 
    // 11) if i <= 10 goto (2) 
    // 12) i = 1 
    // 13) t5 = i - 1 
    // 14) t6 = 88 * t5 
    // 15) a[t6] = 1.0 
    // 16) i = i + 1 
    // 17) if i <= 10 goto (13) 
    auto instr1(factory::create<arcturus_instruction>(0, arcturus_operation_code_traits::operation_code::assignment_hir));
    code.add_instruction(instr1);


    string_type code_str(code.to_string());
    std::wcout << code_str << std::endl;
}

void build_code(arcturus_code& code) //??
{
    for(auto it = code.instructions(); ;)
    {
        if(it == code.end_instruction())
            break;
        std::wcout << arcturus_quadruple::opcode_name((*it).operation) << std::endl;
        it = std::static_pointer_cast<arcturus_quadruple>((*it).next());
    }
    std::cout << std::endl;

    auto instruction1(factory::create<arcturus_instruction>(0, arcturus_operation_code_traits::operation_code::unary_op_plus_hir));
    code.add_instruction(instruction1);

    auto instruction2(factory::create<arcturus_quadruple>(1, arcturus_operation_code_traits::operation_code::binary_op_add_hir));
    code.add_instruction(instruction2);

    auto instruction3(factory::create<arcturus_quadruple>(2, arcturus_operation_code_traits::operation_code::binary_op_divide_hir));
    code.add_instruction(instruction3);

    for(auto it = code.instructions(); ;)
    //for(auto it = std::static_pointer_cast<arcturus_quadruple>((*code.instructions()).next()); ;)
    {
        if(it == code.end_instruction())
            break;
        std::wcout << arcturus_quadruple::opcode_name((*it).operation) << std::endl;
        it = std::static_pointer_cast<arcturus_quadruple>((*it).next());
    }
    std::cout << std::endl;

    code.remove_instruction(instruction1);

    for(auto it = code.instructions(); ;)
    {
        if(it == code.end_instruction())
            break;
        std::wcout << arcturus_quadruple::opcode_name((*it).operation) << std::endl;
        it = std::static_pointer_cast<arcturus_quadruple>((*it).next());
    }
    std::cout << std::endl;

    code.remove_instruction(instruction2);

    for(auto it = code.instructions(); ;)
    {
        if(it == code.end_instruction())
            break;
        std::wcout << arcturus_quadruple::opcode_name((*it).operation) << std::endl;
        it = std::static_pointer_cast<arcturus_quadruple>((*it).next());
    }
    std::cout << std::endl;

    code.remove_instruction(instruction3);

    for(auto it = code.instructions(); ;)
    {
        if(it == code.end_instruction())
            break;
        std::wcout << arcturus_quadruple::opcode_name((*it).operation) << std::endl;
        it = std::static_pointer_cast<arcturus_quadruple>((*it).next());
    }
}

int _tmain(int argc, _TCHAR *argv[])
{
    code<arcturus_instruction> code;

    //build_code(code);
    //build_code_aho_ullman(code);

    arcturus_control_flow_graph cfg;

    //cfg.build_hir(code);

    auto st(factory::create<arcturus_scalar_type>(arcturus_type::kind_type::integer_type));
    auto at(factory::create<arcturus_array_type>(st));

    //arcturus_controller controller;

    //controller.compile();

    arcturus_configurator::instance().configure(argc, argv);

    auto it_file = arcturus_configurator::instance().options().find(L"output-file-name");
    auto& path = (*it_file).second;
    auto file = std::filesystem::path(path).filename();
    auto log_path = std::filesystem::path(path).parent_path();
    auto log_file = uniqueue_file_name(file, L".log");
    
    log_path /= log_file;

    arcturus_configurator::instance().options().insert(std::pair(L"output-log-file-name", log_path));

    auto& config = arcturus_configurator::instance();
    config;

    logger::instance().initialize(log_path);

    arcturus_symbol sym;
    sym.type() = st;
    sym.type() = at;

    arcturus_parse_tree f(arcturus_tree_traits::kind::identifier);

    f.kind() = arcturus_tree_traits::kind::identifier;
}
