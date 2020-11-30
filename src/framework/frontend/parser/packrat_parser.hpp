//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
// Alessandro Warth, James R. Douglass, Todd Millstein
// "Packrat Parsers Can Support Left Recursion"
#ifndef __PACKRAT_PARSER_H__
#define __PACKRAT_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
class packrat_parser : private recursive_descent_parser<Token>
{
    // Memoization table:
    //  Memo: (Rule, Position) -> (Tree|Fail, Position)
    //                             node mark  the next char of the input stream
    public:
        using token_type = Token;

        using lexical_analyzer_type = typename parser<token_type>::lexical_analyzer_type;

    public:
                packrat_parser(const lexical_analyzer_type& lexical_analyzer);
               ~packrat_parser();
};

END_NAMESPACE

#endif // __PACKRAT_PARSER_H__
