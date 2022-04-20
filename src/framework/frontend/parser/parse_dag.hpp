//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSE_DAG_H__
#define __PARSE_DAG_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USING_NAMESPACE(core)
USING_NAMESPACE(symtable)

class parse_dag : public parse_tree_base, public dag, public visitable
{
    public:
        using token_type = typename parse_tree_base::token_type;

        using gr_symbol_type = typename parse_tree_base::grammar_symbol_type;

        using ir_symbol_type = typename parse_tree_base::ir_symbol_type;
        using ir_symbols_type = typename parse_tree_base::ir_symbols_type;

    public:
                    parse_dag();
        virtual    ~parse_dag();

        ACCEPT_METHOD;
};

inline parse_dag::parse_dag()
{
}

inline parse_dag::~parse_dag()
{
}

END_NAMESPACE

#endif // __PARSE_DAG_H__
