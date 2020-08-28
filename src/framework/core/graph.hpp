//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_H__
#define __GRAPH_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TVertex, typename TEdge>
class graph : private noncopyable
{
    private:
        enum class color
        {
            white = 0,  // not processed
            grey,       // processed
            black       // processed with kids
        };

    public:
        using vertex_type = std::shared_ptr<TVertex>;

        struct vertex_key_comparator
        {
            bool operator() (const vertex_type& lhs, const vertex_type& rhs) const
            {
                return (*lhs).id() < (*rhs).id();
            }
        };

        using vertices_type = std::set<vertex_type, vertex_key_comparator>;
        using vertices_iterator_type = typename vertices_type::iterator;

        using edge_type = std::shared_ptr<TEdge>;
        using edge_value_type = typename TEdge::value_type;

        struct edge_key_comparator
        {
            bool operator() (const edge_type& lhs, const edge_type& rhs) const
            {
                return (*lhs).id() < (*rhs).id();
            }
        };

        using edges_type = std::set<edge_type, edge_key_comparator>;
        using edges_iterator_type = typename edges_type::iterator;

        using id_type = int;
        using size_type = int;

        using counter_type = counter;

    private:
        vertices_type                       my_vertices;
        counter_type                        my_vertices_counter;

        edges_type                          my_edges;
        counter_type                        my_edges_counter;

    public:
                                            graph();
                                           ~graph();

        const vertices_type&                vertices() const;
        const edges_type&                   edges() const;

        std::pair<vertices_iterator_type,
                  bool>                     add_vertex(const vertex_type& vertex);
        void                                remove_vertex(const vertex_type& vertex);

        std::pair<edges_iterator_type,
                  bool>                     add_edge(const vertex_type& vertex_u,
                                                     const vertex_type& vertex_v,
                                                     const edge_value_type& value);
        std::pair<edges_iterator_type,
                  bool>                     add_edge(const edge_type& edge);
        void                                remove_edge(const edge_type& edge);
};

template <typename TVertex, typename TEdge>
graph<TVertex, TEdge>::graph()
{
}

template <typename TVertex, typename TEdge>
graph<TVertex, TEdge>::~graph()
{
    my_vertices.clear();
    my_edges.clear();
}

template <typename TVertex, typename TEdge>
inline const typename graph<TVertex, TEdge>::vertices_type& graph<TVertex, TEdge>::vertices() const
{
    return my_vertices;
}

template <typename TVertex, typename TEdge>
inline const typename graph<TVertex, TEdge>::edges_type& graph<TVertex, TEdge>::edges() const
{
    return my_edges;
}

template <typename TVertex, typename TEdge>
std::pair<typename graph<TVertex, TEdge>::vertices_type::iterator, bool>
graph<TVertex, TEdge>::add_vertex(const typename graph<TVertex, TEdge>::vertex_type& vertex)
{
    if((*vertex).id() == 0)
    {
        (*vertex).id() = my_vertices_counter.number();
    }

    auto result = my_vertices.emplace(vertex);

    return result;
}

template <typename TVertex, typename TEdge>
void graph<TVertex, TEdge>::remove_vertex(const typename graph<TVertex, TEdge>::vertex_type& vertex)
{
    my_vertices.erase(vertex);
}

template <typename TVertex, typename TEdge>
std::pair<typename graph<TVertex, TEdge>::edges_iterator_type, bool>
graph<TVertex, TEdge>::add_edge(const typename graph<TVertex, TEdge>::vertex_type& vertex_u,
                                const typename graph<TVertex, TEdge>::vertex_type& vertex_v,
                                const typename graph<TVertex, TEdge>::edge_value_type& value)
{
    if((*vertex_u).id() == 0)
    {
        (*vertex_u).id() = my_vertices_counter.number();
    }

    if((*vertex_v).id() == 0)
    {
        (*vertex_v).id() = my_vertices_counter.number();
    }

    (*vertex_u).adjacencies().emplace_back(vertex_v);
    (*vertex_v).adjacencies().emplace_back(vertex_u);

    auto new_edge(factory::create<TEdge>(my_edges_counter.number()));

    (*new_edge).value() = value;

    (*new_edge).endpoints()[0] = vertex_u;
    (*new_edge).endpoints()[1] = vertex_v;

    auto result = add_edge(new_edge);

    return result;
}

template <typename TVertex, typename TEdge>
std::pair<typename graph<TVertex, TEdge>::edges_iterator_type, bool>
graph<TVertex, TEdge>::add_edge(const typename graph<TVertex, TEdge>::edge_type& edge)
{
    auto result = my_edges.emplace(edge);
    return result;
}

template <typename TVertex, typename TEdge>
void graph<TVertex, TEdge>::remove_edge(const typename graph<TVertex, TEdge>::edge_type& edge)
{
    my_edges.erase(edge);
}

END_NAMESPACE

#endif // __GRAPH_H__
