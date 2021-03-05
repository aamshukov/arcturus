//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#include <core/pch.hpp>

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
