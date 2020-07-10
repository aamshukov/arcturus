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
#include <arcturus_func_type.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

arcturus_func_type::arcturus_func_type(kind_type kind)
                  : arcturus_scalar_type(kind)
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