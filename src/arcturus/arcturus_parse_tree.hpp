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

struct arcturus_parse_tree : public parse_tree<token<arcturus_token_traits>>
{
    using token_type = token<arcturus_token_traits>;

    virtual ~arcturus_parse_tree()
    {
    }

    ACCEPT_METHOD;
};

END_NAMESPACE

#endif // __ARCTURUS_PARSE_TREE_H__
