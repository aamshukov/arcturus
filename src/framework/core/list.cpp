//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#include <core/pch.hpp>
#include <core/list.hpp>

BEGIN_NAMESPACE(core)

list::list()
    : my_flags(list::flags_type::clear)
{
}

list::~list()
{
}

END_NAMESPACE
