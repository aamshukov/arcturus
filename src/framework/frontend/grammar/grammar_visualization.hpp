//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAMMAR_VISUALIZATION_H__
#define __GRAMMAR_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class grammar_visualization : private noncopyable
{
    public:
        using symbol_type = grammar::symbol_type;
        using symbols_type = grammar::symbols_type;

        using set_type = grammar::set_type;
        using sets_type = grammar::sets_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

    public:
        static string_type  decorate_set(const set_type& set, bool add_brackets = true);
        static string_type  decorate_sets(const sets_type& sets, bool add_brackets = true);

        static string_type  decorate_symbol(const symbol_type& symbol);
        static string_type  decorate_symbols(const grammar& grammar);

        static string_type  decorate_rule(const rule_type& rule, bool decorate = true);

        static string_type  decorate_grammar(const grammar& grammar);
};

END_NAMESPACE

#endif // __GRAMMAR_VISUALIZATION_H__
