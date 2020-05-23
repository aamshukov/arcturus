//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_EDGE_H__
#define __GRAPH_EDGE_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename VertexType, typename EdgeValueType, std::size_t N = 2>
struct edge : private noncopyable
{
    using vertex_type = std::shared_ptr<VertexType>;
    using edge_value_type = EdgeValueType;

    using index_type = int;

    index_type id = 0;
    edge_value_type value = edge_value_type();

    vertex_type endpoints[N]; // usually has two vertices and more vertices in hyper-graphs
};

END_NAMESPACE

#endif // __GRAPH_EDGE_H__
