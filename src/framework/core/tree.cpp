//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>
#include <core/unicode.hpp>
#include <core/text.hpp>
#include <core/enum.hpp>
#include <core/tree.hpp>

BEGIN_NAMESPACE(core)

tree::tree()
    : my_flags(tree::flags_type::clear)
{
}

tree::~tree()
{
}

END_NAMESPACE
