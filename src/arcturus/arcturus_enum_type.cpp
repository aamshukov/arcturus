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
#include <arcturus_enum_type.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

arcturus_enum_type::arcturus_enum_type()
                  : arcturus_type(abstract_type<traits_type>::kind_type::enumeration_type)
{
}

arcturus_enum_type::~arcturus_enum_type()
{
}

bool arcturus_enum_type::operator == (const arcturus_type&)
{
    return false; //??
}

bool arcturus_enum_type::operator != (const arcturus_type&)
{
    return false; //??
}

END_NAMESPACE
