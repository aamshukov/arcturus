//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_TYPE_TRAITS_H__
#define __ARCTURUS_TYPE_TRAITS_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

struct arcturus_type_traits
{
    enum class kind : std::size_t
    {
        unknown_type     = 0,

        byte_type        = 100,
        integer_type     = 101,
        real_type        = 102,
        string_type      = 103,
        boolean_type     = 104,
        pointer_type     = 105, // ref/ptr

        array_type       = 150,

        // the following one (1) entry MUST be the last entry in the enum
        size
    };

    enum class attributes : std::size_t
    {
        unknown_type     = 0,

        structure_type   = 200,
        enumeration_type = 201,

        function_type    = 300,
        procedure_type   = 301,

        namespace_type   = 500,
        package_type     = 501,
        module_type      = 502,

        constant         = 1024, // const
        readonly         = 1025, // let
        variable         = 1026, // var
        address,
        param,

        // the following one (1) entry MUST be the last entry in the enum
        size
    };
};

END_NAMESPACE

#endif // __ARCTURUS_TYPE_TRAITS_H__
