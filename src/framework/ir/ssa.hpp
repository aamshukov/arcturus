//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __SSA_H__
#define __SSA_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

// 𝛗

template <typename Instruction>
class ssa : private noncopyable
{
    public:
        using instruction_type = Instruction;
        using control_flow_graph_type = control_flow_graph<instruction_type>;
        using basic_block_type = typename control_flow_graph<instruction_type>::basic_block_type;
        using basic_blocks_type = std::set<basic_block_type>;
        using dominance_frontier_type = std::unordered_map<basic_block_type, basic_blocks_type>;
        using dominance_frontiers_type = std::vector<dominance_frontier_type>;

        struct dominator_tree : public tree, public visitable
        {
        };

        using dominator_tree_type =  dominator_tree;

    private:
        static void build_dominator_tree(const control_flow_graph_type& cfg, dominator_tree_type& dominator_tree);
        static void get_dominance_frontiers(const control_flow_graph_type& cfg, dominance_frontiers_type& dominance_frontiers);

    public:
        static void build_ssa_form(const control_flow_graph_type& cfg_non_ssa, control_flow_graph_type& cfg_ssa);
        static void build_normal_form(const control_flow_graph_type& cfg_ssa, control_flow_graph_type& cfg_non_ssa);

        //static void restore(); //??
};

END_NAMESPACE

#endif // __SSA_H__
