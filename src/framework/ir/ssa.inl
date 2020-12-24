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
template <typename Instruction>
void ssa<Instruction>::build_dominator_tree(const typename ssa<Instruction>::control_flow_graph_type& cfg,
                                            typename ssa<Instruction>::dominator_tree_type& dominator_tree)
{
}

template <typename Instruction>
void ssa<Instruction>::get_dominance_frontiers(const typename ssa<Instruction>::control_flow_graph_type& cfg,
                                               typename ssa<Instruction>::dominance_frontiers_type& dominance_frontiers)
{
}

template <typename Instruction>
void ssa<Instruction>::build_ssa_form(const typename ssa<Instruction>::control_flow_graph_type& cfg_non_ssa,
                                      typename ssa<Instruction>::control_flow_graph_type& cfg_ssa)
{
}

template <typename Instruction>
void ssa<Instruction>::build_normal_form(const typename ssa<Instruction>::control_flow_graph_type& cfg_ssa,
                                         typename ssa<Instruction>::control_flow_graph_type& cfg_non_ssa)
{
}

END_NAMESPACE

#endif // __SSA_H__
