//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_DATA_FLOW_ANALYSIS_H__
#define __ARCTURUS_DATA_FLOW_ANALYSIS_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

class arcturus_data_flow_analysis : public data_flow_analysis<basic_block<arcturus_quadruple>>
{
    public:
        using code_type = typename data_flow_analysis<basic_block<arcturus_quadruple>>::code_type;

        using basic_block_type = typename data_flow_analysis<basic_block<arcturus_quadruple>>::basic_block_type;
        using basic_blocks_type = typename data_flow_analysis<basic_block<arcturus_quadruple>>::basic_blocks_type;

        using symbol_type = typename basic_block<arcturus_quadruple>::symbol_type;
        using symbols_type = typename basic_block<arcturus_quadruple>::symbols_type;

        using control_flow_graph_type = typename data_flow_analysis<basic_block<arcturus_quadruple>>::control_flow_graph_type;

        using operation_code = typename arcturus_quadruple::operation_code;

    public:
                        arcturus_data_flow_analysis();
                       ~arcturus_data_flow_analysis();

        virtual void    collect_liveness_def_use_sets(control_flow_graph_type& cfg) override;
        virtual void    calculate_liveness_in_outs_sets(control_flow_graph_type& cfg) override;
};

END_NAMESPACE

#endif // __ARCTURUS_DATA_FLOW_ANALYSIS_H__
