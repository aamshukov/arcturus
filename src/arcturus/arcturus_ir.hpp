//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_IR_H__
#define __ARCTURUS_IR_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

class arcturus_ir : public ir<arcturus_token, arcturus_operation_code_traits>
{
    public:
        using basic_ir = ir<arcturus_token, arcturus_operation_code_traits>;

        using id_type = std::size_t;

        using quadruple_type = basic_ir::quadruple_type;
        using quadruples_type = basic_ir::quadruples_type;

        using code_type = basic_ir::code_type;

        using basic_block_type = basic_ir::basic_block_type;
        using basic_blocks_type = basic_ir::basic_blocks_type;

    public:
                arcturus_ir();
               ~arcturus_ir();
};

END_NAMESPACE

#endif // __ARCTURUS_IR_H__
