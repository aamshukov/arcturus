//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TREE_TAC_VISITOR_H__
#define __TREE_TAC_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)


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
