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

void list::link(typename list::list_type list, typename list::list_type new_list)
{
    auto& next_list((*list).next());

    (*list).next() = new_list;
    (*new_list).prev() = list;

    if(next_list != nullptr)
    {
        (*next_list).prev() = new_list;
        (*new_list).next() = new_list;
    }
}

void list::unlink(typename list::list_type list)
{
    auto& next_list((*list).next());
    auto& prev_list((*list).prev());

    if(next_list != nullptr)
    {
        (*next_list).prev() = prev_list;
    }

    if(prev_list != nullptr)
    {
        (*prev_list).next() = next_list;
    }
}

END_NAMESPACE
