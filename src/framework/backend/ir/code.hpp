//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CODE_H__
#define __CODE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token>
class code : private noncopyable
{
    public:
        using token_type = Token;

    public:
        using id_type = std::size_t;

        using quadruple_type = std::shared_ptr<quadruple<token_traits>>;
        using quadruples_type = std::list<quadruple_type>;

    private:
        quadruples_type         my_quadruples;

    public:
                                code();
                               ~code();

        const quadruples_type&  quadruples() const;
        quadruples_type&        quadruples();
};

template <typename Token>
inline const typename code<Token>::quadruples_type& code<Token>::quadruples() const
{
    return my_code;
}

template <typename Token>
inline typename code<Token>::quadruples_type& code<Token>::quadruples()
{
    return my_code;
}

END_NAMESPACE

#endif // __CODE_H__
