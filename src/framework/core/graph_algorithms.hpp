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

        using dominance_tree_type = std::shared_ptr<dominance_tree>;

        using number_vertex_type = std::unordered_map<size_type, vertex_type>;
        using vertex_number_type = std::unordered_map<vertex_type, size_type, vertex_hash<dominator_vertex>, vertex_eq_key_comparator<dominator_vertex>>;
        using vertex_vertex_type = std::unordered_map<vertex_type, vertex_type, vertex_hash<dominator_vertex>, vertex_eq_key_comparator<dominator_vertex>>;
        using vertex_vertices_type = std::unordered_map<vertex_type, vertices_type, vertex_hash<dominator_vertex>, vertex_eq_key_comparator<dominator_vertex>>;

    private:
        struct lengauer_tarjan_context
        {
            vertex_number_type semidominators;  // holds semidominator numbers of vertices after computation

            number_vertex_type vertices;
            vertex_vertex_type parents;         // parent vertices

            vertex_vertices_type predecessors;  // predecessors of a vertex

            vertex_vertex_type ancestors;       // required to represent forest
            vertex_vertex_type labels;          // required to represent forest

            vertex_vertices_type buckets;

            size_type n;
        };

        using lengauer_tarjan_context_type = lengauer_tarjan_context;

    private:
        static void         dfs_lengauer_tarjan(const vertex_type& v, lengauer_tarjan_context_type& context);
        static void         compress_lengauer_tarjan(const vertex_type& v, lengauer_tarjan_context_type& context);
        static vertex_type  eval_lengauer_tarjan(const vertex_type& v, lengauer_tarjan_context_type& context);
        static void         link_lengauer_tarjan(const vertex_type& v, const vertex_type& w, lengauer_tarjan_context_type& context);

    public:
        static void         flat_to_vector(const graph_type& graph, std::vector<vertex_type>& result_vector);

        static void         dfs_preorder_to_vector(const graph_type& graph, std::vector<vertex_type>& result_vector);
        static void         dfs_postorder_to_vector(const dominance_tree_type& tree, std::vector<dominance_tree_type>& result_vector);

        static void         compute_dominators(graph_type& graph);
        static void         compute_dominators_lengauer_tarjan(graph_type& graph);

        static void         build_dominance_tree(const graph_type& graph, dominance_tree_type& result_tree);
        static void         compute_dominance_frontiers(graph_type& graph, const dominance_tree_type& tree);

        static void         generate_graphviz_file(const graph_type& graph, const string_type& file_name, bool show_values = true);
        static void         generate_graphviz_file(const dominance_tree_type& tree, const string_type& file_name);
};

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::flat_to_vector(const graph_type& graph, std::vector<vertex_type>& result_vector)
{
    std::vector<vertex_type> vertices;

    vertices.reserve(*(*graph).vertices().size());

    if((*graph).root() != nullptr)
    {
        vertices[0] = (*graph).root();
    }

    for(const auto& vertex : (*graph).vertices())
    {
        if(vertex == (*graph).root())
            continue;

        vertices.emplace_back(vertex);
    }

    result_vector.swap(vertices);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::dfs_preorder_to_vector(const typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph,
                                                                      std::vector<typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type>& result_vector)
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

        for(const auto& adjacent : (*vertex).adjacencies())
        {
            if(((*adjacent).flags() & vertex::flag::visited) != vertex::flag::visited)
            {
                stack.push(std::dynamic_pointer_cast<vertex_type::element_type>(adjacent));
            }
        }
    }

    result_vector.swap(vertices);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::dfs_postorder_to_vector(const typename graph_algorithms<TVertex, TEdgeValue, N>::dominance_tree_type& tree,
                                                                       std::vector<typename graph_algorithms<TVertex, TEdgeValue, N>::dominance_tree_type>& result_vector)
{
    std::vector<dominance_tree_type> nodes;

    std::stack<dominance_tree_type> stack1; // holds collected nodes
    std::stack<dominance_tree_type> stack2; // working stack

    stack2.push(tree);

    while(!stack2.empty())
    {
        auto node(stack2.top());
        stack2.pop();

        stack1.push(node);

        for(const auto& kid : (*node).kids())
        {
            stack2.push(std::dynamic_pointer_cast<dominance_tree_type::element_type>(kid));
        }
    }

    while(!stack1.empty())
    {
        auto node(stack1.top());
        stack1.pop();

        nodes.emplace_back(node);
    }

    result_vector.swap(nodes);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::compute_dominators(typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph)
{
    // 'Advanced Compiler Design and Implementation' by Steven S.Muchnick
    if(!(*graph).vertices().empty() && (*graph).digraph())
    {
        // phase I (compute dominators)
        // flat graph as DFS vector to use bitsets, root is the first element
        std::vector<std::shared_ptr<dominator_vertex>> vertices;

        dfs_preorder_to_vector(graph, vertices);

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

        // phase III (collect dominators, results)
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
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::dfs_lengauer_tarjan(const typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type& v,
                                                                   typename graph_algorithms<TVertex, TEdgeValue, N>::lengauer_tarjan_context_type& context)
{
    context.semidominators[v] = context.n;

    context.vertices[context.n] = v;
    context.labels[v] = v;

    context.ancestors[v] = 0;

    context.n++;

    for(const auto& adjacence : (*v).adjacencies())
    {
        auto w = std::dynamic_pointer_cast<dominator_vertex>(adjacence);

        if(context.semidominators[w] == size_type(-1))
        {
            context.parents[w] = v;
            
            dfs_lengauer_tarjan(w, context);
        }

        context.predecessors[w].emplace(v);
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph_algorithms<TVertex, TEdgeValue, N>::compress_lengauer_tarjan(const typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type& v,
                                                                               typename graph_algorithms<TVertex, TEdgeValue, N>::lengauer_tarjan_context_type& context)
{
    if(context.ancestors[context.ancestors[v]] != nullptr)
    {
        compress_lengauer_tarjan(context.ancestors[v], context);

        if(context.semidominators[context.labels[context.ancestors[v]]] < context.semidominators[context.labels[v]])
        {
            context.labels[v] = context.labels[context.ancestors[v]];
        }

        context.ancestors[v] = context.ancestors[context.ancestors[v]];
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type graph_algorithms<TVertex, TEdgeValue, N>::eval_lengauer_tarjan(const typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type& v,
                                                                                                                                     typename graph_algorithms<TVertex, TEdgeValue, N>::lengauer_tarjan_context_type& context)
{
    vertex_type result;

    if(context.ancestors[v] == nullptr)
    {
        result = v;
    }
    else
    {
        compress_lengauer_tarjan(v, context);

        result = context.labels[v];
    }

    return result;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
inline void graph_algorithms<TVertex, TEdgeValue, N>::link_lengauer_tarjan(const typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type& v,
                                                                           const typename graph_algorithms<TVertex, TEdgeValue, N>::vertex_type& w,
                                                                           typename graph_algorithms<TVertex, TEdgeValue, N>::lengauer_tarjan_context_type& context)
{
    context.ancestors[w] = v;
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::compute_dominators_lengauer_tarjan(typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph)
{
    // 'A fast algorithm for finding dominators in a flowgraph' by Lengauer and Tarjan (July 1979)
    if(!(*graph).vertices().empty() && (*graph).digraph())
    {
        auto vertices_size = ((*graph).vertices()).size();

        lengauer_tarjan_context context;

        context.semidominators.reserve(vertices_size);

        context.vertices.reserve(vertices_size);
        context.parents.reserve(vertices_size);

        context.predecessors.reserve(vertices_size);

        context.ancestors.reserve(vertices_size);
        context.labels.reserve(vertices_size);

        // phase I (step 1 - initialize, DFS numbering)
        for(const auto& v : (*graph).vertices())
        {
            context.semidominators[v] = size_type(-1);
        }

        context.n = 0;

        dfs_lengauer_tarjan((*graph).root(), context);

        // phase II (step 2 and 3 - build forest)
        for(auto k = context.n - 1; k > 0; k--)
        {
            auto w = context.vertices[k];

            for(const auto& v : context.predecessors[w])
            {
                auto u = eval_lengauer_tarjan(v, context);

                if(context.semidominators[u] < context.semidominators[w])
                {
                    context.semidominators[w] = context.semidominators[u];
                }
            }

            context.buckets[context.vertices[context.semidominators[w]]].emplace(w);

            link_lengauer_tarjan(context.parents[w], w, context);

            const auto& parent_w(context.parents[w]);
            const auto& bucket(context.buckets[parent_w]);

            for(auto it = bucket.begin(); it != bucket.end();)
            {
                auto v = *it;

                context.buckets[parent_w].erase(it++);

                auto u = eval_lengauer_tarjan(v, context);

                if(context.semidominators[u] < context.semidominators[v])
                {
                    (*v).idominator() = u;
                }
                else
                {
                    (*v).idominator() = context.parents[w];
                }
            }
        }

        // phase III (step 4 - collect dominators, results)
        for(auto k = 1; k < context.n; k++)
        {
            auto w = context.vertices[k];

            if((*w).idominator() != context.vertices[context.semidominators[w]])
            {
                auto w_dominator = (*w).idominator();
                (*w).idominator() = (*w_dominator).idominator();
            }
        }

        (*(*graph).root()).idominator().reset();
    }
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::build_dominance_tree(const typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph,
                                                                    typename graph_algorithms<TVertex, TEdgeValue, N>::dominance_tree_type& result_tree)
{
    using vertex_tree_map_type = std::unordered_map<vertex_type,
                                                    dominance_tree_type,
                                                    vertex_hash<dominator_vertex>, vertex_eq_key_comparator<dominator_vertex>>;
    vertex_tree_map_type vertex_tree_map;

    vertex_tree_map.reserve((*graph).vertices().size());

    dominance_tree_type tree(factory::create<dominance_tree>((*graph).root()));

    vertex_tree_map[(*graph).root()] = tree;

    for(const auto& vertex : (*graph).vertices())
    {
        if((*vertex).idominator() != nullptr)
        {
            vertex_tree_map[vertex] = factory::create<dominance_tree>(vertex);
        }
    }

    for(const auto& vertex : (*graph).vertices())
    {
        if((*vertex).idominator() != nullptr)
        {
            auto& papa(vertex_tree_map.find((*vertex).idominator()));
            auto& kid(vertex_tree_map.find(vertex));

            (*(*papa).second).kids().emplace_back((*kid).second);
            (*(*kid).second).papa() = (*papa).second;
        }
    }

    result_tree.swap(tree);
}

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::compute_dominance_frontiers(typename graph_algorithms<TVertex, TEdgeValue, N>::graph_type& graph,
                                                                           const typename graph_algorithms<TVertex, TEdgeValue, N>::dominance_tree_type& tree)
{
    // 'An Efficient Method of Computing SSA' by Ron Cytron, etc.
    std::vector<dominance_tree_type> nodes;

    dfs_postorder_to_vector(tree, nodes);

    // for each X in a bottom-up traversal of the dominator tree do
    for(auto& node : nodes)
    {
        // DF(X) <-- 0
        (*(*node).vertex()).frontiers().clear();
    
        vertices_type successors;

        (*graph).collect_successors((*node).vertex(), successors);

        // for each Y ∈ Succ(X) do
        for(const auto& successor : successors)
        {
            // if idom(Y) != X
            if((*successor).idominator() != (*node).vertex())
            {
                // then DF(X) <-- DF(X) ∪ {Y}
                (*(*node).vertex()).frontiers().emplace(successor); // local
            }
        }

        // for each Z ∈ Children(X)
        for(const auto& kid : (*node).kids())
        {
            auto& frontiers((*(*std::dynamic_pointer_cast<dominance_tree>(kid)).vertex()).frontiers());

            // for each Y ∈ DF(Z) do
            for(const auto& frontier : frontiers)
            {
                // if idom(Y) != X
                if((*frontier).idominator() != (*node).vertex())
                {
                    // then DF(X) <-- DF(X) ∪ {Y}
                    (*(*node).vertex()).frontiers().emplace(frontier); // up
                }
            }
        }
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

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms<TVertex, TEdgeValue, N>::generate_graphviz_file(const typename graph_algorithms<TVertex, TEdgeValue, N>::dominance_tree_type& tree,
                                                                      const string_type& file_name)
{
    log_info(L"Generating graphviz file of a dominance tree ...");

    std::vector<dominance_tree_type> nodes;

    std::stack<dominance_tree_type> stack;

    stack.push(tree);

    while(!stack.empty())
    {
        auto node(stack.top());
        stack.pop();

        if(((*node).flags() & tree::flag::visited) == tree::flag::visited)
        {
            continue;
        }

        (*node).flags() |= tree::flag::visited;

        nodes.emplace(nodes.begin(), node);

        for(const auto& kid : (*node).kids())
        {
            if(((*kid).flags() & tree::flag::visited) != tree::flag::visited)
            {
                stack.push(std::dynamic_pointer_cast<dominance_tree_type::element_type>(kid));
            }
        }
    }

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate graphviz file of a dominance tree: stream is either open or in a bad condition.");
        }

        const char_type* indent(get_indent(4));

        stream << L"digraph" << std::endl;
        stream << L"{" << std::endl;

        for(const auto& node : nodes)
        {
            stream << indent << ((*(*node).vertex()).label().empty() ? std::to_wstring((*(*node).vertex()).id()) : (*(*node).vertex()).label())
                             << L" node [shape = circle];" << std::endl;
        }

        for(const auto& node : nodes)
        {
            for(const auto& kid0 : (*node).kids())
            {
                auto kid(std::dynamic_pointer_cast<dominance_tree_type::element_type>(kid0));

                stream << indent
                       << ((*(*node).vertex()).label().empty() ? std::to_wstring((*(*node).vertex()).id()) : (*(*node).vertex()).label())
                       << L" -> "
                       << ((*(*kid).vertex()).label().empty() ? std::to_wstring((*(*kid).vertex()).id()) : (*(*kid).vertex()).label())
                       << L";"
                       << std::endl;
            }
        }

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate graphviz file of a dominance tree: error occurred.");
    }

    log_info(L"Generated graphviz file of a dominance tree.");
}

END_NAMESPACE

#endif // __GRAPH_ALGORITHMS_H__
