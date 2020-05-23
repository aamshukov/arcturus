//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_VERTEX_H__
#define __GRAPH_VERTEX_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename VertexValueType>
struct vertex : private noncopyable, public visitable<vertex<VertexValueType>>
{
    using vertex_type = vertex<VertexValueType>;
    using vertices_type = std::list<std::weak_ptr<vertex>>;

    using vertex_value_type = VertexValueType;

    using visitor_type = visitor<vertex_type>;

    using index_type = int;

    index_type id = 0;
    vertex_value_type value = vertex_value_type();
    vertices_type adjacencies;

    void accept(visitor_type& visitor) override;
};

template <typename VertexValueType>
void vertex<VertexValueType>::accept(typename vertex<VertexValueType>::visitor_type& visitor)
{
    visitor.visit(static_cast<vertex<VertexValueType>&>(*this));
}

END_NAMESPACE

#endif // __GRAPH_VERTEX_H__
