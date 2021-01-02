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
        static void set_to_vector(const vertices_type& vertices_set, std::vector<vertex_type>& vertices_vector);
        static void dfs_to_vector(const vertex_type& root, const vertices_type& vertices_set, std::vector<vertex_type>& vertices_vector);

        static void compute_dominators(graph_type& graph);
};

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::set_to_vector(const typename graph_algorithms<TVertex, TEdgeValue, N>::vertices_type& vertices_set,
                                                             std::vector<typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type>& vertices_vector)
{
    std::vector<vertex_type> result;

    result.reserve(vertices_set.size());

    for(const auto& vertex : vertices_set)
    {
        result.emplace_back(vertex);
    }

    vertices_vector.swap(result);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::dfs_to_vector(const vertex_type& root,
                                                             const typename graph_algorithms<TVertex, TEdgeValue, N>::vertices_type& vertices_set,
                                                             std::vector<typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type>& vertices_vector)
{
    std::vector<vertex_type> result;
    result.reserve(vertices_set.size());

    std::stack<vertex_type> stack;

    result.emplace_back(root);
    stack.push(root);
    (*root).flags() |= vertex::flag::visited;

    while(!stack.empty())
    {
        auto vertex(stack.top());
        stack.pop();

        for(auto& adjacent : (*vertex).adjacencies())
        {
            if(((*adjacent).flags() & vertex::flag::visited) != vertex::flag::visited)
            {
                result.emplace_back(std::dynamic_pointer_cast<vertex_type::element_type>(adjacent));
                stack.push(std::dynamic_pointer_cast<vertex_type::element_type>(adjacent));
                (*adjacent).flags() |= vertex::flag::visited;
            }
        }
    }

    vertices_vector.swap(result);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::compute_dominators(typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph)
{
    if((*graph).digraph())
    {
                constexpr auto s0 = calculate_alignment<uint32_t>(30, 16);
                constexpr auto s1 = calculate_alignment(30, 16);
                constexpr auto s2 = calculate_alignment(3, 16);
                constexpr auto s3 = calculate_alignment(24, 8);
    }
}

END_NAMESPACE

#endif // __GRAPH_ALGORITHMS_H__
