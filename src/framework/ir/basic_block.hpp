//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Instruction>
class basic_block : public vertex
{
    public:
        //using token_type = Token;
        using instruction_type = Instruction;

        using id_type = typename vertex::id_type;

        //using quadruple_type = std::shared_ptr<quadruple<token_type, typename instruction_type::traits_type>>;
        //using quadruples_type = std::list<quadruple_type>;

        using code_type = code<instruction_type>;

        //using basic_block_type = std::shared_ptr<basic_block<token_type, typename instruction_type::traits_type>>;
        //using basic_blocks_type = std::list<basic_block_type>;

    private:
        string_type                 my_label;
        code_type                   my_code;

        //basic_blocks_type           my_successors;
        //basic_blocks_type           my_predecessors;

    public:
                                    basic_block(const id_type& id, const string_type& name);
                                   ~basic_block();

        const string_type&          label() const;
        string_type&                label();

        const code_type&            code() const;
        code_type&                  code();

        //const basic_blocks_type&    successors() const;
        //basic_blocks_type&          successors();

        //const basic_blocks_type&    predecessors() const;
        //basic_blocks_type&          predecessors();
};

template <typename Instruction>
inline const string_type& basic_block<Instruction>::label() const
{
    return my_label;
}

template <typename Instruction>
inline string_type& basic_block<Instruction>::label()
{
    return my_label;
}

template <typename Instruction>
inline const typename basic_block<Instruction>::code_type& basic_block<Instruction>::code() const
{
    return my_code;
}

template <typename Instruction>
inline typename basic_block<Instruction>::code_type& basic_block<Instruction>::code()
{
    return my_code;
}

//template <typename Instruction>
//inline const typename basic_block<Instruction>::basic_blocks_type& basic_block<Instruction>::successors() const
//{
//    return my_successors;
//}
//
//template <typename Instruction>
//inline typename basic_block<Instruction>::basic_blocks_type& basic_block<Instruction>::successors()
//{
//    return my_successors;
//}
//
//template <typename Instruction>
//inline const typename basic_block<Instruction>::basic_blocks_type& basic_block<Instruction>::predecessors() const
//{
//    return my_predecessors;
//}
//
//template <typename Instruction>
//inline typename basic_block<Instruction>::basic_blocks_type& basic_block<Instruction>::predecessors()
//{
//    return my_predecessors;
//}

END_NAMESPACE

#endif // __BASIC_BLOCK_H__
