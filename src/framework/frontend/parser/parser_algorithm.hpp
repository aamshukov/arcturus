//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_ALGORITHM_H__
#define __PARSER_ALGORITHM_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class parser_algorithm : private noncopyable
{
    public:
        using symbol_type = grammar::symbol_type;
        using symbols_type = grammar::symbols_type;

        using pool_type = grammar::pool_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using set_type = grammar::set_type;
        using sets_type = grammar::sets_type;
};

END_NAMESPACE

#endif // __PARSER_ALGORITHM_H__
