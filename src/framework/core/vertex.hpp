//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
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
        result ^= std::hash<std::size_t>{}(result + 0x9E3779B9 + (result << 6) + (result >> 2)); // aka boost hash_combine
        return result;
    }
};

class vertex : public visitable
{
    public:
        using vertex_type = std::shared_ptr<vertex>;
        using vertices_type = std::set<vertex_type, vertex_lt_key_comparator<vertex>>;
        using vertices_iterator_type = typename vertices_type::iterator;

        using id_type = std::size_t;

        enum class flag : uint64_t
        {
            clear   = 0x0000,
            visited = 0x0001
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

    protected:
        id_type                 my_id;
        vertices_type           my_adjacencies;

        flags_type              my_flags;

    public:
                                vertex(const id_type& id);
        virtual                ~vertex();

        const id_type&          id() const;
        id_type&                id();

        const vertices_type&    adjacencies() const;
        vertices_type&          adjacencies();

        const flags_type&       flags() const;
        flags_type&             flags();

        ACCEPT_METHOD;
};

inline vertex::vertex(const typename vertex::id_type& id = 0)
             : my_id(id), my_flags(vertex::flags_type::clear)
{
}

inline vertex::~vertex()
{
}

inline const typename vertex::id_type& vertex::id() const
{
    return my_id;
}

inline typename vertex::id_type& vertex::id()
{
    return my_id;
}

inline const typename vertex::vertices_type& vertex::adjacencies() const
{
    return my_adjacencies;
}

inline typename vertex::vertices_type& vertex::adjacencies()
{
    return my_adjacencies;
}

inline const typename vertex::flags_type& vertex::flags() const
{
    return my_flags;
}

inline typename vertex::flags_type& vertex::flags()

{
    return my_flags;
}

END_NAMESPACE

#endif // __GRAPH_VERTEX_H__
