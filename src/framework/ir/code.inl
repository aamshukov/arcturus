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

template <typename Instruction>
code<Instruction>::code()
                 : my_head(factory::create<Instruction>(-1, Instruction::operation_code::sentinel)), //??
                   my_tail(factory::create<Instruction>(-1, Instruction::operation_code::sentinel))
{
    (*my_head).next() = my_tail;
    (*my_tail).prev() = my_head;
}

template <typename Instruction>
code<Instruction>::~code()
{
}

template <typename Instruction>
inline void code<Instruction>::add_instruction(typename code<Instruction>::instruction_type& instruction)
{
    // append to the end, before tail
    (*(*my_tail).prev()).next() = instruction;

    (*instruction).next() = my_tail;
    (*instruction).prev() = (*my_tail).prev();

    (*my_tail).prev() = instruction;
}

template <typename Instruction>
inline void code<Instruction>::remove_instruction(typename code<Instruction>::instruction_type& instruction)
{
    (*(*instruction).prev()).next() = (*instruction).next();
    (*(*instruction).next()).prev() = (*instruction).prev();
}

END_NAMESPACE

#endif // __CODE_INL__
