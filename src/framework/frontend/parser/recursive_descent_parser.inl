//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// Recursive descent parser with backtracking
//
#ifndef __RECURSIVE_DESCENT_PARSER_INL__
#define __RECURSIVE_DESCENT_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
recursive_descent_parser<Token, TreeTraits>::recursive_descent_parser(const typename recursive_descent_parser<Token, TreeTraits>::lexical_analyzer_type& lexical_analyzer)
                                           : parser<Token, TreeTraits>(lexical_analyzer)
{
}

template <typename Token, typename TreeTraits>
recursive_descent_parser<Token, TreeTraits>::~recursive_descent_parser()
{
}

END_NAMESPACE

#endif // __RECURSIVE_DESCENT_PARSER_INL__
