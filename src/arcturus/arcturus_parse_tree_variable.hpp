//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_PARSE_TREE_VARIABLE_H__
#define __ARCTURUS_PARSE_TREE_VARIABLE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)

struct arcturus_parse_tree_variable : public arcturus_parse_tree
{
    virtual ~arcturus_parse_tree_variable()
    {
    }

    ACCEPT_METHOD;
};

END_NAMESPACE

#endif // __ARCTURUS_PARSE_TREE_VARIABLE_H__
