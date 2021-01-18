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

template <typename TBasicBlock>
class ssa : private noncopyable
{
    public:
        using instruction_type = typename TBasicBlock::instruction_type;

        using basic_block_type = std::shared_ptr<basic_block<instruction_type>>;
        using basic_blocks_type = std::vector<basic_block_type>;

        using control_flow_graph_type = std::shared_ptr<control_flow_graph<basic_block<instruction_type>>>;

    public:
        static void build_ssa_form(control_flow_graph_type& cfg);
};

END_NAMESPACE

#endif // __SSA_H__
