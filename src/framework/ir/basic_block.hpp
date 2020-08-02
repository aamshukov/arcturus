//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token, typename Traits>
class basic_block : private noncopyable
{
    public:
        using token_type = Token;
        using traits_type = Traits;

    public:
        using id_type = std::size_t;

        using quadruple_type = std::shared_ptr<quadruple<token_type, traits_type>>;
        using quadruples_type = std::list<quadruple_type>;

        using basic_block_type = std::shared_ptr<basic_block>;
        using basic_blocks_type = std::vector<basic_block_type>;

    private:
        id_type                     my_id; // 0 - entry-block, 1 - exit-block
        string_type                 my_label;

        quadruples_type             my_code;

        basic_blocks_type           my_successors;
        basic_blocks_type           my_predecessors;

    public:
                                    basic_block(const id_type& id, const string_type& name);
                                   ~basic_block();

        const id_type&              id() const;
        id_type&                    id();

        const string_type&          label() const;
        string_type&                label();

        const quadruples_type&      code() const;
        quadruples_type&            code();

        const basic_blocks_type&    successors() const;
        basic_blocks_type&          successors();

        const basic_blocks_type&    predecessors() const;
        basic_blocks_type&          predecessors();
};

template <typename Token, typename Traits>
inline const typename basic_block<Token, Traits>::id_type& basic_block<Token, Traits>::id() const
{
    return my_id;
}

template <typename Token, typename Traits>
inline typename basic_block<Token, Traits>::id_type& basic_block<Token, Traits>::id()
{
    return my_id;
}

template <typename Token, typename Traits>
inline const string_type& basic_block<Token, Traits>::label() const
{
    return my_label;
}

template <typename Token, typename Traits>
inline string_type& basic_block<Token, Traits>::label()
{
    return my_label;
}

template <typename Token, typename Traits>
inline const typename basic_block<Token, Traits>::quadruples_type& basic_block<Token, Traits>::code() const
{
    return my_code;
}

template <typename Token, typename Traits>
inline typename basic_block<Token, Traits>::quadruples_type& basic_block<Token, Traits>::code()
{
    return my_code;
}

template <typename Token, typename Traits>
inline const typename basic_block<Token, Traits>::basic_blocks_type& basic_block<Token, Traits>::successors() const
{
    return my_successors;
}

template <typename Token, typename Traits>
inline typename basic_block<Token, Traits>::basic_blocks_type& basic_block<Token, Traits>::successors()
{
    return my_successors;
}

template <typename Token, typename Traits>
inline const typename basic_block<Token, Traits>::basic_blocks_type& basic_block<Token, Traits>::predecessors() const
{
    return my_predecessors;
}

template <typename Token, typename Traits>
inline typename basic_block<Token, Traits>::basic_blocks_type& basic_block<Token, Traits>::predecessors()
{
    return my_predecessors;
}

END_NAMESPACE

#endif // __BASIC_BLOCK_H__
