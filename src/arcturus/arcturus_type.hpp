//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_TYPE_H__
#define __ARCTURUS_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

class arcturus_type : public abstract_type<arcturus_type_traits>
{
    public:
        using kind_type = typename abstract_type<arcturus_type_traits>::kind_type;
        using attributes_type = typename abstract_type<arcturus_type_traits>::attributes_type;

    public:
                        arcturus_type(kind_type kind = kind_type::unknown_type,
                                      attributes_type attributes = attributes_type::unknown_type);

                        arcturus_type(const arcturus_type& other) = default;
                        arcturus_type(arcturus_type&& other) = default;

                       ~arcturus_type();

        arcturus_type&  operator = (const arcturus_type& other) = default;
        arcturus_type&  operator = (arcturus_type&& other) = default;
};

END_NAMESPACE

#endif // __ARCTURUS_TYPE_H__
