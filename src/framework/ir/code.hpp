//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
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

        const instruction_type      start_instruction() const;
        const instruction_type      end_instruction() const;

        void                        add_instruction(instruction_type& instruction);
        void                        remove_instruction(instruction_type& instruction);

        string_type                 to_string();
};

template <typename Instruction>
inline const typename code<Instruction>::instruction_type code<Instruction>::instructions() const
{
    return std::static_pointer_cast<Instruction>((*my_head).next());
}

template <typename Instruction>
inline const typename code<Instruction>::instruction_type code<Instruction>::start_instruction() const
{
    return my_head;
}

template <typename Instruction>
inline const typename code<Instruction>::instruction_type code<Instruction>::end_instruction() const
{
    return my_tail;
}

END_NAMESPACE

#endif // __CODE_H__
