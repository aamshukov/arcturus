//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// Recursive descent parser with backtracking
//
#ifndef __RECURSIVE_DESCENT_PARSER_H__
#define __RECURSIVE_DESCENT_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
class recursive_descent_parser : private parser<Token, TreeTraits>
{
    public:
        using token_type = typename parser<Token, TreeTraits>::token_type;
        using tree_traits_type = typename parser<Token, TreeTraits>::tree_traits_type;

        using lexical_analyzer_type = typename parser<token_type, tree_traits_type>::lexical_analyzer_type;

    public:
                        recursive_descent_parser(const lexical_analyzer_type& lexical_analyzer);
                       ~recursive_descent_parser();
};

END_NAMESPACE

#endif // __RECURSIVE_DESCENT_PARSER_H__
