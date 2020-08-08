//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_EDGE_H__
#define __GRAPH_EDGE_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TVertex, typename TValue = double, std::size_t N = 2>
struct edge : private noncopyable
{
    using vertex_type = std::shared_ptr<TVertex>;
    using value_type = TValue; // edge value, might be weight

    using index_type = int64_t;

    index_type id = 0;
    value_type value = value_type();

    vertex_type endpoints[N]; // usually has two vertices and more vertices in hyper-graphs
};

END_NAMESPACE

#endif // __GRAPH_EDGE_H__
