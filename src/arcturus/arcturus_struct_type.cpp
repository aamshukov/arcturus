//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>
#include <frontend/type/abstract_type.hpp>
#include <frontend/type/abstract_type.inl>

#include <arcturus_type.hpp>
#include <arcturus_scalar_type.hpp>
#include <arcturus_struct_type.hpp>

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

arcturus_struct_type::arcturus_struct_type()
                    : arcturus_scalar_type(abstract_type<traits_type>::kind_type::structure_type)
{
}

arcturus_struct_type::~arcturus_struct_type()
{
}

bool operator == (const arcturus_struct_type& lhs, const arcturus_struct_type& rhs)
{
    lhs; rhs;
    return false; //??
}

bool operator != (const arcturus_struct_type& lhs, const arcturus_struct_type& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE
