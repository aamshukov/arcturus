//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __GRAPH_ALGORITHMS_H__
#define __GRAPH_ALGORITHMS_H__

#pragma once

BEGIN_NAMESPACE(core)

// α β ε λ ∅ ∈ Σ ∪ ⋂ δ γ
template <typename TVertex, typename TEdgeValue = double, std::size_t N = GraphConnectivityRank>
class graph_algorithms : private noncopyable
{
    public:
        using vertex_type = std::shared_ptr<TVertex>;
        using vertices_type = std::set<vertex_type, vertex_lt_key_comparator<vertex>>;

        using graph_type = std::shared_ptr<graph<TVertex, TEdgeValue, N>>;
        using bitset_type = bitset<uint64_t>;

    public:
        static void dfs_to_vector(const graph_type& graph, std::vector<vertex_type>& result);

        static void compute_dominators(graph_type& graph);

        static void generate_graphviz_file(const graph_type& graph, const string_type& file_name, bool show_values = true);
};

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
    // 'Advanced Compiler Design and Implementation' by Steven S.Muchnick
    if((*graph).digraph())
    {
        // phase I (compute dominators)
        // flat graph as DFS vector to use bitsets, root is the first element
        std::vector<std::shared_ptr<dominator_vertex>> vertices;

        graph_algorithms<dominator_vertex>::dfs_to_vector(graph, vertices);

        auto vertices_size = vertices.size();

        // initialize root vertex with itself
        // Domin(r) := {r} 
        (*(*graph).root()).bitset().create(vertices_size);
        (*(*graph).root()).bitset()[0] = 1;

        // initialize each vertex with all vertices as dominators excluding root
        // for each n ∈ N - {r} do
        for(size_type k = 1; k < vertices_size; k++) // k = 1 - except root
        {
            auto& n(vertices[k]);

            (*n).bitset().create(vertices_size);

            // Domin(n) := N
            (*n).bitset().set(); // all bits - all vertices
        }

        // iterate
        bitset t(vertices_size); // T

        volatile bool changed;

        do
        {
            changed = false;

            // for each n ∈ N - {r} do
            for(size_type k = 1; k < vertices_size; k++) // k = 1 - except root
            {
                // T := N
                t.set();

                auto& n(vertices[k]);

                vertices_type predecessors;

                (*graph).collect_predecessors(n, predecessors);

                // for each p ∈ Pred(n) do
                for(auto& p : predecessors)
                {
                    // T ⋂= Domin(p)
                    t &= (*p).bitset();
                }

                // D := {n} ∪ T
                t[k] = 1;

                // if D != Domin(n) then
                if(t != (*n).bitset())
                {
                    // Domin(n) := D
                    (*n).bitset() = t;
                    changed = true;
                }
            }
        }
        while(changed);

        // phase II (compute immediate dominators)
        std::vector<bitset_type> tmp(vertices_size, bitset_type()); // Tmp

        // for each n ∈ N do
        for(size_type k = 0; k < vertices_size; k++)
        {
            auto& n(vertices[k]);

            // Tmp(n) := Domin(n) - {n}
            tmp[k] = (*n).bitset();
            tmp[k][k] = 0; // ... - {n}
        }

        // for each n ∈ N - {r} do
        for(size_type n = 1; n < vertices_size; n++) // k = 1 - except root
        {
            auto& tmp_n(tmp[n]);

            // for each s ∈ Tmp(n) do
            auto s = tmp_n.find_first();

            while(s != bitset_type::npos)
            {
                // for each t ∈ Tmp(n) - {s} do
                auto t = tmp_n.find_first();

                while(t != bitset_type::npos)
                {
                    if(t != s) // ... - {s}
                    {
                        auto& tmp_s(tmp[s]);

                        // if t ∈ Tmp(s) then
                        if(tmp_s[t])
                        {
                            // Tmp(n) -= {t}
                            tmp_n[t] = 0;
                        }
                    }

                    t = tmp_n.find_next(t);
                }

                s = tmp_n.find_next(s);
            }
        }

        // for each n ∈ N - {r} do
        for(size_type n = 1; n < vertices_size; n++) // k = 1 - except root
        {
            auto& idom(vertices[n]);
            auto& tmp_n(tmp[n]);

            // Idom(n) := Tmp(n)
            auto position = tmp_n.find_first();

            if(position != bitset_type::npos) // either one idominator or nothing
            {
                (*idom).idominator() = vertices[position];
            }
        }

        // pahase III (collect dominators, results)
        for(auto& vertex : vertices)
        {
            const auto& dominators((*vertex).bitset());

            auto position = dominators.find_first();

            while(position != bitset_type::npos)
            {
                (*vertex).dominators().emplace(vertices[position]);

                position = dominators.find_next(position);
            }
        }

        //??
        std::map<string_type, std::vector<string_type>> doms;
        std::map<string_type, string_type> idoms;
        for(auto& vertex : vertices)
        {
            std::vector<string_type> dom;
            for(auto& d : (*vertex).dominators())
                dom.push_back((*d).label());
            doms[(*vertex).label()] = dom;
            if((*vertex).idominator() != nullptr)
                idoms[(*vertex).label()] = (*(*vertex).idominator()).label();
        }
        doms.clear();

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
            stream << indent << ((*vertex).label().empty() ? std::to_wstring((*vertex).id()) : (*vertex).label()) << L" node [shape = circle];" << std::endl;
        }

        for(const auto& edge : (*graph).edges())
        {
            const auto& vertex_u((*edge).endpoints()[0]);
            const auto& vertex_v((*edge).endpoints()[1]);

            stream << indent << ((*vertex_u).label().empty() ? std::to_wstring((*vertex_u).id()) : (*vertex_u).label()) << L" -> "
                             << ((*vertex_v).label().empty() ? std::to_wstring((*vertex_v).id()) : (*vertex_v).label()) << L";" << std::endl;

            if(show_values)
            {
                stream << indent << L" [ " << L"label = \"" << (*edge).value() << L"\" ];" << std::endl;
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
