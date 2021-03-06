//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DOMINANCE_TREE_H__
#define __DOMINANCE_TREE_H__

#pragma once

BEGIN_NAMESPACE(core)

class dominance_tree : public tree, public visitable
{
    public:
        using dominator_vertex_type = std::shared_ptr<dominator_vertex>;

    private:
        dominator_vertex_type           my_vertex;

    public:
                                        dominance_tree(const dominator_vertex_type& vertex);
        virtual                        ~dominance_tree();

        const dominator_vertex_type&    vertex() const;
        dominator_vertex_type&          vertex();
};

inline dominance_tree::dominance_tree(const typename dominance_tree::dominator_vertex_type& vertex)
                     : my_vertex(vertex)
{
}

inline dominance_tree::~dominance_tree()
{
}

inline const typename dominance_tree::dominator_vertex_type& dominance_tree::vertex() const
{
    return my_vertex;
}

inline typename dominance_tree::dominator_vertex_type& dominance_tree::vertex()
{
    return my_vertex;
}

END_NAMESPACE

#endif // __DOMINANCE_TREE_H__
