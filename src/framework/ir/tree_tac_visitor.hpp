//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __TREE_TAC_VISITOR_H__
#define __TREE_TAC_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)


template <typename TreeTraits>
struct tree_tac_visitor : public visitor<void, bool, parse_tree<TreeTraits>>
{
    using parse_tree_type = parse_tree<TreeTraits>;

    void visit(parse_tree_type& tree, const bool&) override
    {
        tree;//??
    }
};

END_NAMESPACE

#endif // __TREE_TAC_VISITOR_H__
