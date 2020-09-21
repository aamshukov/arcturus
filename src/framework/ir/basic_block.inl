//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __BASIC_BLOCK_INL__
#define __BASIC_BLOCK_INL__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

template <typename Instruction>
basic_block<Instruction>::basic_block(const typename basic_block<Instruction>::id_type& id, const string_type& label)
                        : vertex(id), my_label(label)
{
}

template <typename Instruction>
basic_block<Instruction>::~basic_block()
{
}

END_NAMESPACE

#endif // __BASIC_BLOCK_INL__
