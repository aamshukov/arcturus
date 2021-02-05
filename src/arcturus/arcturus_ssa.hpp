//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
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
        using basic_blocks_type = std::vector<basic_block_type>;

        using control_flow_graph_type = std::shared_ptr<arcturus_control_flow_graph>;

        using id_type = size_type;

        using symbol_type = typename arcturus_quadruple::symbol_type;
        using arcturus_instruction_type = std::shared_ptr<arcturus_quadruple>;

        using dominance_tree_type = std::shared_ptr<dominance_tree>;

        struct entry
        {
            size_type version;
            std::stack<id_type> stack;
        };

        using entry_type = entry;

        //??using token_type = typename arcturus_quadruple::token_type;
        //using symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
        //using entries_type = std::unordered_map<symbol_type,
        //                                        entry_type,
        //                                        symbol_hash<symtable::symbol<token_type>>,
        //                                        symbol_eq_key_comparator<symtable::symbol<token_type>>>;
    private:
        static arcturus_instruction_type
                    make_phi_instruction(const symbol_type& v, id_type n);

        static void place_phi_functions(const symbol_type& v, control_flow_graph_type& cfg);

        static void rename_variables(const symbol_type& v,
                                     const dominance_tree_type& dt,
                                     const control_flow_graph_type& cfg);
        static void rename_variables(const symbol_type& v,
                                     const dominance_tree_type& x,
                                     const control_flow_graph_type& cfg,
                                     entry_type& e);

    public:
        static void build_ssa_form(control_flow_graph_type& cfg);
};

END_NAMESPACE

#endif // __ARCTURUS_SSA_H__
