//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSE_DAG_H__
#define __PARSE_DAG_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

template <typename Token>
class parse_dag : public parse_tree_base<Token>, public dag, public visitable
{
    public:
        using token_type = Token;

        using gr_symbol_type = parse_tree_base<token_type>::gr_symbol_type;

        using ir_symbol_type = parse_tree_base<token_type>::ir_symbol_type;
        using ir_symbols_type = parse_tree_base<token_type>::ir_symbols_type;

    public:
                    parse_dag();
        virtual    ~parse_dag();

        ACCEPT_METHOD;
};

template <typename Token>
parse_dag<Token>::parse_dag()
{
}

template <typename Token>
parse_dag<Token>::~parse_dag()
{
}

END_NAMESPACE

#endif // __PARSE_DAG_H__
