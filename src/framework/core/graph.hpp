﻿//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __GRAPH_H__
#define __GRAPH_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TVertex, typename TEdgeValue = double, std::size_t N = GraphConnectivityRank>
class graph : private noncopyable
{
    protected:
        enum class color
        {
            white = 0,  // not processed
            grey,       // processed
            black       // processed with kids
        };

    public:
        using vertex_type = std::shared_ptr<TVertex>;
        using vertices_type = std::set<vertex_type, vertex_lt_key_comparator<TVertex>>;
        using vertices_iterator_type = typename vertices_type::iterator;

        using edge_value_type = TEdgeValue;
        using edge_type = std::shared_ptr<edge<TVertex, TEdgeValue, N>>;
        using edges_type = std::set<edge_type, edge_lt_key_comparator<TVertex, TEdgeValue, N>>;
        using edges_iterator_type = typename edges_type::iterator;

        using vertex_edge_map_type = std::unordered_map<vertex_type, edges_type, vertex_hash<TVertex>, vertex_eq_key_comparator<TVertex>>;

        using id_type = size_type;

        using counter_type = counter;

    protected:
        vertex_type                         my_root; // optional, used in some digraph algorithms
        vertices_type                       my_vertices;
        counter_type                        my_vertices_counter;

        edges_type                          my_edges;
        counter_type                        my_edges_counter;

        vertex_edge_map_type                my_vertex_edge_map;

        bool                                my_digraph; // directed or not

    public:
                                            graph(bool digraph = true);
                                           ~graph();

        const vertex_type&                  root() const;
        vertex_type&                        root();

        const vertices_type&                vertices() const;
        const edges_type&                   edges() const;

        bool                                digraph() const;

        std::pair<vertices_iterator_type,
                  bool>                     add_vertex(const vertex_type& vertex);
        void                                remove_vertex(const vertex_type& vertex);

        std::pair<edges_iterator_type,
                  bool>                     add_edge(const vertex_type& vertex_u,
                                                     const vertex_type& vertex_v,
                                                     const edge_value_type& value);
        void                                remove_edge(const edge_type& edge);

        void                                collect_predecessors(const vertex_type& vertex, vertices_type& predecessors) const;
        void                                collect_successors(const vertex_type& vertex, vertices_type& successors) const;
};

