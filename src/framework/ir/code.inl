//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CODE_INL__
#define __CODE_INL__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

template <typename Token, typename Traits>
code<Token, Traits>::code()
{
}

template <typename Token, typename Traits>
code<Token, Traits>::~code()
{
}

template <typename Token, typename Traits>
inline void code<Token, Traits>::add_instruction(typename code<Token, Traits>::instruction_type instruction)
{
    if(my_tail == nullptr)
    {
        list::link(my_tail, instruction);
    }
    else
    {
        my_head = my_tail = instruction;
    }
}

template <typename Token, typename Traits>
inline void code<Token, Traits>::remove_instruction(typename code<Token, Traits>::instruction_type instruction)
{
    list::unlink(instruction); // does not affect instruction

    if(instruction == my_head)
    {
        my_head = (*instruction).next();
    }

    if(instruction == my_tail)
    {
        my_tail = (*instruction).prev();
    }
}

END_NAMESPACE

#endif // __CODE_INL__
