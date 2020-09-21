//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __PARSE_TREE_BASE_H__
#define __PARSE_TREE_BASE_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

template <typename Token>
class parse_tree_base
{
    public:
        using token_type = Token;

        using gr_symbol_type = grammar::symbol_type;

        using ir_symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
        using ir_symbols_type = std::vector<ir_symbol_type>;

    protected:
        gr_symbol_type          my_gr_symbol; // populated by parser
        ir_symbol_type          my_ir_symbol; // populated by further passes

    public:
                                parse_tree_base();
        virtual                ~parse_tree_base();

        const gr_symbol_type&   gr_symbol() const;
        gr_symbol_type&         gr_symbol();

        const ir_symbol_type&   ir_symbol() const;
        ir_symbol_type&         ir_symbol();

        ACCEPT_METHOD;
};

template <typename Token>
parse_tree_base<Token>::parse_tree_base()
{
}

template <typename Token>
parse_tree_base<Token>::~parse_tree_base()
{
}

template <typename Token>
inline const typename parse_tree_base<Token>::gr_symbol_type& parse_tree_base<Token>::gr_symbol() const
{
    return my_gr_symbol;
}

template <typename Token>
inline typename parse_tree_base<Token>::gr_symbol_type& parse_tree_base<Token>::gr_symbol()
{
    return my_gr_symbol;
}

template <typename Token>
inline const typename parse_tree_base<Token>::ir_symbol_type& parse_tree_base<Token>::ir_symbol() const
{
    return my_ir_symbol;
}

template <typename Token>
inline typename parse_tree_base<Token>::ir_symbol_type& parse_tree_base<Token>::ir_symbol()
{
    return my_ir_symbol;
}

END_NAMESPACE

#endif // __PARSE_TREE_BASE_H__
