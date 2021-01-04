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
        static void compute_idominators(graph_type& graph);

        static void generate_graphviz_file(const graph_type& graph, const string_type& file_name, bool show_values = true);
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
    // https://stackoverflow.com/questions/9201166/iterative-dfs-vs-recursive-dfs-and-different-elements-order
    //  1. In the iterative approach: You first insert all the elements into the stack - and then handle the head of the stack
    //     [which is the last node inserted] - thus the first node you handle is the last child.
    //
    //  2. In the recursive approach: You handle each node when you see it. Thus the first node you handle is the first child.
    //
    //  3. To make the iterative DFS yield the same result as the recursive one - you need to add elements to the stack in reverse order
    //     [for each node, insert its last child first and its first child last]
    std::vector<vertex_type> vertices;

    vertices.reserve((*graph).vertices().size());

    std::stack<vertex_type> stack;

    stack.push((*graph).root());

    while(!stack.empty())
    {
        auto vertex(stack.top());
        stack.pop();

        if(((*vertex).flags() & vertex::flag::visited) == vertex::flag::visited)
        {
            continue;
        }

        (*vertex).flags() |= vertex::flag::visited;

        vertices.emplace_back(std::dynamic_pointer_cast<vertex_type::element_type>(vertex)); // 1 ... faster
        //vertices.emplace(vertices.begin(), std::dynamic_pointer_cast<vertex_type::element_type>(vertex)); // 3 ...

        for(auto& adjacent : (*vertex).adjacencies())
        {
            if(((*adjacent).flags() & vertex::flag::visited) != vertex::flag::visited)
            {
                stack.push(std::dynamic_pointer_cast<vertex_type::element_type>(adjacent));
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

        bitset temp(vertices.size());

        // init


        // iterate
        volatile bool changed = false;

        do
        {
        }
        while(changed);

        // collect results


        vertices_type predecessors;

        //(*graph).collect_predecessors(vertex, predecessors);

        //(*graph).root()



                constexpr auto s0 = calculate_alignment<uint32_t>(30, 16);
                constexpr auto s1 = calculate_alignment(30, 16);
                constexpr auto s2 = calculate_alignment(3, 16);
                constexpr auto s3 = calculate_alignment(24, 8);
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::compute_idominators(typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph)
{
    if((*graph).digraph())
    {
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::generate_graphviz_file(const typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph,
                                                                      const string_type& file_name,
                                                                      bool show_values)
{
    log_info(L"Generating graphviz file of a graph ...");

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate graphviz file of a graph: stream is either open or in a bad condition.");
        }

        const char_type* indent(get_indent(4));

        stream << ((*graph).digraph() ? L"digraph" : L"graph") << std::endl;
        stream << L"{" << std::endl;

        for(const auto& vertex : (*graph).vertices())
        {
            stream << indent << (*vertex).id() << " node [shape = circle];" << std::endl;
        }

        for(const auto& edge : (*graph).edges())
        {
            const auto& vertex_u((*edge).endpoints()[0]);
            const auto& vertex_v((*edge).endpoints()[1]);

            stream << indent << (*vertex_u).id() << " -> " << (*vertex_v).id() << ";" << std::endl;

            if(show_values)
            {
                stream << indent << " [ " << "label = \"" << (*edge).value() << "\" ];" << std::endl;
            }
        }

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate graphviz file of a graph: error occurred.");
    }

    log_info(L"Generated graphviz file of a graph.");

    // D:\Soft\graphviz\2.38\release\bin\dot -Tpng d:\tmp\cfg.dot -o d:\tmp\cfg.png
}

END_NAMESPACE

#endif // __GRAPH_ALGORITHMS_H__
