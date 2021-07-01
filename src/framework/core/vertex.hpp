//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_VERTEX_H__
#define __GRAPH_VERTEX_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TVertex>
struct vertex_lt_key_comparator
{
    using vertex_type = std::shared_ptr<TVertex>;
    bool operator() (const vertex_type& lhs, const vertex_type& rhs) const
    {
        return (*lhs).id() < (*rhs).id();
    }
};

template <typename TVertex>
struct vertex_eq_key_comparator
{
    using vertex_type = std::shared_ptr<TVertex>;
    bool operator() (const vertex_type& lhs, const vertex_type& rhs) const
    {
        return (*lhs).id() == (*rhs).id();
    }
};

template <typename TVertex>
struct vertex_hash
{
    using vertex_type = std::shared_ptr<TVertex>;
    std::size_t operator () (const vertex_type& vertex) const
    {
        std::size_t result = (*vertex).id();
        result ^= combine_hash(result);
        return result;
    }
};

template <typename TValue = std::size_t>
class vertex : public visitable
{
    public:
        using vertex_type = std::shared_ptr<vertex>;
        using vertices_type = std::set<vertex_type, vertex_lt_key_comparator<vertex>>;
        using vertices_iterator_type = typename vertices_type::iterator;

        using value_type = TValue; // vertex value, might be counter

        enum class flag : uint64_t
        {
            clear     = 0x0000,
            visited   = 0x0001,
            processed = 0x0002,
            hasphi    = 0x0004 // related to SSA form
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

        using id_type = std::size_t;

    protected:
        id_type                 my_id;
        string_type             my_label;

        value_type              my_value;

        vertices_type           my_adjacencies;

        flags_type              my_flags;

        std::size_t             my_ref_count; // if the vertex is referenced by another vertices

    public:
                                vertex(const id_type& id = 0, const string_type& label = empty_string());
        virtual                ~vertex();

        const id_type&          id() const;
        id_type&                id();

        const string_type&      label() const;
        string_type&            label();

        const value_type&       value() const;
        value_type&             value();

        const vertices_type&    adjacencies() const;
        vertices_type&          adjacencies();

        const flags_type&       flags() const;
        flags_type&             flags();

        std::size_t             ref_count() const;
        std::size_t             add_ref();
        std::size_t             release();

        ACCEPT_METHOD;
};

template <typename TValue>
inline vertex<TValue>::vertex(const typename vertex<TValue>::id_type& id, const string_type& label)
                     : my_id(id), my_label(label), my_flags(vertex<TValue>::flag::clear), my_ref_count(0)
{
}

template <typename TValue>
inline vertex<TValue>::~vertex()
{
}

template <typename TValue>
inline const typename vertex<TValue>::id_type& vertex<TValue>::id() const
{
    return my_id;
}

template <typename TValue>
inline typename vertex<TValue>::id_type& vertex<TValue>::id()
{
    return my_id;
}

template <typename TValue>
inline const string_type& vertex<TValue>::label() const
{
    return my_label;
}

template <typename TValue>
inline string_type& vertex<TValue>::label()
{
    return my_label;
}

template <typename TValue>
inline const typename vertex<TValue>::value_type& vertex<TValue>::value() const
{
    return my_value;
}

template <typename TValue>
inline typename vertex<TValue>::value_type& vertex<TValue>::value()
{
    return my_value;
}

template <typename TValue>
inline const typename vertex<TValue>::vertices_type& vertex<TValue>::adjacencies() const
{
    return my_adjacencies;
}

template <typename TValue>
inline typename vertex<TValue>::vertices_type& vertex<TValue>::adjacencies()
{
    return my_adjacencies;
}

template <typename TValue>
inline const typename vertex<TValue>::flags_type& vertex<TValue>::flags() const
{
    return my_flags;
}

template <typename TValue>
inline typename vertex<TValue>::flags_type& vertex<TValue>::flags()

{
    return my_flags;
}

template <typename TValue>
inline std::size_t vertex<TValue>::ref_count() const
{
    return my_ref_count;
}

template <typename TValue>
inline std::size_t vertex<TValue>::add_ref()
{
    return ++my_ref_count;
}

template <typename TValue>
inline std::size_t vertex<TValue>::release()
{
    if(my_ref_count > 0)
    {
        my_ref_count--;
    }
    else
    {
        // ... something wrong
    }

    return my_ref_count;
}

END_NAMESPACE

#endif // __GRAPH_VERTEX_H__
