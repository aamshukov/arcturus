//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONTROL_FLOW_GRAPH_H__
#define __CONTROL_FLOW_GRAPH_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

template <typename TBasicBlock>
class control_flow_graph : public graph<TBasicBlock>
{
    public:
        using instruction_type = typename TBasicBlock::instruction_type;
        using code_type = std::shared_ptr<code<instruction_type>>;

        using basic_block_type = std::shared_ptr<basic_block<instruction_type>>;
        using basic_blocks_type = std::vector<basic_block_type>;

        using symbol_type = typename instruction_type::symbol_type;
        using assignments_type = std::unordered_map<symbol_type, basic_blocks_type, symbol_hash, symbol_eq_key_comparator>;

    protected:
        assignments_type        my_assignments; // holds definitions and asignments of variables

    public:
                                control_flow_graph();
        virtual                ~control_flow_graph();

        const assignments_type& assignments() const;
        assignments_type&       assignments();

        virtual void            collect_basic_blocks(code_type& code) = 0;
        virtual void            flat_basic_blocks(code_type& code) = 0; // back to flat list of instructions
};

template <typename TBasicBlock>
control_flow_graph<TBasicBlock>::control_flow_graph()
                               : graph<TBasicBlock>(true)
{
}

template <typename TBasicBlock>
control_flow_graph<TBasicBlock>::~control_flow_graph()
{
}

template <typename TBasicBlock>
inline const typename control_flow_graph<TBasicBlock>::assignments_type& control_flow_graph<TBasicBlock>::assignments() const
{
    return my_assignments;
}

template <typename TBasicBlock>
inline typename control_flow_graph<TBasicBlock>::assignments_type& control_flow_graph<TBasicBlock>::assignments()
{
    return my_assignments;
}

END_NAMESPACE

#endif // __CONTROL_FLOW_GRAPH_H__
