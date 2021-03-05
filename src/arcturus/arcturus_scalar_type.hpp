//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __ARCTURUS_SCALAR_TYPE_H__
#define __ARCTURUS_SCALAR_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_scalar_type : public arcturus_type
{
    public:
                                arcturus_scalar_type(kind_type kind);

                                arcturus_scalar_type(const arcturus_scalar_type& other) = default;
                                arcturus_scalar_type(arcturus_scalar_type&& other) = default;

                               ~arcturus_scalar_type();

        arcturus_scalar_type&   operator = (const arcturus_scalar_type& other) = default;
        arcturus_scalar_type&   operator = (arcturus_scalar_type&& other) = default;

        friend bool             operator == (const arcturus_scalar_type& lhs, const arcturus_scalar_type& rhs);
        friend bool             operator != (const arcturus_scalar_type& lhs, const arcturus_scalar_type& rhs);
};

END_NAMESPACE

#endif // __ARCTURUS_SCALAR_TYPE_H__
