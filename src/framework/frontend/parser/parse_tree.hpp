//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSE_TREE_H__
#define __PARSE_TREE_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USING_NAMESPACE(core)
USING_NAMESPACE(symtable)

class parse_tree : public parse_tree_base, public tree, public visitable
{
    public:
        using token_type = typename parse_tree_base::token_type;

        using gr_symbol_type = typename parse_tree_base::gr_symbol_type;

        using ir_symbol_type = typename parse_tree_base::ir_symbol_type;
        using ir_symbols_type = typename parse_tree_base::ir_symbols_type;

    public:
                    parse_tree();
        virtual    ~parse_tree();

        ACCEPT_METHOD;
};

inline parse_tree::parse_tree()
{
}

inline parse_tree::~parse_tree()
{
}

END_NAMESPACE

#endif // __PARSE_TREE_H__
