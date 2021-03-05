//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#include <core/pch.hpp>

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
