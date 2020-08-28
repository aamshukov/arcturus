//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_VERTEX_H__
#define __GRAPH_VERTEX_H__

#pragma once

BEGIN_NAMESPACE(core)

class vertex : public visitable
{
    public:
        using vertices_type = std::list<std::weak_ptr<vertex>>;
        using id_type = std::size_t;

    private:
        id_type         my_id = 0;
        vertices_type   my_adjacencies;

    public:
                        vertex(const id_type& id);
        virtual        ~vertex();

        const id_type&  id() const;
        id_type&        id();

        ACCEPT_METHOD;
};

inline vertex::vertex(const typename vertex::id_type& id)
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

END_NAMESPACE

#endif // __GRAPH_VERTEX_H__
