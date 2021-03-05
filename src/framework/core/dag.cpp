//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#include <core/pch.hpp>
#include <core/dag.hpp>

BEGIN_NAMESPACE(core)

dag::dag()
    : my_flags(dag::flags_type::clear)
{
}

dag::~dag()
{
}

END_NAMESPACE
