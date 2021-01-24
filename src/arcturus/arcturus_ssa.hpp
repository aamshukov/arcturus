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
        using instruction_type = typename ssa<basic_block<arcturus_quadruple>>::instruction_type;

        using basic_block_type = typename ssa<basic_block<arcturus_quadruple>>::basic_block_type;
        using basic_blocks_type = typename ssa<basic_block<arcturus_quadruple>>::basic_blocks_type;

        using control_flow_graph_type = typename ssa<basic_block<arcturus_quadruple>>::control_flow_graph_type;

        using arcturus_instruction_type = std::shared_ptr<instruction_type>;

    private:
        arcturus_instruction_type   make_phi_instruction(size_type n);

    public:
                                    arcturus_ssa();
                                   ~arcturus_ssa();

        void                        build_ssa_form(control_flow_graph_type& cfg);
};

END_NAMESPACE

#endif // __ARCTURUS_SSA_H__
