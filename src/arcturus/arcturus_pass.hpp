//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_OPTIMIZATION_PASS_H__
#define __ARCTURUS_OPTIMIZATION_PASS_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(backend)

class arcturus_pass : public pass
{
    public:
        using context_type = typename pass::context_type;

    public:
                    arcturus_pass() = default;
        virtual    ~arcturus_pass() = 0;
};

END_NAMESPACE

#endif // __ARCTURUS_OPTIMIZATION_PASS_H__
