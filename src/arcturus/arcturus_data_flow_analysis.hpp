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

        using operation_code = typename arcturus_quadruple::operation_code;

    public:
                        arcturus_data_flow_analysis();
                       ~arcturus_data_flow_analysis();

        virtual void    collect_gen_kills(basic_blocks_type& basic_blocks) override;
        virtual void    calculate_in_outs(basic_blocks_type& basic_blocks) override;
};

END_NAMESPACE

#endif // __ARCTURUS_DATA_FLOW_ANALYSIS_H__
