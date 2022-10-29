//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GLR_PARSER_H__
#define __GLR_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class glr_parser : public parser
{
    public:
        using token_type = typename parser::token_type;
        using lexical_analyzer_type = typename parser::lexical_analyzer_type;

    public:
                glr_parser(const lexical_analyzer_type& lexical_analyzer);
               ~glr_parser();
};

END_NAMESPACE

#endif // __GLR_PARSER_H__
