//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONTROL_FLOW_GRAPH_H__
#define __CONTROL_FLOW_GRAPH_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token, typename OpCodeTraits>
class control_flow_graph : public graph<std::shared_ptr<basic_block<Token, OpCodeTraits>>,
                                        edge<std::shared_ptr<basic_block<Token, OpCodeTraits>>>>
{
    public:
        using token_type = Token;
        using traits_type = OpCodeTraits;

        using id_type = std::size_t;

        using quadruple_type = std::shared_ptr<quadruple<token_type, traits_type>>;
        using quadruples_type = std::list<quadruple_type>;

        using code_type = std::shared_ptr<code<token_type, traits_type>>;

        using basic_block_type = std::shared_ptr<basic_block<token_type, traits_type>>;
        using basic_blocks_type = std::list<basic_block_type>;

    public:

        virtual void build(const code_type& code) = 0;
};

END_NAMESPACE

#endif // __CONTROL_FLOW_GRAPH_H__
