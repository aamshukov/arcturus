﻿//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __GRAPH_H__
#define __GRAPH_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TVertex, typename TEdgeValue = double, std::size_t N = 2>
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
        using vertex_key_comparator = vertex::vertex_key_comparator;
        using vertices_type = std::set<vertex_type, vertex_key_comparator>;
        using vertices_iterator_type = typename vertices_type::iterator;

        using edge_value_type = TEdgeValue;
        using edge_type = std::shared_ptr<edge<TVertex, TEdgeValue, N>>;

        struct edge_key_comparator
        {
            bool operator() (const edge_type& lhs, const edge_type& rhs) const
            {
                return (*lhs).id() < (*rhs).id();
            }
        };

        using edges_type = std::set<edge_type, edge_key_comparator>;
        using edges_iterator_type = typename edges_type::iterator;

        struct vertex_hash
        {
            std::size_t operator () (const vertex_type& vertex) const
            {
                std::size_t result = (*vertex).id();
                result ^= std::hash<std::size_t>{}(result + 0x9E3779B9 + (result << 6) + (result >> 2)); // aka boost hash_combine
                return result;
            }
        };

        using vertex_hash_type = vertex_hash;
        using vertex_edge_map_type = std::unordered_map<vertex_type, edges_type, vertex_hash_type, vertex_key_comparator>;

        using id_type = int;
        using size_type = int;

        using counter_type = counter;

    private:
        vertices_type                       my_vertices;
        counter_type                        my_vertices_counter;

        edges_type                          my_edges;
        counter_type                        my_edges_counter;

        vertex_edge_map_type                my_vertex_edge_map;

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
        void                                remove_edge(const edge_type& edge);
};

template <typename TVertex, typename TEdgeValue, std::size_t N>
graph<TVertex, TEdgeValue, N>::graph()
{
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
graph<TVertex, TEdgeValue, N>::~graph()
{
    my_vertices.clear();
    my_edges.clear();
    my_vertex_edge_map.clear();
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline const typename graph<TVertex, TEdgeValue, N>::vertices_type& graph<TVertex, TEdgeValue, N>::vertices() const
{
    return my_vertices;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline const typename graph<TVertex, TEdgeValue, N>::edges_type& graph<TVertex, TEdgeValue, N>::edges() const
{
    return my_edges;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
std::pair<typename graph<TVertex, TEdgeValue, N>::vertices_iterator_type, bool>
inline graph<TVertex, TEdgeValue, N>::add_vertex(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex)
{
    if((*vertex).id() == 0)
    {
        (*vertex).id() = my_vertices_counter.number();
    }

    (*vertex).adjacencies().clear();

    auto result = my_vertices.emplace(vertex);

    return result;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph<TVertex, TEdgeValue, N>::remove_vertex(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex)
{
    // clean up adjacencies
    auto adjacency_it((*vertex).adjacencies().find(vertex));

    if(adjacency_it != (*vertex).adjacencies().end())
    {
        (*vertex).adjacencies().erase(adjacency_it);
    }

    // clean up vetex/edges map
    auto edge_it(my_vertex_edge_map.find(vertex));

    if(edge_it != my_vertex_edge_map.end())
    {
        auto& edges((*edge_it).second);
        edges.clear();
    }

    // remove vertex
    my_vertices.erase(vertex);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
std::pair<typename graph<TVertex, TEdgeValue, N>::edges_iterator_type, bool>
graph<TVertex, TEdgeValue, N>::add_edge(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex_u,
                                        const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex_v,
                                        const typename graph<TVertex, TEdgeValue, N>::edge_value_type& value)
{
    // adjust identities
    if((*vertex_u).id() == 0)
    {
        (*vertex_u).id() = my_vertices_counter.number();
    }

    if((*vertex_v).id() == 0)
    {
        (*vertex_v).id() = my_vertices_counter.number();
    }

    // add vertex
    (*vertex_u).adjacencies().emplace(vertex_v);

    // add new edge
    auto new_edge(factory::create<edge<TVertex, TEdgeValue, N>>(my_edges_counter.number()));

    (*new_edge).value() = value;

    (*new_edge).endpoints()[0] = vertex_u;
    (*new_edge).endpoints()[1] = vertex_v;

    auto result = my_edges.emplace(new_edge);

    // link vertex with edge(s)
    auto it = my_vertex_edge_map.find(vertex_u);

    if(it != my_vertex_edge_map.end())
    {
        auto& edges((*it).second);
        edges.emplace(new_edge);
    }
    else
    {
        edges_type edges;
        edges.emplace(new_edge);
        my_vertex_edge_map.emplace(vertex_u, edges);
    }

    return result;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph<TVertex, TEdgeValue, N>::remove_edge(const typename graph<TVertex, TEdgeValue, N>::edge_type& edge)
{
    auto& vertex_u((*edge).endpoints()[0]);
    auto& vertex_v((*edge).endpoints()[1]);

    auto adjacency_it((*vertex_u).adjacencies().find(vertex_v));

    if(adjacency_it != (*vertex_u).adjacencies().end())
    {
        (*vertex_u).adjacencies().erase(adjacency_it);
    }

    my_edges.erase(edge);
}

END_NAMESPACE

#endif // __GRAPH_H__
