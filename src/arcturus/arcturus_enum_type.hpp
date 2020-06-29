//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_ENUM_TYPE_H__
#define __ARCTURUS_ENUM_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_enum_type : public arcturus_type
{
    public:
                        arcturus_enum_type();
                       ~arcturus_enum_type();

        bool            operator == (const arcturus_type& other) override;
        bool            operator != (const arcturus_type& other) override;
};

END_NAMESPACE

#endif // __ARCTURUS_ENUM_TYPE_H__
