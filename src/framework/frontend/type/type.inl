#ifndef __TYPE_INL__
#define __TYPE_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

inline type::type()
           : my_size(0), my_flags(type::flags::clear)
{
}

inline type::~type()
{
}

inline typename type::size_type type::size() const
{
    return my_size;
}

inline typename type::size_type& type::size()
{
    return my_size;
}

inline typename type::flags_type type::flags() const
{
    return my_flags;
}

inline typename type::flags_type& type::flags()
{
    return my_flags;
}

template <typename Traits>
abstract_type<Traits>::abstract_type()
                     : my_kind(abstract_type<Traits>::kind_type::unknown_type)
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

#endif // __TYPE_INL__
