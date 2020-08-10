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

class arcturus_control_flow_graph : public control_flow_graph<arcturus_quadruple>
{
    public:
        //using id_type = typename control_flow_graph<arcturus_quadruple>::id_type;

        //using quadruple_type = typename control_flow_graph<arcturus_quadruple>::quadruple_type;
        //using quadruples_type = typename control_flow_graph<arcturus_quadruple>::quadruples_type;

        using code_type = typename control_flow_graph<arcturus_quadruple>::code_type;

        //using basic_block_type = typename control_flow_graph<arcturus_quadruple>::basic_block_type;
        //using basic_blocks_type = typename control_flow_graph<arcturus_quadruple>::basic_blocks_type;

    public:
        void build(const code_type& code) override;
};

END_NAMESPACE

#endif // __ARCTURUS_CONTROL_FLOW_GRAPH_H__
