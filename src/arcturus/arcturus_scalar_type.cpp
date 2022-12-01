//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/abstract_type.hpp>

#include <arcturus_type.hpp>
#include <arcturus_scalar_type.hpp>

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

arcturus_scalar_type::arcturus_scalar_type(kind_type kind)
                    : arcturus_type(kind)
{
    if(my_kind == arcturus_type::kind_type::integer_type)
    {
        my_size = sizeof(std::size_t) * bits_in_byte;
    }
    else if(my_kind == arcturus_type::kind_type::real_type)
    {
        my_size = sizeof(double) * bits_in_byte;
    }
    else if(my_kind == arcturus_type::kind_type::boolean_type)
    {
        my_size = sizeof(bool) * bits_in_byte;
    }
    else if(my_kind == arcturus_type::kind_type::pointer_type)
    {
        my_size = sizeof(void*) * bits_in_byte;
    }
    else if(my_kind == arcturus_type::kind_type::byte_type)
    {
        my_size = sizeof(byte) * bits_in_byte;
    }
}

arcturus_scalar_type::~arcturus_scalar_type()
{
}

bool arcturus_scalar_type::equivalent(const type&) const
{
    return false; //??
}

bool operator == (const arcturus_scalar_type& lhs, const arcturus_scalar_type& rhs)
{
    return lhs.equivalent(rhs);
}

bool operator != (const arcturus_scalar_type& lhs, const arcturus_scalar_type& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE
