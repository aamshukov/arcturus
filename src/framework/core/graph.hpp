//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_H__
#define __GRAPH_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename VertexType, typename EdgeType, typename VisitorType>
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
        using vertex_type = std::shared_ptr<VertexType>;
        using vertex_value_type = typename VertexType::vertex_value_type;

        struct vertex_key_comparator
        {
            bool operator() (const vertex_type& lhs, const vertex_type& rhs) const
            {
                return (*lhs).id < (*rhs).id;
            }
        };

        using vertices_type = std::set<vertex_type, vertex_key_comparator>;

        using edge_type = std::shared_ptr<EdgeType>;
        using edge_value_type = typename EdgeType::edge_value_type;

        struct edge_key_comparator
        {
            bool operator() (const edge_type& lhs, const edge_type& rhs) const
            {
                return (*lhs).id < (*rhs).id;
            }
        };

        using edges_type = std::set<edge_type, edge_key_comparator>;

        using visitor_type = VisitorType;

        using ve_map_type = std::map<vertex_type, edges_type, vertex_key_comparator>;

        using adjacency_list_type = std::map<vertex_type, vertices_type, vertex_key_comparator>;

        using index_type = int;
        using size_type = int;

        using counter_type = counter;

    private:
        vertices_type                   my_vertices;
        counter_type                    my_vertices_counter;

        edges_type                      my_edges;
        counter_type                    my_edges_counter;

        ve_map_type                     my_ve_map; // mapping vertex to edge

    public:
                                        graph();
                                       ~graph();

        const vertices_type&            vertices() const;
        const edges_type&               edges() const;

        std::pair<vertex_type, bool>    add_vertex(const vertex_value_type& value);
        std::pair<vertex_type, bool>    add_vertex(const vertex_type& vertex);

        std::pair<vertex_type, bool>    remove_vertex(const index_type& id);

        std::pair<edge_type, bool>      add_edge(const vertex_type& vertex_u,
                                                 const vertex_type& vertex_v,
                                                 const edge_value_type& value);
        std::pair<edge_type, bool>      add_edge(const edge_type& edge);

        std::pair<edge_type, bool>      remove_edge(const index_type& id);

        void                            build_adjacency_list(adjacency_list_type& adjacency_list);

        //static void                     dfs(const graph_type& g, const visitor_type& visitor);
        //static bool                     has_cycle(const graph_type& g);
};

template <typename VertexType, typename EdgeType, typename VisitorType>
graph<VertexType, EdgeType, VisitorType>::graph()
{
}

template <typename VertexType, typename EdgeType, typename VisitorType>
graph<VertexType, EdgeType, VisitorType>::~graph()
{
    my_ve_map.clear();
    my_vertices.clear();
    my_edges.clear();
}

template <typename VertexType, typename EdgeType, typename VisitorType>
inline const typename graph<VertexType, EdgeType, VisitorType>::vertices_type& graph<VertexType, EdgeType, VisitorType>::vertices() const
{
    return my_vertices;
}

template <typename VertexType, typename EdgeType, typename VisitorType>
inline const typename graph<VertexType, EdgeType, VisitorType>::edges_type& graph<VertexType, EdgeType, VisitorType>::edges() const
{
    return my_edges;
}

template <typename VertexType, typename EdgeType, typename VisitorType>
std::pair<typename graph<VertexType, EdgeType, VisitorType>::vertex_type, bool>
graph<VertexType, EdgeType, VisitorType>::add_vertex(const typename graph<VertexType, EdgeType, VisitorType>::vertex_value_type& value)
{
    auto new_vertex(factory::create<VertexType>());

    (*new_vertex).id = my_vertices_counter.number();
    (*new_vertex).value = value;

    auto result = add_vertex(new_vertex);

    return result;
}

template <typename VertexType, typename EdgeType, typename VisitorType>
std::pair<typename graph<VertexType, EdgeType, VisitorType>::vertex_type, bool>
graph<VertexType, EdgeType, VisitorType>::add_vertex(const typename graph<VertexType, EdgeType, VisitorType>::vertex_type& vertex)
{
    if((*vertex).id == 0)
    {
        (*vertex).id = my_vertices_counter.number();
    }

    my_vertices.emplace(vertex);

    visitor_type vis;
    auto it = vertices().begin();
    auto vert = *it;
    (*vert).accept(vis);//??

    auto result(std::make_pair(nullptr, false));

    return result;
}

template <typename VertexType, typename EdgeType, typename VisitorType>
std::pair<typename graph<VertexType, EdgeType, VisitorType>::vertex_type, bool>
graph<VertexType, EdgeType, VisitorType>::remove_vertex(const typename graph<VertexType, EdgeType, VisitorType>::index_type& id)
{
    auto result(std::make_pair(nullptr, false));

    return result;
}

template <typename VertexType, typename EdgeType, typename VisitorType>
std::pair<typename graph<VertexType, EdgeType, VisitorType>::edge_type, bool>
graph<VertexType, EdgeType, VisitorType>::add_edge(const typename graph<VertexType, EdgeType, VisitorType>::vertex_type& vertex_u,
                                                   const typename graph<VertexType, EdgeType, VisitorType>::vertex_type& vertex_v,
                                                   const typename graph<VertexType, EdgeType, VisitorType>::edge_value_type& value)
{
    auto new_edge(factory::create<EdgeType>());

    (*new_edge).id = my_edges_counter.number();
    (*new_edge).value = value;

    (*new_edge).endpoints[0] = vertex_u;
    (*new_edge).endpoints[0] = vertex_v;

    auto result = add_edge(new_edge);

    return result;
}

template <typename VertexType, typename EdgeType, typename VisitorType>
std::pair<typename graph<VertexType, EdgeType, VisitorType>::edge_type, bool>
graph<VertexType, EdgeType, VisitorType>::add_edge(const typename graph<VertexType, EdgeType, VisitorType>::edge_type& edge)
{
    if((*edge).id == 0)
    {
        (*edge).id = my_edges_counter.number();
    }


    auto result(std::make_pair(nullptr, false));

    //result.first = new_edge;
    //result.second = false;

    return result;
}

template <typename VertexType, typename EdgeType, typename VisitorType>
std::pair<typename graph<VertexType, EdgeType, VisitorType>::edge_type, bool>
graph<VertexType, EdgeType, VisitorType>::remove_edge(const typename graph<VertexType, EdgeType, VisitorType>::index_type& id)
{
    auto result(std::make_pair(nullptr, false));

    return result;
}

template <typename VertexType, typename EdgeType, typename VisitorType>
void graph<VertexType, EdgeType, VisitorType>::build_adjacency_list(typename graph<VertexType, EdgeType, VisitorType>::adjacency_list_type& adjacency_list)
{
    adjacency_list.clear();
}

END_NAMESPACE

#endif // __GRAPH_H__
