//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __TREE_TAC_VISITOR_H__
#define __TREE_TAC_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)


template <typename Token, typename TreeTraits>
struct tree_tac_visitor : public visitor<void, bool, parse_tree<Token, TreeTraits>>
{
    using parse_tree_type = parse_tree<Token, TreeTraits>;

    void visit(parse_tree_type& tree, const bool&) override
    {
        tree;//??
    }
};

END_NAMESPACE

#endif // __TREE_TAC_VISITOR_H__
