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

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

arcturus_type::arcturus_type(kind_type kind, attributes_type attributes) : abstract_type<arcturus_type_traits>(kind, attributes)
{
}

arcturus_type::~arcturus_type()
{
}

END_NAMESPACE
