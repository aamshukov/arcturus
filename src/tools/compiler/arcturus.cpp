//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <arcturus.hpp>

USING_NAMESPACE(core)
USING_NAMESPACE(symtable)
USING_NAMESPACE(frontend)
USING_NAMESPACE(backend)
USING_NAMESPACE(orchestration)
USING_NAMESPACE(arcturus)

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
    auto instr1(factory::create<arcturus_instruction>(0, arcturus_operation_code_traits::operation_code::assignment_mir));
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

    auto instruction1(factory::create<arcturus_instruction>(0, arcturus_operation_code_traits::operation_code::unary_op_plus_mir));
    code.add_instruction(instruction1);

    auto instruction2(factory::create<arcturus_quadruple>(1, arcturus_operation_code_traits::operation_code::binary_op_add_mir));
    code.add_instruction(instruction2);

    auto instruction3(factory::create<arcturus_quadruple>(2, arcturus_operation_code_traits::operation_code::binary_op_divide_mir));
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

#include <fcntl.h>
#include <io.h>

int _tmain(int argc, _TCHAR *argv[])
{
    auto o = 077;
    auto d = 77;
    o, d;

    _setmode(_fileno(stdout), _O_U16TEXT);

    std::ios::sync_with_stdio(false);

    code<arcturus_instruction> code;

    //build_code(code);
    //build_code_aho_ullman(code);

    arcturus_control_flow_graph cfg;

    //cfg.build_mir(code);

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

    arcturus_parse_tree f(arcturus_tree_traits::kind::identifier);

    f.kind() = arcturus_tree_traits::kind::identifier;
}
