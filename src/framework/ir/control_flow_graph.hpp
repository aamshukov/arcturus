//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONTROL_FLOW_GRAPH_H__
#define __CONTROL_FLOW_GRAPH_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Instruction>
class control_flow_graph : public graph<basic_block<Instruction>, edge<basic_block<Instruction>>>
{
    public:
        using id_type = std::size_t;

        using instruction_type = Instruction;
        using code_type = code<instruction_type>;

        using basic_block_type = std::shared_ptr<basic_block<instruction_type>>;
        //using basic_blocks_type = std::list<basic_block_type>;

    public:

        virtual void build_hir(code_type& code) = 0;
        virtual void build_mir(code_type& code) = 0;
        virtual void build_lir(code_type& code) = 0;
};

END_NAMESPACE

#endif // __CONTROL_FLOW_GRAPH_H__
