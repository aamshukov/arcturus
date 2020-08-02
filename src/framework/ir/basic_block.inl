//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __BASIC_BLOCK_INL__
#define __BASIC_BLOCK_INL__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

template <typename Token, typename Traits>
basic_block<Token, Traits>::basic_block(const typename basic_block<Token, Traits>::id_type& id, const string_type& label)
                          : my_id(id), my_label(label)
{
}

template <typename Token, typename Traits>
basic_block<Token, Traits>::~basic_block()
{
}

END_NAMESPACE

#endif // __BASIC_BLOCK_INL__
