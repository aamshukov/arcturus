//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __GRAPH_ALGORITHMS_H__
#define __GRAPH_ALGORITHMS_H__

#pragma once

BEGIN_NAMESPACE(core)

class graph_algorithms : private noncopyable
{
        //using dominator_tree_type = std::shared_ptr<tree>;

    public:
        template <typename TVertex, typename TEdgeValue, std::size_t N>
        static void compute_dominators(std::shared_ptr<graph<TVertex, TEdgeValue, N>>& graph);
};

template <typename TVertex, typename TEdgeValue, std::size_t N>
void graph_algorithms::compute_dominators(std::shared_ptr<graph<TVertex, TEdgeValue, N>>& graph)
{
    if((*graph).digraph())
    {
        //dominator_tree_type dom_tree;

        //dominator_tree.swap(dom_tree);
    }
}

END_NAMESPACE

#endif // __GRAPH_ALGORITHMS_H__
