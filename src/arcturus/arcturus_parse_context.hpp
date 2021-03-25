//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_PARSE_CONTEXT_H__
#define __ARCTURUS_PARSE_CONTEXT_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

class arcturus_parse_context : public parse_context
{
    public:
        using tree_type = typename parse_context::tree_type;

    private:
        // extra data

    public:
                    arcturus_parse_context();
        virtual    ~arcturus_parse_context();
};

END_NAMESPACE

#endif // __ARCTURUS_PARSE_CONTEXT_H__
