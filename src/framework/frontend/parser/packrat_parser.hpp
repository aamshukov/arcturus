//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __PACKRAT_PARSER_H__
#define __PACKRAT_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
class packrat_parser : private recursive_descent_parser<Token>
{
    //?? Memoization table --> key<position(my_ptr), rule(my_id)>  value<tree>
    public:
        using token_type = Token;

        using lexical_analyzer_type = typename parser<token_type>::lexical_analyzer_type;

    public:
                packrat_parser(const lexical_analyzer_type& lexical_analyzer);
               ~packrat_parser();
};

END_NAMESPACE

#endif // __PACKRAT_PARSER_H__
