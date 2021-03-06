﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................

// Alessandro Warth, James R. Douglass, Todd Millstein
// "Packrat Parsers Can Support Left Recursion"
#ifndef __PACKRAT_PARSER_H__
#define __PACKRAT_PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class packrat_parser : private recursive_descent_parser
{
    // Memoization table:
    //  Memo: (Rule, Position) -> (Tree|Fail, Position)
    //                             node mark  the next char of the input stream
    public:
        using token_type = typename recursive_descent_parser::token_type;
        using lexical_analyzer_type = typename recursive_descent_parser::lexical_analyzer_type;

    public:
                packrat_parser(const lexical_analyzer_type& lexical_analyzer);
               ~packrat_parser();
};

END_NAMESPACE

#endif // __PACKRAT_PARSER_H__
