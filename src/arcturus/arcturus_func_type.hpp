//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_FUNC_TYPE_H__
#define __ARCTURUS_FUNC_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_func_type : public arcturus_type
{
    public:
                            arcturus_func_type(kind_type kind); // function_type or procedure_type
                           ~arcturus_func_type();

        bool                operator == (const arcturus_type& other) override;
        bool                operator != (const arcturus_type& other) override;
};

END_NAMESPACE

#endif // __ARCTURUS_FUNC_TYPE_H__
