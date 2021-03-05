//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __SSA_H__
#define __SSA_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

// 𝛗

template <typename TBasicBlock>
class ssa : private noncopyable
{
    protected:
        static const cp_type ssa_suffix = { '`' };
};

END_NAMESPACE

#endif // __SSA_H__
