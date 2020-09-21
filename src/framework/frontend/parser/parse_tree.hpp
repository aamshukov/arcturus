//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __PARSE_TREE_H__
#define __PARSE_TREE_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

template <typename Token>
class parse_tree : public parse_tree_base<Token>, public tree, public visitable
{
    public:
        using token_type = Token;

        using gr_symbol_type = parse_tree_base<token_type>::gr_symbol_type;

        using ir_symbol_type = parse_tree_base<token_type>::ir_symbol_type;
        using ir_symbols_type = parse_tree_base<token_type>::ir_symbols_type;

    public:
                    parse_tree();
        virtual    ~parse_tree();

        ACCEPT_METHOD;
};

template <typename Token>
parse_tree<Token>::parse_tree()
{
}

template <typename Token>
parse_tree<Token>::~parse_tree()
{
}

END_NAMESPACE

#endif // __PARSE_TREE_H__
