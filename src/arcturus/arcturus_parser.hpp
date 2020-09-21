//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __ARCTURUS_PARSER_H__
#define __ARCTURUS_PARSER_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_parser : public recursive_descent_parser<arcturus_token>
{
    public:
        using token_type = arcturus_token;

        using lexical_analyzer_type = typename parser<token_type>::lexical_analyzer_type;

    private:
        void    parse() override;

    public:
                arcturus_parser(const lexical_analyzer_type& lexical_analyzer);
               ~arcturus_parser();
};

END_NAMESPACE

#endif // __ARCTURUS_PARSER_H__
