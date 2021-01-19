//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
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

        using token_type = typename instruction_type::token_type;
        using symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
        using assignments_type = std::unordered_map<symbol_type, basic_block_type,
                                                    symbol_hash<symtable::symbol<token_type>>,
                                                    symbol_eq_key_comparator<symtable::symbol<token_type>>>;
    protected:
        assignments_type        my_assignments;

    public:
                                control_flow_graph();
                               ~control_flow_graph();

        const assignments_type& assignments() const;
        assignments_type&       assignments();

        virtual void            build_hir(code_type& code) = 0;
        virtual void            build_mir(code_type& code) = 0;
        virtual void            build_lir(code_type& code) = 0;
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
