//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/domain_helper.hpp>

#include <core/factory.hpp>
#include <core/singleton.hpp>

#include <core/status.hpp>

#include <core/diagnostics.hpp>
#include <core/statistics.hpp>

#include <core/logger.hpp>

#include <core/unicode.hpp>
#include <core/text.hpp>

#include <core/enum.hpp>
#include <core/enumerate.hpp>

#include <core/configurator.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <arcturus_type.hpp>
#include <arcturus_scalar_type.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

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

bool operator == (const arcturus_scalar_type& lhs, const arcturus_scalar_type& rhs)
{
    return lhs.my_kind == rhs.my_kind;
}

bool operator != (const arcturus_scalar_type& lhs, const arcturus_scalar_type& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE
