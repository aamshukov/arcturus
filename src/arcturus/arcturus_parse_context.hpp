//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __ARCTURUS_PARSE_CONTEXT_H__
#define __ARCTURUS_PARSE_CONTEXT_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_parse_context : public parse_context<arcturus_token>
{
    public:
        using tree_type = parse_tree_base<arcturus_token>;

    private:
        // extra data

    public:
                    arcturus_parse_context();
        virtual    ~arcturus_parse_context();
};

END_NAMESPACE

#endif // __ARCTURUS_PARSE_CONTEXT_H__
