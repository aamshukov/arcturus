//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ABSTRACT_TYPE_H__
#define __ABSTRACT_TYPE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

template <typename Traits>
class abstract_type : public type
{
    public:
        using traits_type = Traits;
        using kind_type = typename traits_type::kind;

    protected:
        kind_type       my_kind; // kind of type

    public:
                        abstract_type(kind_type kind = abstract_type<traits_type>::kind_type::unknown_type);

                        abstract_type(const abstract_type& other) = default;
                        abstract_type(abstract_type&& other) = default;

        virtual        ~abstract_type() = 0;

        abstract_type&  operator = (const abstract_type& other) = default;
        abstract_type&  operator = (abstract_type&& other) = default;

        kind_type       kind() const;
        kind_type&      kind();
};

template <typename Traits>
abstract_type<Traits>::abstract_type(typename abstract_type<Traits>::kind_type kind)
                     : my_kind(kind)
{
}

template <typename Traits>
abstract_type<Traits>::~abstract_type()
{
}

template <typename Traits>
inline typename abstract_type<Traits>::kind_type abstract_type<Traits>::kind() const
{
    return my_kind;
}

template <typename Traits>
inline typename abstract_type<Traits>::kind_type& abstract_type<Traits>::kind()
{
    return my_kind;
}

END_NAMESPACE

#endif // __ABSTRACT_TYPE_H__
