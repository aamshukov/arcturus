//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __GRAPH_EDGE_H__
#define __GRAPH_EDGE_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TVertex, typename TValue = double, std::size_t N = 2>
class edge : private noncopyable
{
    public:
        using vertex_type = std::shared_ptr<TVertex>;
        using vertices_type = vertex_type[N]; // usually has two vertices and more vertices in hyper-graphs

        using edge_type = std::shared_ptr<edge<TVertex, TValue, N>>;

        struct edge_lt_key_comparator
        {
            bool operator() (const edge_type& lhs, const edge_type& rhs) const
            {
                return (*lhs).id() < (*rhs).id();
            }
        };

        struct edge_eq_key_comparator
        {
            bool operator() (const edge_type& lhs, const edge_type& rhs) const
            {
                return (*lhs).id() == (*rhs).id();
            }
        };

        using value_type = TValue; // edge value, might be weight

        using id_type = std::size_t;

    private:
        id_type                 my_id;
        value_type              my_value;

        vertices_type           my_endpoints;

    public:
                                edge(const id_type& id = 0);
        virtual                ~edge();

        const id_type&          id() const;
        id_type&                id();

        const value_type&       value() const;
        value_type&             value();

        const vertices_type&    endpoints() const;
        vertices_type&          endpoints();
};

template <typename TVertex, typename TValue, std::size_t N>
inline edge<TVertex, TValue, N>::edge(const typename edge::id_type& id)
                               : my_id(id)
{
}

template <typename TVertex, typename TValue, std::size_t N>
inline edge<TVertex, TValue, N>::~edge()
{
}

template <typename TVertex, typename TValue, std::size_t N>
inline const typename edge<TVertex, TValue, N>::id_type& edge<TVertex, TValue, N>::id() const
{
    return my_id;
}

template <typename TVertex, typename TValue, std::size_t N>
inline typename edge<TVertex, TValue, N>::id_type& edge<TVertex, TValue, N>::id()
{
    return my_id;
}

template <typename TVertex, typename TValue, std::size_t N>
const typename edge<TVertex, TValue, N>::value_type& edge<TVertex, TValue, N>::value() const
{
    return my_value;
}

template <typename TVertex, typename TValue, std::size_t N>
typename edge<TVertex, TValue, N>::value_type& edge<TVertex, TValue, N>::value()
{
    return my_value;
}

template <typename TVertex, typename TValue, std::size_t N>
const typename edge<TVertex, TValue, N>::vertices_type& edge<TVertex, TValue, N>::endpoints() const
{
    return my_endpoints;
}

template <typename TVertex, typename TValue, std::size_t N>
typename edge<TVertex, TValue, N>::vertices_type& edge<TVertex, TValue, N>::endpoints()
{
    return my_endpoints;
}

END_NAMESPACE

#endif // __GRAPH_EDGE_H__
