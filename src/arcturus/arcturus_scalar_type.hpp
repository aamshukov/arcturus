//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
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
                   ~arcturus_scalar_type();

        bool        operator == (const arcturus_type& other) override;
        bool        operator != (const arcturus_type& other) override;
};

END_NAMESPACE

#endif // __ARCTURUS_SCALAR_TYPE_H__
