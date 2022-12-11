//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DATA_FLOW_ANALYSIS_H__
#define __DATA_FLOW_ANALYSIS_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)

template <typename TBasicBlock>
class data_flow_analysis : private noncopyable
{
    public:
        using instruction_type = typename TBasicBlock::instruction_type;
        using code_type = std::shared_ptr<code<instruction_type>>;

        using basic_block_type = std::shared_ptr<TBasicBlock>;
        using basic_blocks_type = std::vector<basic_block_type>;

        using control_flow_graph_type = std::shared_ptr<control_flow_graph<TBasicBlock>>;
        using interference_graph_type = std::shared_ptr<interference_graph>;

    public:
                        data_flow_analysis();
                       ~data_flow_analysis();

        virtual void    collect_liveness_def_use_sets(control_flow_graph_type& cfg) = 0;
        virtual void    calculate_liveness_in_outs_sets(control_flow_graph_type& cfg) = 0;

        virtual interference_graph_type
                        build_interference_graph(const control_flow_graph_type& cfg) = 0;
        virtual interference_graph_type
                        build_interference_graph_appel(const control_flow_graph_type& cfg) = 0;
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
