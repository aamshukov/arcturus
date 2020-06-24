//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/dag.hpp>

BEGIN_NAMESPACE(core)

dag::dag()
    : my_flags(dag::flags::clear)
{
}

dag::~dag()
{
}

END_NAMESPACE
