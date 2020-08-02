//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CODE_H__
#define __CODE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token, typename Traits>
class code : private noncopyable
{
    public:
        using token_type = Token;
        using traits_type = Traits;

    public:
        using id_type = std::size_t;

        using quadruple_type = std::shared_ptr<quadruple<token_type, traits_type>>;
        using quadruples_type = std::list<quadruple_type>;

    private:
        quadruples_type         my_code;

    public:
                                code();
                               ~code();

        const quadruples_type&  quadruples() const;
        quadruples_type&        quadruples();
};

template <typename Token, typename Traits>
inline const typename code<Token, Traits>::quadruples_type& code<Token, Traits>::quadruples() const
{
    return my_code;
}

template <typename Token, typename Traits>
inline typename code<Token, Traits>::quadruples_type& code<Token, Traits>::quadruples()
{
    return my_code;
}

END_NAMESPACE

#endif // __CODE_H__
