//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DATA_FLOW_ANALYSIS_H__
#define __DATA_FLOW_ANALYSIS_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

template <typename TBasicBlock>
class data_flow_analysis : private noncopyable
{
    public:
        using instruction_type = typename TBasicBlock::instruction_type;
        using code_type = std::shared_ptr<code<instruction_type>>;

        using basic_block_type = std::shared_ptr<basic_block<instruction_type>>;
        using basic_blocks_type = std::vector<basic_block_type>;

        using symbol_type = typename instruction_type::symbol_type;

    public:
                        data_flow_analysis();
                       ~data_flow_analysis();

        virtual void    collect_gen_kills(basic_blocks_type& basic_blocks) = 0;
        virtual void    calculate_in_outs(basic_blocks_type& basic_blocks) = 0;
};

template <typename TBasicBlock>
data_flow_analysis<TBasicBlock>::data_flow_analysis()
{
}

template <typename TBasicBlock>
data_flow_analysis<TBasicBlock>::~data_flow_analysis()
{
}

END_NAMESPACE

#endif // __DATA_FLOW_ANALYSIS_H__
