//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/tree.hpp>

BEGIN_NAMESPACE(core)

tree::tree()
    : my_id(0), my_flags(tree::flags_type::clear)
{
}

tree::~tree()
{
}

END_NAMESPACE
