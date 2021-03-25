//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_PARSE_TREE_VARIABLE_H__
#define __ARCTURUS_PARSE_TREE_VARIABLE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(symtable)
USING_NAMESPACE(frontend)

struct arcturus_parse_tree_variable : public arcturus_parse_tree
{
    virtual ~arcturus_parse_tree_variable()
    {
    }

    ACCEPT_METHOD;
};

END_NAMESPACE

#endif // __ARCTURUS_PARSE_TREE_VARIABLE_H__
