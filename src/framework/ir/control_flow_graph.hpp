//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __CONTROL_FLOW_GRAPH_H__
#define __CONTROL_FLOW_GRAPH_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename TBasicBlock>
class control_flow_graph : public graph<TBasicBlock>
{
    public:
        using instruction_type = typename TBasicBlock::instruction_type;
        using code_type = std::shared_ptr<code<instruction_type>>;

        using basic_block_type = std::shared_ptr<basic_block<instruction_type>>;
        using basic_blocks_type = std::vector<basic_block_type>;

    public:
        virtual void build_hir(code_type& code) = 0;
        virtual void build_mir(code_type& code) = 0;
        virtual void build_lir(code_type& code) = 0;
};

END_NAMESPACE

#endif // __CONTROL_FLOW_GRAPH_H__
