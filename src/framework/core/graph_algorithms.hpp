//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __GRAPH_ALGORITHMS_H__
#define __GRAPH_ALGORITHMS_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TVertex, typename TEdgeValue = double, std::size_t N = GraphConnectivityRank>
class graph_algorithms : private noncopyable
{
    public:
        using vertex_type = std::shared_ptr<TVertex>;
        using vertices_type = std::set<vertex_type, vertex_lt_key_comparator<vertex>>;

        using graph_type = std::shared_ptr<graph<TVertex, TEdgeValue, N>>;

        //using dominator_tree_type = std::shared_ptr<tree>;

    public:
        static void set_to_vector(const graph_type& graph, std::vector<vertex_type>& result);
        static void dfs_to_vector(const graph_type& graph, std::vector<vertex_type>& result);

        static void compute_dominators(graph_type& graph);
};

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::set_to_vector(const typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph,
                                                             std::vector<typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type>& result)
{
    std::vector<vertex_type> vertices;

    vertices.reserve((*graph).vertices().size());

    for(const auto& vertex : (*graph).vertices())
    {
        vertices.emplace_back(vertex);
    }

    result.swap(vertices);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::dfs_to_vector(const typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph,
                                                             std::vector<typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type>& result)
{
    std::vector<vertex_type> vertices;

    vertices.reserve((*graph).vertices().size());

    std::stack<vertex_type> stack;

    stack.push((*graph).root());

    (*(*graph).root()).flags() |= vertex::flag::visited;

    while(!stack.empty())
    {
        auto vertex(stack.top());
        stack.pop();

        if(((*vertex).flags() & vertex::flag::visited) != vertex::flag::visited)
        {
            continue;
        }

        vertices.emplace_back(std::dynamic_pointer_cast<vertex_type::element_type>(vertex));

        for(auto& adjacent : (*vertex).adjacencies())
        {
            if(((*adjacent).flags() & vertex::flag::visited) != vertex::flag::visited)
            {
                stack.push(std::dynamic_pointer_cast<vertex_type::element_type>(adjacent));
                (*adjacent).flags() |= vertex::flag::visited;
            }
        }
    }

    result.swap(vertices);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::compute_dominators(typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph)
{
    if((*graph).digraph())
    {
        std::vector<std::shared_ptr<dominator_vertex>> vertices;

        graph_algorithms<dominator_vertex>::dfs_to_vector(graph, vertices);



                constexpr auto s0 = calculate_alignment<uint32_t>(30, 16);
                constexpr auto s1 = calculate_alignment(30, 16);
                constexpr auto s2 = calculate_alignment(3, 16);
                constexpr auto s3 = calculate_alignment(24, 8);
    }
}

END_NAMESPACE

#endif // __GRAPH_ALGORITHMS_H__
