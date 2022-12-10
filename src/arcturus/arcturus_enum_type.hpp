//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_ENUM_TYPE_H__
#define __ARCTURUS_ENUM_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

class arcturus_enum_type : public arcturus_scalar_type
{
    public:
        using kind_type = typename arcturus_type::kind_type;
        using attributes_type = typename arcturus_type::attributes_type;

    public:
                    arcturus_enum_type();
                   ~arcturus_enum_type();

        friend bool operator == (const arcturus_enum_type& lhs, const arcturus_enum_type& rhs);
        friend bool operator != (const arcturus_enum_type& lhs, const arcturus_enum_type& rhs);
};

END_NAMESPACE

#endif // __ARCTURUS_ENUM_TYPE_H__
