//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __GRAPH_VERTEX_H__
#define __GRAPH_VERTEX_H__

#pragma once

BEGIN_NAMESPACE(core)

class vertex : public visitable
{
    public:
        using vertex_type = std::shared_ptr<vertex>;

        struct vertex_key_comparator
        {
            bool operator() (const vertex_type& lhs, const vertex_type& rhs) const
            {
                return (*lhs).id() < (*rhs).id();
            }
        };

        using vertices_type = std::set<vertex_type, vertex_key_comparator>;
        using vertices_iterator_type = typename vertices_type::iterator;

        using id_type = std::size_t;

    private:
        id_type                 my_id;
        vertices_type           my_adjacencies;

    public:
                                vertex(const id_type& id);
        virtual                ~vertex();

        const id_type&          id() const;
        id_type&                id();

        const vertices_type&    adjacencies() const;
        vertices_type&          adjacencies();

        ACCEPT_METHOD;
};

inline vertex::vertex(const typename vertex::id_type& id = 0)
             : my_id(id)
{
}

inline vertex::~vertex()
{
}

inline const typename vertex::id_type& vertex::id() const
{
    return my_id;
}

inline typename vertex::id_type& vertex::id()
{
    return my_id;
}

inline const typename vertex::vertices_type& vertex::adjacencies() const
{
    return my_adjacencies;
}

inline typename vertex::vertices_type& vertex::adjacencies()
{
    return my_adjacencies;
}

END_NAMESPACE

#endif // __GRAPH_VERTEX_H__
