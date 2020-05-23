//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DAG_H__
#define __DAG_H__

#pragma once

BEGIN_NAMESPACE(core)

struct dag : private noncopyable
{
    enum class flags : uint64_t
    {
        clear     = 0x0000,
        deleted   = 0x0001,
        processed = 0x0008
    };

    using flags_type = tmpl_flags<flags>;

    using dag_type = std::shared_ptr<dag>;
    using dags_type = std::vector<dag_type>;

    struct dag_key_comparator
    {
        bool operator () (const dag_type& lhs, const dag_type& rhs) const
        {
            return (*lhs).id < (*rhs).id;
        }
    };

    using kids_type = std::set<dag_type, dag_key_comparator>;

    std::size_t id; // key

    dags_type papas; // parents
    kids_type kids;

    flags_type flags;

    virtual ~dag()
    {
    }
};

END_NAMESPACE

#endif // __DAG_H__
