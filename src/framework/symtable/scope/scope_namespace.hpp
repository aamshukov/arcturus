//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SCOPE_NAMESPACE_H__
#define __SCOPE_NAMESPACE_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class scope_namespace : public scope
{
};

END_NAMESPACE

#endif // __SCOPE_NAMESPACE_H__
