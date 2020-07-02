//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_TYPE_H__
#define __ARCTURUS_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

struct arcturus_type_traits
{
    DECLARE_ENUM
    (
        kind,
        uint32_t,
        unknown_type     = 0,

        byte_type        = 100,
        integer_type     = 101,
        real_type        = 102,
        string_type      = 103,
        boolean_type     = 104,
        pointer_type     = 105, // ref

        array_type       = 150,

        structure_type   = 200,
        enumeration_type = 201,

        function_type    = 300,
        procedure_type   = 301,

        namespace_type   = 500,
        package_type     = 501,
        module_type      = 502,

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

class arcturus_type : public abstract_type<arcturus_type_traits>
{
    public:
                        arcturus_type(kind_type kind);

                        arcturus_type(const arcturus_type& other) = default;
                        arcturus_type(arcturus_type&& other) = default;

                       ~arcturus_type();

        arcturus_type&  operator = (const arcturus_type& other) = default;
        arcturus_type&  operator = (arcturus_type&& other) = default;
};

END_NAMESPACE

#endif // __ARCTURUS_TYPE_H__
