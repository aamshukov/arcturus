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
#include <arcturus_array_type.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

arcturus_array_type::arcturus_array_type(const typename arcturus_array_type::base_type& base_type)
                   : arcturus_type(abstract_type<traits_type>::kind_type::array_type),
                     my_base(base_type),
                     my_array_lower_bound(0),
                     my_array_upper_bound(0),
                     my_checked_array(true),
                     my_rowbased_array(true)
{
}

arcturus_array_type::~arcturus_array_type()
{
}

bool operator == (const arcturus_array_type& lhs, const arcturus_array_type& rhs)
{
    lhs; rhs;
    return false; //??
}

bool operator != (const arcturus_array_type& lhs, const arcturus_array_type& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE
