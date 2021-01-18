//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __SSA_INL__
#define __SSA_INL__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

// 𝛗
template <typename TBasicBlock>
void ssa<TBasicBlock>::build_ssa_form(typename ssa<TBasicBlock>::control_flow_graph_type& cfg)
{
}

END_NAMESPACE

#endif // __SSA_H__
