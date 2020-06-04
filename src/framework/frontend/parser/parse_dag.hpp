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
struct parse_dag : public dag, public visitable
{
    using token_type = Token;

    using gr_symbol_type = grammar::symbol_type;

    using ir_symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
    using ir_symbols_type = std::vector<ir_symbol_type>;

    gr_symbol_type gr_symbol;
    ir_symbol_type ir_symbol;

    virtual ~parse_dag()
    {
    }

    ACCEPT_METHOD;
};

END_NAMESPACE

#endif // __PARSE_DAG_H__
