//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __ARCTURUS_IR_SYMBOL_H__
#define __ARCTURUS_IR_SYMBOL_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

class arcturus_symbol : public symtable::symbol
{
    public:
            arcturus_symbol(const id_type& id);
           ~arcturus_symbol();
};

END_NAMESPACE

#endif // __ARCTURUS_IR_SYMBOL_H__
