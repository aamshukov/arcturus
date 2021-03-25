//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_IR_SYMBOL_H__
#define __ARCTURUS_IR_SYMBOL_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)

class arcturus_symbol : public symtable::symbol
{
    public:
                            arcturus_symbol();
                            arcturus_symbol(const arcturus_symbol& other);
                            arcturus_symbol(arcturus_symbol&& other);
                           ~arcturus_symbol();

        arcturus_symbol&    operator = (const arcturus_symbol& other);
        arcturus_symbol&    operator = (arcturus_symbol&& other);
};

END_NAMESPACE

#endif // __ARCTURUS_IR_SYMBOL_H__
