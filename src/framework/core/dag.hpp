//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DAG_H__
#define __DAG_H__

#pragma once

BEGIN_NAMESPACE(core)

class dag : private noncopyable
{
    public:
        using dag_type = std::shared_ptr<dag>;
        using dags_type = std::vector<dag_type>;

        enum class flags : uint64_t
        {
            clear     = 0x0000,
            deleted   = 0x0001,
            processed = 0x0008
        };

        using flags_type = flags;

        struct dag_key_comparator
        {
            bool operator () (const dag_type& lhs, const dag_type& rhs) const
            {
                return (*lhs).my_id < (*rhs).my_id;
            }
        };

        using kids_type = std::set<dag_type, dag_key_comparator>;

    protected:
        std::size_t         my_id; // key

        dags_type           my_papas; // parents
        kids_type           my_kids;

        flags_type          my_flags;

    public:
                            dag();
        virtual            ~dag() = 0;

        std::size_t         id() const;
        std::size_t&        id();

        const dags_type&    papas() const;
        dags_type&          papas();

        const kids_type&    kids() const;
        kids_type&          kids();

        const flags_type&   flags() const;
        flags_type&         flags();
};

inline std::size_t dag::id() const
{
    return my_id;
}

inline std::size_t& dag::id()
{
    return my_id;
}

inline const typename dag::dags_type& dag::papas() const
{
    return my_papas;
}

inline typename dag::dags_type& dag::papas()
{
    return my_papas;
}

inline const typename dag::kids_type& dag::kids() const
{
    return my_kids;
}

inline typename dag::kids_type& dag::kids()
{
    return my_kids;
}

inline const typename dag::flags_type& dag::flags() const
{
    return my_flags;
}

inline typename dag::flags_type& dag::flags()
{
    return my_flags;
}

END_NAMESPACE

#endif // __DAG_H__
