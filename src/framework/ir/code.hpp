//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CODE_H__
#define __CODE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token, typename OpCodeTraits>
class code : private noncopyable
{
    public:
        using token_type = Token;
        using traits_type = OpCodeTraits;

    public:
        using id_type = std::size_t;

        using quadruple_type = std::shared_ptr<quadruple<token_type, traits_type>>;
        using quadruples_type = quadruple_type;

        using instruction_type = quadruple_type;
        using instructions_type = quadruples_type;

    private:
        instruction_type            my_head; // head
        instruction_type            my_tail; // tail

    public:
                                    code();
                                   ~code();

        const instructions_type&    instructions() const;
        instructions_type&          instructions();

        bool                        end_of_instructions() const;

        void                        add_instruction(instruction_type instruction);
        void                        remove_instruction(instruction_type instruction);
};

template <typename Token, typename OpCodeTraits>
inline const typename code<Token, OpCodeTraits>::instructions_type& code<Token, OpCodeTraits>::instructions() const
{
    return my_head;
}

template <typename Token, typename OpCodeTraits>
inline typename code<Token, OpCodeTraits>::instructions_type& code<Token, OpCodeTraits>::instructions()
{
    return my_head;
}

template <typename Token, typename OpCodeTraits>
inline bool code<Token, OpCodeTraits>::end_of_instructions() const
{
    return my_tail;
}

END_NAMESPACE

#endif // __CODE_H__
