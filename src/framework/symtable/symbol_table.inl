//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __SYMBOL_TABLE_INL__
#define __SYMBOL_TABLE_INL__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token>
symbol_table<Token>::symbol_table()
{
}

template <typename Token>
symbol_table<Token>::~symbol_table()
{
}

END_NAMESPACE

#endif // __SYMBOL_TABLE_INL__
