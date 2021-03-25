//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <arcturus_type.hpp>
#include <arcturus_scalar_type.hpp>
#include <arcturus_enum_type.hpp>

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

arcturus_enum_type::arcturus_enum_type()
                  : arcturus_scalar_type(abstract_type<traits_type>::kind_type::enumeration_type)
{
}

arcturus_enum_type::~arcturus_enum_type()
{
}

bool operator == (const arcturus_enum_type& lhs, const arcturus_enum_type& rhs)
{
    lhs; rhs;
    return false; //??
}

bool operator != (const arcturus_enum_type& lhs, const arcturus_enum_type& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE
