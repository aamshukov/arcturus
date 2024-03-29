﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SSA_H__
#define __SSA_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)

// 𝛗

template <typename TBasicBlock>
class ssa : private noncopyable
{
    protected:
        static const cp_type ssa_suffix = { '`' };
};

END_NAMESPACE

#endif // __SSA_H__
