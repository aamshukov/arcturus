//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
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

        using vertex_edge_map_type = std::unordered_map<vertex_type,
                                                        edges_type,
                                                        vertex_hash<TVertex>,
                                                        vertex_eq_key_comparator<TVertex>>;
        using edge_edge_map_type = std::unordered_map<edge_type,
                                                      edge_type,
                                                      edge_hash<TVertex, TEdgeValue, N>,
                                                      edge_eq_key_comparator<TVertex, TEdgeValue, N>>;
        using id_type = std::size_t;
        using counter_type = counter;

    protected:
        vertex_type                         my_root; // optional, used in some digraph algorithms
        vertices_type                       my_vertices;
        counter_type                        my_vertices_counter;

        edges_type                          my_edges;
        counter_type                        my_edges_counter;

        vertex_edge_map_type                my_vertex_edge_map;
        edge_edge_map_type                  my_edge_edge_map; // holds synthetic edges in undirected graphs

        bool                                my_digraph; // directed or not

    private:
        void                                adjust_vertex_id(const vertex_type& vertex);

        edge_type                           create_edge(const vertex_type& vertex_u, const vertex_type& vertex_v, const edge_value_type& value);
        void                                link_edge(const vertex_type& vertex_u, const vertex_type& vertex_v, const edge_type& edge);
    public:
                                            graph(bool digraph = true);
        virtual                            ~graph();

        const vertex_type&                  root() const;
        vertex_type&                        root();

        const vertices_type&                vertices() const;
        const edges_type&                   edges() const;

        bool                                digraph() const;

        std::pair<vertices_iterator_type,
                  bool>                     add_vertex(const vertex_type& vertex);
        void                                remove_vertex(const vertex_type& vertex);

        const edges_type                    get_edges(const vertex_type& vertex) const;

        void                                add_edge(const vertex_type& vertex_u, const vertex_type& vertex_v, const edge_value_type& value);
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
    my_edge_edge_map.clear();
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
    adjust_vertex_id(vertex);

    (*vertex).adjacencies().clear();

    return my_vertices.emplace(vertex);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph<TVertex, TEdgeValue, N>::remove_vertex(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex)
{
    auto map_it(my_vertex_edge_map.find(vertex));

    if(map_it != my_vertex_edge_map.end())
    {
        auto edges((*map_it).second);

        for(auto edge_it = edges.begin(); edge_it != edges.end(); ++edge_it)
        {
            remove_edge(*edge_it);
        }
    }

    my_vertices.erase(vertex);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline const typename graph<TVertex, TEdgeValue, N>::edges_type graph<TVertex, TEdgeValue, N>::get_edges(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex) const
{
    edges_type result;

    auto it(my_vertex_edge_map.find(vertex));

    if(it != my_vertex_edge_map.end())
    {
        result.insert((*it).second.begin(), (*it).second.end());
    }

    return result;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph<TVertex, TEdgeValue, N>::add_edge(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex_u,
                                             const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex_v,
                                             const typename graph<TVertex, TEdgeValue, N>::edge_value_type& value)
{
    // adjust identities
    adjust_vertex_id(vertex_u);
    adjust_vertex_id(vertex_v);

    // add vertex, U -> V
    (*vertex_u).adjacencies().emplace(vertex_v);

    if(!my_digraph)
    {
        // add vertex, V -> U
        (*vertex_v).adjacencies().emplace(vertex_u);
    }

    // add new edge, U -> V
    auto uv_edge = create_edge(vertex_u, vertex_v, value);

    if(!my_digraph)
    {
        // add new edge, V -> U
        auto vu_edge = create_edge(vertex_v, vertex_u, value);

        (*vu_edge).flags() |= edge<TVertex, TEdgeValue, N>::flag::synthetic;

        my_edge_edge_map[uv_edge] = vu_edge;
    }

    // mark V as referenced by U
    (*vertex_v).add_ref();

    if(!my_digraph)
    {
        // mark U as referenced by V
        (*vertex_u).add_ref();
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph<TVertex, TEdgeValue, N>::adjust_vertex_id(const vertex_type& vertex)
{
    auto id = my_vertices_counter.number(); // always increment

    if((*vertex).id() == 0)
    {
        (*vertex).id() = id;
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline typename graph<TVertex, TEdgeValue, N>::edge_type graph<TVertex, TEdgeValue, N>::create_edge(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex_u,
                                                                                                    const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex_v,
                                                                                                    const typename graph<TVertex, TEdgeValue, N>::edge_value_type& value)
{
    auto result(factory::create<edge<TVertex, TEdgeValue, N>>(my_edges_counter.number()));

    (*result).value() = value;

    (*result).endpoints()[0] = vertex_u;
    (*result).endpoints()[1] = vertex_v;

    my_edges.emplace(result);

    link_edge(vertex_u, vertex_v, result);

    return result;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph<TVertex, TEdgeValue, N>::link_edge(const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex_u,
                                                     const typename graph<TVertex, TEdgeValue, N>::vertex_type& vertex_v,
                                                     const typename graph<TVertex, TEdgeValue, N>::edge_type& edge)
{
    // link vertices with edge ...
    // ... V is successor of U
    {
        auto map_it = my_vertex_edge_map.find(vertex_u);

        if(map_it != my_vertex_edge_map.end())
        {
            auto& edges((*map_it).second);
            edges.emplace(edge);
        }
        else
        {
            edges_type edges;
            edges.emplace(edge);
            my_vertex_edge_map.emplace(vertex_u, edges);
        }
    }

    // ... U is predecessor of V
    {
        auto map_it = my_vertex_edge_map.find(vertex_v);

        if(map_it != my_vertex_edge_map.end())
        {
            auto& edges((*map_it).second);
            edges.emplace(edge);
        }
        else
        {
            edges_type edges;
            edges.emplace(edge);
            my_vertex_edge_map.emplace(vertex_v, edges);
        }
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph<TVertex, TEdgeValue, N>::remove_edge(const typename graph<TVertex, TEdgeValue, N>::edge_type& edge)
{
    auto& vertex_u((*edge).endpoints()[0]);
    auto& vertex_v((*edge).endpoints()[1]);

    // break U, V relation
    auto adjacency_v_it((*vertex_u).adjacencies().find(vertex_v));

    if(adjacency_v_it != (*vertex_u).adjacencies().end())
    {
        (*vertex_u).adjacencies().erase(adjacency_v_it);
    }

    if(!my_digraph)
    {
        // break V, U relation
        auto adjacency_u_it((*vertex_v).adjacencies().find(vertex_u));

        if(adjacency_u_it != (*vertex_v).adjacencies().end())
        {
            (*vertex_v).adjacencies().erase(adjacency_u_it);
        }
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

    // cleanup edges synthetic map
    if(!my_digraph)
    {
        auto s_edge_it(my_edge_edge_map.find(edge));

        if(s_edge_it != my_edge_edge_map.end())
        {
            // remove synthetic edge
            my_edges.erase((*s_edge_it).second);

            // unlink synthetic edge
            my_edge_edge_map.erase(s_edge_it);
        }
    }

    // remove edge
    my_edges.erase(edge);

    // decrement ref counter
    (*vertex_v).release();

    if(!my_digraph)
    {
        (*vertex_u).release();
    }
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
