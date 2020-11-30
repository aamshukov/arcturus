//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
// Recursive descent parser with backtracking
//
#ifndef __RECURSIVE_DESCENT_PARSER_INL__
#define __RECURSIVE_DESCENT_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
recursive_descent_parser<Token>::recursive_descent_parser(const typename recursive_descent_parser<Token>::lexical_analyzer_type& lexical_analyzer)
                               : parser<Token>(lexical_analyzer), my_ptr(nullptr)
{
}

template <typename Token>
recursive_descent_parser<Token>::~recursive_descent_parser()
{
}

END_NAMESPACE

#endif // __RECURSIVE_DESCENT_PARSER_INL__
