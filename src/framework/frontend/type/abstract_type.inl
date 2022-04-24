#ifndef __ABSTRACT_TYPE_INL__
#define __ABSTRACT_TYPE_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

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

#endif // __ABSTRACT_TYPE_INL__
