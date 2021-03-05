//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __CODE_H__
#define __CODE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Instruction>
class code : private noncopyable
{
    public:
        using instruction_type = std::shared_ptr<Instruction>;

    private:                                 // head node ... node tail
        instruction_type            my_head; // head
        instruction_type            my_tail; // tail

    public:
                                    code();
                                   ~code();

        const instruction_type      instructions() const;

        instruction_type            start_instruction();
        instruction_type            end_instruction();

        void                        add_instruction(instruction_type& instruction);
        void                        insert_instruction(instruction_type& instruction, instruction_type prev_instruction = nullptr);

        void                        remove_instruction(instruction_type& instruction);

        void                        clear();

        string_type                 to_string();
};

template <typename Instruction>
inline const typename code<Instruction>::instruction_type code<Instruction>::instructions() const
{
    return std::dynamic_pointer_cast<Instruction>((*my_head).next());
}

template <typename Instruction>
inline typename code<Instruction>::instruction_type code<Instruction>::start_instruction()
{
    return my_head;
}

template <typename Instruction>
inline typename code<Instruction>::instruction_type code<Instruction>::end_instruction()
{
    return my_tail;
}

template <typename Instruction>
code<Instruction>::code()
                 : my_head(factory::create<Instruction>(-1, Instruction::operation_code::sentinel)),
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
inline void code<Instruction>::insert_instruction(typename code<Instruction>::instruction_type& instruction,
                                                  typename code<Instruction>::instruction_type prev_instruction)
{
    if(prev_instruction == nullptr)
        prev_instruction = my_head;

    (*instruction).next() = (*prev_instruction).next();
    (*instruction).prev() = prev_instruction;

    (*(*prev_instruction).next()).prev() = instruction;
    (*prev_instruction).next() = instruction;
}

template <typename Instruction>
inline void code<Instruction>::remove_instruction(typename code<Instruction>::instruction_type& instruction)
{
    (*(*instruction).prev()).next() = (*instruction).next();
    (*(*instruction).next()).prev() = (*instruction).prev();
}

template <typename Instruction>
void code<Instruction>::clear()
{
    auto instruction((*my_head).next());

    while(instruction != my_tail)
    {
        (*(*instruction).prev()).next() = (*instruction).next();
        (*(*instruction).next()).prev() = (*instruction).prev();

        instruction = (*instruction).next();
    }
}

template <typename Instruction>
inline string_type code<Instruction>::to_string()
{
    string_type result;

    for(auto instruction = instructions();
        instruction != end_instruction();
        instruction = std::dynamic_pointer_cast<Instruction>((*instruction).next()))
    {
        result += (*instruction).to_string();
        result += L"\n";
    }

    return result;
}

END_NAMESPACE

#endif // __CODE_H__
