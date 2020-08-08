//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_CONTROL_FLOW_GRAPH_H__
#define __ARCTURUS_CONTROL_FLOW_GRAPH_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

class arcturus_control_flow_graph : public control_flow_graph<arcturus_token, arcturus_operation_code_traits>
{
    public:
        using control_flow_graph_type = control_flow_graph<arcturus_token, arcturus_operation_code_traits>;

        using id_type = control_flow_graph_type::id_type;

        using quadruple_type = control_flow_graph_type::quadruple_type;
        using quadruples_type = control_flow_graph_type::quadruples_type;

        using code_type = control_flow_graph_type::code_type;

        using basic_block_type = control_flow_graph_type::basic_block_type;
        using basic_blocks_type = control_flow_graph_type::basic_blocks_type;

    public:
        void build(const code_type& code) override;
};

END_NAMESPACE

#endif // __ARCTURUS_CONTROL_FLOW_GRAPH_H__
