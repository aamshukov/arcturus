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
#include <arcturus_struct_type.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

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
