#ifndef __TYPE_INL__
#define __TYPE_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

inline type::type()
           : my_size(0),
             my_flags(type::flags_type::clear),
             my_cardinality(0)
{
}

inline type::~type()
{
}

inline const string_type& type::name() const
{
    return my_name;
}

inline string_type& type::name()
{
    return my_name;
}

inline typename type::size_type type::size() const
{
    return my_size;
}

inline typename type::size_type& type::size()
{
    return my_size;
}

inline typename type::size_type type::platform_size() const
{
    return my_platform_size;
}

inline typename type::size_type& type::platform_size()
{
    return my_platform_size;
}

inline typename type::flags_type type::flags() const
{
    return my_flags;
}

inline typename type::flags_type& type::flags()
{
    return my_flags;
}


inline typename type::size_type type::cardinality() const
{
    return my_cardinality;
}

inline typename type::size_type& type::cardinality()
{
    return my_cardinality;
}


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

#endif // __TYPE_INL__
