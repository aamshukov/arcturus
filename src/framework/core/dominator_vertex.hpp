//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __GRAPH_DOMINATOR_VERTEX_H__
#define __GRAPH_DOMINATOR_VERTEX_H__

#pragma once

BEGIN_NAMESPACE(core)

class dominator_vertex : public vertex
{
    public:
        using vertex_type = typename vertex::vertex_type;
        using vertices_type = typename vertex::vertices_type;

        using id_type = typename vertex::id_type;

    protected:
        vertices_type           my_dominators;
        vertex_type             my_idominator;

    public:
                                dominator_vertex(const id_type& id);
        virtual                ~dominator_vertex();

        const vertices_type&    dominators() const;
        vertices_type&          dominators();

        const vertex_type&      idominator() const;
        vertex_type&            idominator();

        ACCEPT_METHOD;
};

inline dominator_vertex::dominator_vertex(const typename dominator_vertex::id_type& id = 0)
                       : vertex(id)
{
}

inline dominator_vertex::~dominator_vertex()
{
}

inline const typename dominator_vertex::vertices_type& dominator_vertex::dominators() const
{
    return my_dominators;
}

inline typename dominator_vertex::vertices_type& dominator_vertex::dominators()
{
    return my_dominators;
}

inline const typename dominator_vertex::vertex_type& dominator_vertex::idominator() const
{
    return my_idominator;
}

inline typename dominator_vertex::vertex_type& dominator_vertex::idominator()
{
    return my_idominator;
}

END_NAMESPACE

#endif // __GRAPH_DOMINATOR_VERTEX_H__
