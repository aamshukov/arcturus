//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __PACKRAT_PARSER_INL__
#define __PACKRAT_PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
packrat_parser<Token>::packrat_parser(const typename packrat_parser<Token>::lexical_analyzer_type& lexical_analyzer)
                     : parser<Token>(lexical_analyzer)
{
}

template <typename Token>
packrat_parser<Token>::~packrat_parser()
{
}

END_NAMESPACE

#endif // __PACKRAT_PARSER_INL__
