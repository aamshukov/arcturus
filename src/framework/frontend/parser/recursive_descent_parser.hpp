//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
// Recursive descent parser with backtracking
//
#ifndef __RECURSIVE_DESCENT_PARSER_H__
#define __RECURSIVE_DESCENT_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
class recursive_descent_parser : public parser<Token>
{
    public:
        using token_type = Token;

        using lexical_analyzer_type = typename parser<token_type>::lexical_analyzer_type;

    protected:
        const datum_type*   my_ptr; // master/slave lexer(s) backtrack position(s)

    public:
                            recursive_descent_parser(const lexical_analyzer_type& lexical_analyzer);
                           ~recursive_descent_parser();
};

END_NAMESPACE

#endif // __RECURSIVE_DESCENT_PARSER_H__
