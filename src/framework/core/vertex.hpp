//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_VERTEX_H__
#define __GRAPH_VERTEX_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TValue>
struct vertex : public visitable
{
    using value_type = TValue; // vertex value type

    using vertex_type = vertex<value_type>;
    using vertices_type = std::list<std::weak_ptr<vertex>>;

    using index_type = int64_t;

    index_type id = 0;
    value_type value = value_type();

    vertices_type adjacencies;

    template <typename TVisitor> // deficiency of C++ ... template methods cannot be virtual ...
    typename TVisitor::return_type accept(TVisitor& visitor, const typename TVisitor::param_type& param)
    {
        if constexpr (std::is_void_v<typename TVisitor::return_type>)
        {
            if constexpr (std::is_void_v<typename TVisitor::param_type>)
            {
                visitor.visit(*this);
            }
            else
            {
                visitor.visit(*this, param);
            }
        }
        else
        {
            if constexpr (std::is_void_v<typename TVisitor::param_type>)
            {
                return visitor.visit(*this);
            }
            else
            {
                return visitor.visit(*this, param);
            }
        }
    }
};

END_NAMESPACE

#endif // __GRAPH_VERTEX_H__
