//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __ARCTURUS_SSA_H__
#define __ARCTURUS_SSA_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

// 𝛗

class arcturus_ssa : public ssa<basic_block<arcturus_quadruple>>
{
    public:
        using basic_block_type = std::shared_ptr<basic_block<arcturus_quadruple>>;
        using basic_blocks_type = std::set<basic_block_type, vertex_lt_key_comparator<basic_block<arcturus_quadruple>>>;

        using control_flow_graph_type = std::shared_ptr<arcturus_control_flow_graph>;

        using id_type = std::size_t;

        using symbol_type = typename arcturus_quadruple::symbol_type;
        using arcturus_instruction_type = std::shared_ptr<arcturus_quadruple>;

        using dominance_tree_type = std::shared_ptr<dominance_tree>;

        struct entry
        {
            std::size_t version;
            std::stack<id_type> stack;
        };

        using entry_type = entry;

    private:
        static arcturus_instruction_type
                    make_phi_instruction(const symbol_type& v, const basic_blocks_type& predecessors);

        static void place_phi_functions(const symbol_type& v, control_flow_graph_type& cfg);

        static void rename_variables(const symbol_type& v,
                                     const dominance_tree_type& dt,
                                     const control_flow_graph_type& cfg);
        static void rename_variables(const symbol_type& v,
                                     const dominance_tree_type& x,
                                     const control_flow_graph_type& cfg,
                                     entry_type& e);
        static void rename_variables(const control_flow_graph_type& cfg);

    public:
        static void build_ssa_form(control_flow_graph_type& cfg);
        static void destruct_ssa_form(control_flow_graph_type& cfg);
};

END_NAMESPACE

#endif // __ARCTURUS_SSA_H__
