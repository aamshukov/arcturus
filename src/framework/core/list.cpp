//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>
#include <core/unicode.hpp>
#include <core/text.hpp>
#include <core/enum.hpp>
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
