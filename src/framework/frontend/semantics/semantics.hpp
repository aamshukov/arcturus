//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class semantics : private noncopyable
{
    public:
        using token_type = typename parser::token_type;

        using parse_tree_type = std::shared_ptr<parse_tree>;
        using parse_trees_type = std::vector<parse_tree_type>;

    public:
                semantics();
               ~semantics();

        void    populate_symbol_table(const parse_tree_type& tree);

};

END_NAMESPACE

#endif // __SEMANTICS_H__