template <typename TVertex, typename TEdgeValue, std::size_t N>
graph<TVertex, TEdgeValue, N>::graph(bool digraph)
                             : my_digraph(digraph)
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
inline const typename graph<TVertex, TEdgeValue, N>::vertex_type& graph<TVertex, TEdgeValue, N>::root() const
{
    return my_root;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline typename graph<TVertex, TEdgeValue, N>::vertex_type& graph<TVertex, TEdgeValue, N>::root()
{
    return my_root;
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
inline bool graph<TVertex, TEdgeValue, N>::digraph() const
{
    return my_digraph;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
std::pair<typename graph<TVertex, TEdgeValue, N>::vertices_iterator_type, bool>
inline graph<TVertex, TEdgeValue, N>::add_vertex(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex)
{
    if((*vertex).id() == 0)
    {
        (*vertex).id() = my_vertices_counter.number();
    }
    else
    {
        my_vertices_counter.number();
    }

    (*vertex).adjacencies().clear();

    auto result = my_vertices.emplace(vertex);

    return result;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph<TVertex, TEdgeValue, N>::remove_vertex(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex)
{
    // check if the vertex is involved in any ralation ...
    auto map_it(my_vertex_edge_map.find(vertex));

    if(map_it != my_vertex_edge_map.end())
    {
        auto& edges((*map_it).second);

        // .. involved - iterate over all edges and clean up adjacencies of involved vertices
        for(auto edge_it = edges.begin(); edge_it != edges.end();)
        {
            auto& vertex_u((*(*edge_it)).endpoints()[0]);
            auto& vertex_v((*(*edge_it)).endpoints()[1]);

            if(vertex_u == vertex || vertex_v == vertex)
            {
                // clean up adjacencies of U and/or V
                for(auto& vertex_uv : (*(*edge_it)).endpoints())
                {
                    auto adjacency_it((*vertex_uv).adjacencies().find(vertex));

                    if(adjacency_it != (*vertex_uv).adjacencies().end())
                    {
                        (*vertex_uv).adjacencies().erase(adjacency_it);
                    }
                }

                // .. remove edge itself
                my_edges.erase(*edge_it);
                edges.erase(edge_it++);
            }
            else
            {
                ++edge_it;
            }
        }
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
    else
    {
        my_vertices_counter.number();
    }

    if((*vertex_v).id() == 0)
    {
        (*vertex_v).id() = my_vertices_counter.number();
    }
    else
    {
        my_vertices_counter.number();
    }

    // add vertex, U -(>) V
    (*vertex_u).adjacencies().emplace(vertex_v);

    // add new edge
    auto new_edge(factory::create<edge<TVertex, TEdgeValue, N>>(my_edges_counter.number()));

    (*new_edge).value() = value;

    (*new_edge).endpoints()[0] = vertex_u;
    (*new_edge).endpoints()[1] = vertex_v;

    auto result = my_edges.emplace(new_edge);

    // link vertices with edge ...
    // ... V is successor of U
    {
        auto map_it = my_vertex_edge_map.find(vertex_u);

        if(map_it != my_vertex_edge_map.end())
        {
            auto& edges((*map_it).second);
            edges.emplace(new_edge);
        }
        else
        {
            edges_type edges;
            edges.emplace(new_edge);
            my_vertex_edge_map.emplace(vertex_u, edges);
        }
    }

    // ... U is predecessor of V
    {
        auto map_it = my_vertex_edge_map.find(vertex_v);

        if(map_it != my_vertex_edge_map.end())
        {
            auto& edges((*map_it).second);
            edges.emplace(new_edge);
        }
        else
        {
            edges_type edges;
            edges.emplace(new_edge);
            my_vertex_edge_map.emplace(vertex_v, edges);
        }
    }

    // mark V as referenced
    (*vertex_v).add_ref();

    return result;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph<TVertex, TEdgeValue, N>::remove_edge(const typename graph<TVertex, TEdgeValue, N>::edge_type& edge)
{
    // break U, V relation
    auto& vertex_u((*edge).endpoints()[0]);
    auto& vertex_v((*edge).endpoints()[1]);

    auto adjacency_it((*vertex_u).adjacencies().find(vertex_v));

    if(adjacency_it != (*vertex_u).adjacencies().end())
    {
        (*vertex_u).adjacencies().erase(adjacency_it);
    }

    // clean up vertex/edges map
    for(auto& vertex_uv : (*edge).endpoints())
    {
        auto map_it(my_vertex_edge_map.find(vertex_uv));

        if(map_it != my_vertex_edge_map.end())
        {
            auto& edges((*map_it).second);
            edges.erase(edge);
        }
    }

    // reduce ref counter
    (*vertex_v).release();

    // remove edge
    my_edges.erase(edge);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph<TVertex, TEdgeValue, N>::collect_predecessors(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex,
                                                         typename graph<TVertex, TEdgeValue, N>::vertices_type& predecessors) const
{
    predecessors.clear();

    auto map_it(my_vertex_edge_map.find(vertex));

    if(map_it != my_vertex_edge_map.end())
    {
        const auto& edges((*map_it).second);

        for(const auto& edge : edges)
        {
            const auto& vertex_u((*edge).endpoints()[0]);
            const auto& vertex_v((*edge).endpoints()[1]);

            if(vertex_v == vertex) // incoming edge
            {
                predecessors.emplace(vertex_u);
            }
        }
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph<TVertex, TEdgeValue, N>::collect_successors(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex,
                                                       typename graph<TVertex, TEdgeValue, N>::vertices_type& successors) const
{
    successors.clear();

    for(const auto& successor : (*vertex).adjacencies())
    {
        successors.emplace(std::dynamic_pointer_cast<TVertex>(successor));
    }
}

END_NAMESPACE

#endif // __GRAPH_H__
