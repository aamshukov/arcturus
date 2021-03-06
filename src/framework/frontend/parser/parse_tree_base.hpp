//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSE_TREE_BASE_H__
#define __PARSE_TREE_BASE_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

class parse_tree_base
{
    public:
        using token_type = token<token_traits>;

        using gr_symbol_type = grammar::symbol_type;

        using ir_symbol_type = std::shared_ptr<symtable::symbol>;
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

inline parse_tree_base::parse_tree_base()
{
}

inline parse_tree_base::~parse_tree_base()
{
}

inline const typename parse_tree_base::gr_symbol_type& parse_tree_base::gr_symbol() const
{
    return my_gr_symbol;
}

inline typename parse_tree_base::gr_symbol_type& parse_tree_base::gr_symbol()
{
    return my_gr_symbol;
}

inline const typename parse_tree_base::ir_symbol_type& parse_tree_base::ir_symbol() const
{
    return my_ir_symbol;
}

inline typename parse_tree_base::ir_symbol_type& parse_tree_base::ir_symbol()
{
    return my_ir_symbol;
}

END_NAMESPACE

#endif // __PARSE_TREE_BASE_H__
