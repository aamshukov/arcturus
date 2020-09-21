//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __PARSER_INL__
#define __PARSER_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
parser<Token>::parser(const typename parser<Token>::lexical_analyzer_type& lexical_analyzer)
             : my_lexical_analyzer(lexical_analyzer)
{
}

template <typename Token>
parser<Token>::~parser()
{
}

template <typename Token>
typename parser<Token>::tokens_type parser<Token>::synchronization_tokens()
{
    tokens_type sync_tokens;
    return sync_tokens;
}

END_NAMESPACE

#endif // __PARSER_INL__
