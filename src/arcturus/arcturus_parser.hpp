//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __ARCTURUS_PARSER_H__
#define __ARCTURUS_PARSER_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_parser : public recursive_descent_parser
{
    public:
        using token_type = typename recursive_descent_parser::token_type;
        using lexical_analyzer_type = typename recursive_descent_parser::lexical_analyzer_type;

    private:
        void    parse() override;

    public:
                arcturus_parser(const lexical_analyzer_type& lexical_analyzer);
               ~arcturus_parser();
};

END_NAMESPACE

#endif // __ARCTURUS_PARSER_H__
