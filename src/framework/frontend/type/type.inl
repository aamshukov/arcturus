#ifndef __TYPE_INL__
#define __TYPE_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

inline type::type()
           : my_size(0),
             my_platform_size(0),
             my_alignment(0),
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

inline std::size_t type::size() const
{
    return my_size;
}

inline std::size_t& type::size()
{
    return my_size;
}

inline std::size_t type::platform_size() const
{
    return my_platform_size;
}

inline std::size_t& type::platform_size()
{
    return my_platform_size;
}

inline std::size_t type::alignment() const
{
    return my_alignment;
}

inline std::size_t& type::alignment()
{
    return my_alignment;
}

inline typename type::flags_type type::flags() const
{
    return my_flags;
}

inline typename type::flags_type& type::flags()
{
    return my_flags;
}


inline std::size_t type::cardinality() const
{
    return my_cardinality;
}

inline std::size_t& type::cardinality()
{
    return my_cardinality;
}

END_NAMESPACE

#endif // __TYPE_INL__
