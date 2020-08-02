//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CODE_INL__
#define __CODE_INL__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

template <typename Token, typename Traits>
code<Token, Traits>::code()
{
}

template <typename Token, typename Traits>
code<Token, Traits>::~code()
{
}

END_NAMESPACE

#endif // __CODE_INL__
