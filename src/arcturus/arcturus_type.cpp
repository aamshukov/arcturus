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

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

typename arcturus_type_traits::enum_map_type arcturus_type_traits::mapping;

arcturus_type::arcturus_type(kind_type kind) : abstract_type<arcturus_type_traits>(kind)
{
}

arcturus_type::~arcturus_type()
{
}

END_NAMESPACE
