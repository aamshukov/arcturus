//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_SCALAR_TYPE_H__
#define __ARCTURUS_SCALAR_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

class arcturus_scalar_type : public arcturus_type
{
    public:
        using kind_type = typename arcturus_type::kind_type;
        using attributes_type = typename arcturus_type::attributes_type;

    public:
                                arcturus_scalar_type(kind_type kind = kind_type::unknown_type,
                                                     attributes_type attributes = attributes_type::unknown_type);

                                arcturus_scalar_type(const arcturus_scalar_type& other) = default;
                                arcturus_scalar_type(arcturus_scalar_type&& other) = default;

                               ~arcturus_scalar_type();

        arcturus_scalar_type&   operator = (const arcturus_scalar_type& other) = default;
        arcturus_scalar_type&   operator = (arcturus_scalar_type&& other) = default;

        friend bool             operator == (const arcturus_scalar_type& lhs, const arcturus_scalar_type& rhs);
        friend bool             operator != (const arcturus_scalar_type& lhs, const arcturus_scalar_type& rhs);

        bool                    equivalent(const type& other) const override;
};

END_NAMESPACE

#endif // __ARCTURUS_SCALAR_TYPE_H__
