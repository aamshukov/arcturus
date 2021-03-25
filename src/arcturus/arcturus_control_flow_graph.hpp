//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_CONTROL_FLOW_GRAPH_H__
#define __ARCTURUS_CONTROL_FLOW_GRAPH_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)
USING_NAMESPACE(backend)

class arcturus_control_flow_graph : public control_flow_graph<basic_block<arcturus_quadruple>>
{
    public:
        using code_type = typename control_flow_graph<basic_block<arcturus_quadruple>>::code_type;

        using basic_block_type = typename control_flow_graph<basic_block<arcturus_quadruple>>::basic_block_type;
        using basic_blocks_type = typename control_flow_graph<basic_block<arcturus_quadruple>>::basic_blocks_type;

        using operation_code = typename arcturus_quadruple::operation_code;

    private:
        bool            is_assignment(const operation_code& operation);

    public:
                        arcturus_control_flow_graph();
        virtual        ~arcturus_control_flow_graph();

        virtual void    collect_basic_blocks(code_type& code) override;
        virtual void    flat_basic_blocks(code_type& code) override;

        void            generate_graphviz_file(const string_type& file_name);
};

END_NAMESPACE

#endif // __ARCTURUS_CONTROL_FLOW_GRAPH_H__
