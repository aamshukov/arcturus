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
        using vertex_type = std::shared_ptr<dominator_vertex>;
        using vertices_type = std::set<vertex_type, vertex_lt_key_comparator<dominator_vertex>>;

        using bitset_type = bitset<uint64_t>;

        using id_type = typename vertex::id_type;

    protected:
        vertices_type           my_dominators;
        vertex_type             my_idominator;
        vertices_type           my_frontiers;

        bitset_type             my_bitset;

    public:
                                dominator_vertex(const id_type& id, const string_type& label = empty_string());
        virtual                ~dominator_vertex();

        const vertices_type&    dominators() const;
        vertices_type&          dominators();

        const vertex_type&      idominator() const;
        vertex_type&            idominator();

        const vertices_type&    frontiers() const;
        vertices_type&          frontiers();

        const bitset_type&      bitset() const;
        bitset_type&            bitset();

        ACCEPT_METHOD;
};

inline dominator_vertex::dominator_vertex(const typename dominator_vertex::id_type& id = 0, const string_type& label)
                       : vertex(id, label)
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

inline const typename dominator_vertex::vertices_type& dominator_vertex::frontiers() const
{
    return my_frontiers;
}

inline typename dominator_vertex::vertices_type& dominator_vertex::frontiers()
{
    return my_frontiers;
}

inline const typename dominator_vertex::bitset_type& dominator_vertex::bitset() const
{
    return my_bitset;
}

inline typename dominator_vertex::bitset_type& dominator_vertex::bitset()
{
    return my_bitset;
}

END_NAMESPACE

#endif // __GRAPH_DOMINATOR_VERTEX_H__
