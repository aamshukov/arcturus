//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/abstract_type.hpp>

#include <arcturus_type_traits.hpp>
#include <arcturus_type.hpp>
#include <arcturus_scalar_type.hpp>
#include <arcturus_func_type.hpp>

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

arcturus_func_type::arcturus_func_type(kind_type kind, attributes_type attributes)
                  : arcturus_scalar_type(kind, attributes)
{
}

arcturus_func_type::~arcturus_func_type()
{
}

bool operator == (const arcturus_func_type& lhs, const arcturus_func_type& rhs)
{
    lhs; rhs;
    return false; //??
}

bool operator != (const arcturus_func_type& lhs, const arcturus_func_type& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE
