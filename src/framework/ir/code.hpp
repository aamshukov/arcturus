//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CODE_H__
#define __CODE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

template <typename TInstruction>
class code : private noncopyable
{
    public:
        using instruction_type = std::shared_ptr<TInstruction>;

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

template <typename TInstruction>
inline const typename code<TInstruction>::instruction_type code<TInstruction>::instructions() const
{
    return std::dynamic_pointer_cast<TInstruction>((*my_head).next());
}

template <typename TInstruction>
inline typename code<TInstruction>::instruction_type code<TInstruction>::start_instruction()
{
    return my_head;
}

template <typename TInstruction>
inline typename code<TInstruction>::instruction_type code<TInstruction>::end_instruction()
{
    return my_tail;
}

template <typename TInstruction>
code<TInstruction>::code()
                 : my_head(factory::create<TInstruction>(-1, TInstruction::operation_code::sentinel)),
                   my_tail(factory::create<TInstruction>(-1, TInstruction::operation_code::sentinel))
{
    (*my_head).next() = my_tail;
    (*my_tail).prev() = my_head;
}

template <typename TInstruction>
code<TInstruction>::~code()
{
}

template <typename TInstruction>
inline void code<TInstruction>::add_instruction(typename code<TInstruction>::instruction_type& instruction)
{
    // append to the end, before tail
    (*(*my_tail).prev()).next() = instruction;

    (*instruction).next() = my_tail;
    (*instruction).prev() = (*my_tail).prev();

    (*my_tail).prev() = instruction;
}

template <typename TInstruction>
inline void code<TInstruction>::insert_instruction(typename code<TInstruction>::instruction_type& instruction,
                                                   typename code<TInstruction>::instruction_type prev_instruction)
{
    if(prev_instruction == nullptr)
        prev_instruction = my_head;

    (*instruction).next() = (*prev_instruction).next();
    (*instruction).prev() = prev_instruction;

    (*(*prev_instruction).next()).prev() = instruction;
    (*prev_instruction).next() = instruction;
}

template <typename TInstruction>
inline void code<TInstruction>::remove_instruction(typename code<TInstruction>::instruction_type& instruction)
{
    (*(*instruction).prev()).next() = (*instruction).next();
    (*(*instruction).next()).prev() = (*instruction).prev();
}

template <typename TInstruction>
void code<TInstruction>::clear()
{
    auto instruction((*my_head).next());

    while(instruction != my_tail)
    {
        (*(*instruction).prev()).next() = (*instruction).next();
        (*(*instruction).next()).prev() = (*instruction).prev();

        instruction = (*instruction).next();
    }
}

template <typename TInstruction>
inline string_type code<TInstruction>::to_string()
{
    string_type result;

    for(auto instruction = instructions();
        instruction != end_instruction();
        instruction = std::dynamic_pointer_cast<TInstruction>((*instruction).next()))
    {
        result += (*instruction).to_string();
        result += L"\n";
    }

    return result;
}

END_NAMESPACE

#endif // __CODE_H__
