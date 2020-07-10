//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_PARSE_TREE_H__
#define __ARCTURUS_PARSE_TREE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)

class arcturus_parse_tree : public parse_tree<arcturus_token>
{
    public:
        using token_type = arcturus_token;

    public:
                    arcturus_parse_tree();
        virtual    ~arcturus_parse_tree();

        ACCEPT_METHOD;
};

END_NAMESPACE

#endif // __ARCTURUS_PARSE_TREE_H__
