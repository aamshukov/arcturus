//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __ARCTURUS_IR_H__
#define __ARCTURUS_IR_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

class arcturus_ir : public ir<basic_block<arcturus_quadruple>>
{
    public:
        using code_type = typename ir<basic_block<arcturus_quadruple>>::code_type;

        using basic_block_type = typename ir<basic_block<arcturus_quadruple>>::basic_block_type;
        using basic_blocks_type = typename ir<basic_block<arcturus_quadruple>>::basic_blocks_type;

    public:
                        arcturus_ir();
                       ~arcturus_ir();

        virtual void    build_hir(code_type& code) override;
        virtual void    build_mir(code_type& code) override;
        virtual void    build_lir(code_type& code) override;
};

END_NAMESPACE

#endif // __ARCTURUS_IR_H__
