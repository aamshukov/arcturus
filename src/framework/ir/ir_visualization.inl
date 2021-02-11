//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __IR_VISUALIZATION_INL__
#define __IR_VISUALIZATION_INL__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename TreeTraits>
void ir_visualization<TreeTraits>::print_tree(const typename ir_visualization<TreeTraits>::parse_tree_type& tree, std::size_t level, std::wostream& stream)
{
    for(std::size_t i = 0; i < level; i++)
    {
        stream << "    ";
    }

    if(static_cast<uint32_t>((*(*tree).ir_symbol).token().type) != 0)
        stream << (*(*tree).gr_symbol).name() << L":" << static_cast<uint32_t>((*(*tree).ir_symbol).token().type) << std::endl;
    else
        stream << (*(*tree).gr_symbol).name() << std::endl;

    for(const auto& kid : (*tree).kids)
    {
        print_tree(std::dynamic_pointer_cast<parse_tree<tree_traits_type>>(kid), level + 1, stream);
    }

    stream << std::endl;
}

template <typename TreeTraits>
void ir_visualization<TreeTraits>::print_trees(const typename ir_visualization<TreeTraits>::parse_trees_type& trees, std::wostream& stream)
{
    stream << std::endl;

    for(auto [k, tree] : enumerate(trees))
    {
        stream << L"[" << k << L"]:" << std::endl;
        print_tree(tree, 0, stream);
    }

    stream << std::endl;
}

template <typename TreeTraits>
void ir_visualization<TreeTraits>::collect_tree_dot_labels(const typename ir_visualization<TreeTraits>::parse_tree_type& root, std::wostream& stream)
{
    std::size_t k = 1;

    std::queue<parse_tree_type> queue;

    queue.emplace(root);

    while(!queue.empty())
    {
        auto entry(queue.front());

        queue.pop();

        stream << L"    " << k++ << L" [label=\"" << (*(*entry).gr_symbol).name() << L"\"];" << std::endl;

        for(auto kid : (*entry).kids)
        {
            queue.emplace(std::dynamic_pointer_cast<parse_tree<tree_traits_type>>(kid));
        }
    }

    stream << std::endl;
}

template <typename TreeTraits>
void ir_visualization<TreeTraits>::build_tree_dot_graph(const typename ir_visualization<TreeTraits>::parse_tree_type& tree, std::wostream& stream)
{
    struct queue_entry
    {
        parse_tree_type node;
        std::size_t papa;
    };

    std::size_t k = 1;

    std::queue<queue_entry> queue;

    queue.emplace(queue_entry{ tree, k });

    while(!queue.empty())
    {
        auto entry(queue.front());

        queue.pop();

        for(auto kid : (*entry.node).kids)
        {
            stream << L"    " << entry.papa << L" -> " << ++k << L";" << std::endl;
            queue.emplace(queue_entry{ std::dynamic_pointer_cast<parse_tree<tree_traits_type>>(kid), k });
        }
    }
}

template <typename TreeTraits>
void ir_visualization<TreeTraits>::collect_dag_dot_labels(const typename ir_visualization<TreeTraits>::parse_dag_type& root, std::wostream& stream)
{
    std::queue<parse_dag_type> queue;

    queue.emplace(root);

    while(!queue.empty())
    {
        auto dag(queue.front());

        queue.pop();

        stream << L"    " << (*dag).id << L" [label=\"" << (*(*dag).gr_symbol).name() << L"  " << (*dag).papas.size() <<  "\"];" << std::endl;

        for(auto kid : (*dag).kids)
        {
            auto dag_kid(std::dynamic_pointer_cast<parse_dag<tree_traits_type>>(kid));
            queue.emplace(dag_kid);
        }
    }

    stream << std::endl;
}

template <typename TreeTraits>
void ir_visualization<TreeTraits>::build_dag_dot_graph(const typename ir_visualization<TreeTraits>::parse_dag_type& dag, std::wostream& stream)
{
    std::queue<parse_dag_type> queue;

    queue.emplace(dag);

    while(!queue.empty())
    {
        auto node(queue.front());

        queue.pop();

        if(((*node).flags & dag::flags_type::processed) == dag::flags_type::processed)
        {
            continue;
        }

        (*node).flags |= dag::flags_type::processed;

        for(auto kid : (*node).kids)
        {
            for(auto papa : (*kid).papas)
            {
                if(papa == node)
                {
                    auto dag_kid(std::dynamic_pointer_cast<parse_dag<tree_traits_type>>(kid));
                    stream << L"    " << (*node).id << L" -> " << (*dag_kid).id << L";" << std::endl;
                }
            }

            queue.emplace(std::dynamic_pointer_cast<parse_dag<tree_traits_type>>(kid));
        }
    }
}

template <typename TreeTraits>
void ir_visualization<TreeTraits>::decorate_tree(const typename ir_visualization<TreeTraits>::parse_tree_type& tree, const string_type& dot_file_name, std::size_t k)
{
    string_type file_name(dot_file_name + std::to_wstring(k) + L".etree.dot");

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate graphviz file %s: stream is either open or in a bad condition.", file_name.c_str());
        }

        stream << L"digraph EarleyTree" << std::endl;
        stream << L"{" << std::endl;

        collect_tree_dot_labels(tree, stream);
        build_tree_dot_graph(tree, stream);

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate graphviz file %s: error occurred.", file_name.c_str());
    }
}

template <typename TreeTraits>
void ir_visualization<TreeTraits>::decorate_trees(const typename ir_visualization<TreeTraits>::parse_trees_type& trees, const string_type& dot_file_name)
{
    // generate Graphviz dot file ...
    // for %i in (d:\tmp\fsa\*.dot) do D:\Soft\graphviz\2.38\release\bin\dot -Tpng %i -o %i.png
    for(auto [k, tree] : enumerate(trees))
    {
        decorate_tree(tree, dot_file_name, k);
    }
}

template <typename TreeTraits>
void ir_visualization<TreeTraits>::decorate_dag(const typename ir_visualization<TreeTraits>::parse_dag_type& dag, const string_type& dot_file_name)
{
    string_type file_name(dot_file_name + L".dag.dot");

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate graphviz file %s: stream is either open or in a bad condition.", file_name.c_str());
        }

        stream << L"digraph DAG" << std::endl;
        stream << L"{" << std::endl;

        collect_dag_dot_labels(dag, stream);
        build_dag_dot_graph(dag, stream);

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate graphviz file %s: error occurred.", file_name.c_str());
    }
}

END_NAMESPACE

#endif // __IR_VISUALIZATION_INL__
