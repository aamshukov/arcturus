//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_STRUCT_TYPE_H__
#define __ARCTURUS_STRUCT_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_struct_type : public arcturus_scalar_type
{
    public:
        using member_type = std::shared_ptr<arcturus_type>;
        using members_type = std::vector<member_type>;

    private:
        members_type            my_members;

    public:
                                arcturus_struct_type();

                                arcturus_struct_type(const arcturus_struct_type& other) = default;
                                arcturus_struct_type(arcturus_struct_type&& other) = default;

                               ~arcturus_struct_type();

        arcturus_struct_type&   operator = (const arcturus_struct_type& other) = default;
        arcturus_struct_type&   operator = (arcturus_struct_type&& other) = default;

        friend bool             operator == (const arcturus_struct_type& lhs, const arcturus_struct_type& rhs);
        friend bool             operator != (const arcturus_struct_type& lhs, const arcturus_struct_type& rhs);

        const members_type&     members() const;
        members_type&           members();
};

inline const typename arcturus_struct_type::members_type& arcturus_struct_type::members() const
{
    return my_members;
}

inline typename arcturus_struct_type::members_type& arcturus_struct_type::members()
{
    return my_members;
}

END_NAMESPACE

#endif // __ARCTURUS_STRUCT_TYPE_H__
