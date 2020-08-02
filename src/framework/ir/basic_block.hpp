//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token, typename OpCodeTraits>
class basic_block : private noncopyable
{
    public:
        using token_type = Token;
        using traits_type = OpCodeTraits;

        using id_type = std::size_t;

        using quadruple_type = std::shared_ptr<quadruple<token_type, traits_type>>;
        using quadruples_type = std::list<quadruple_type>;

        using code_type = std::shared_ptr<code<token_type, traits_type>>;

        using basic_block_type = std::shared_ptr<basic_block<token_type, traits_type>>;
        using basic_blocks_type = std::vector<basic_block_type>;

    private:
        id_type                     my_id; // 0 - entry-block, 1 - exit-block
        string_type                 my_label;

        code_type                   my_code;

        basic_blocks_type           my_successors;
        basic_blocks_type           my_predecessors;

    public:
                                    basic_block(const id_type& id, const string_type& name);
                                   ~basic_block();

        const id_type&              id() const;
        id_type&                    id();

        const string_type&          label() const;
        string_type&                label();

        const code_type&            code() const;
        code_type&                  code();

        const basic_blocks_type&    successors() const;
        basic_blocks_type&          successors();

        const basic_blocks_type&    predecessors() const;
        basic_blocks_type&          predecessors();
};

template <typename Token, typename OpCodeTraits>
inline const typename basic_block<Token, OpCodeTraits>::id_type& basic_block<Token, OpCodeTraits>::id() const
{
    return my_id;
}

template <typename Token, typename OpCodeTraits>
inline typename basic_block<Token, OpCodeTraits>::id_type& basic_block<Token, OpCodeTraits>::id()
{
    return my_id;
}

template <typename Token, typename OpCodeTraits>
inline const string_type& basic_block<Token, OpCodeTraits>::label() const
{
    return my_label;
}

template <typename Token, typename OpCodeTraits>
inline string_type& basic_block<Token, OpCodeTraits>::label()
{
    return my_label;
}

template <typename Token, typename OpCodeTraits>
inline const typename basic_block<Token, OpCodeTraits>::code_type& basic_block<Token, OpCodeTraits>::code() const
{
    return my_code;
}

template <typename Token, typename OpCodeTraits>
inline typename basic_block<Token, OpCodeTraits>::code_type& basic_block<Token, OpCodeTraits>::code()
{
    return my_code;
}

template <typename Token, typename OpCodeTraits>
inline const typename basic_block<Token, OpCodeTraits>::basic_blocks_type& basic_block<Token, OpCodeTraits>::successors() const
{
    return my_successors;
}

template <typename Token, typename OpCodeTraits>
inline typename basic_block<Token, OpCodeTraits>::basic_blocks_type& basic_block<Token, OpCodeTraits>::successors()
{
    return my_successors;
}

template <typename Token, typename OpCodeTraits>
inline const typename basic_block<Token, OpCodeTraits>::basic_blocks_type& basic_block<Token, OpCodeTraits>::predecessors() const
{
    return my_predecessors;
}

template <typename Token, typename OpCodeTraits>
inline typename basic_block<Token, OpCodeTraits>::basic_blocks_type& basic_block<Token, OpCodeTraits>::predecessors()
{
    return my_predecessors;
}

END_NAMESPACE

#endif // __BASIC_BLOCK_H__
