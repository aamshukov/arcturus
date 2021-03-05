//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __ARCTURUS_CONTROL_FLOW_GRAPH_H__
#define __ARCTURUS_CONTROL_FLOW_GRAPH_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

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
                       ~arcturus_control_flow_graph();

        virtual void    collect_basic_blocks(code_type& code) override;

        void            generate_graphviz_file(const string_type& file_name);
};

END_NAMESPACE

#endif // __ARCTURUS_CONTROL_FLOW_GRAPH_H__
