//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSE_DAG_H__
#define __PARSE_DAG_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
//??USINGNAMESPACE(symtable)

template <typename Token, typename TreeTraits>
struct parse_dag : public dag, public visitable
{
    using token_type = Token;
    using tree_traits_type = TreeTraits;

    using gr_symbol_type = grammar::symbol_type;

    //using ir_symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
    //using ir_symbols_type = std::vector<ir_symbol_type>;

    //gr_symbol_type gr_symbol;
    //ir_symbol_type ir_symbol;

    virtual ~parse_dag()
    {
    }

    template <typename TVisitor> // deficiency of C++ ... template methods cannot be virtual ...
    typename TVisitor::return_type accept(TVisitor& visitor, const typename TVisitor::param_type& param)
    {
        if constexpr (std::is_void_v<typename TVisitor::return_type>)
        {
            if constexpr (std::is_void_v<typename TVisitor::param_type>)
            {
                visitor.visit(*this);
            }
            else
            {
                visitor.visit(*this, param);
            }
        }
        else
        {
            if constexpr (std::is_void_v<typename TVisitor::param_type>)
            {
                return visitor.visit(*this);
            }
            else
            {
                return visitor.visit(*this, param);
            }
        }
    }
};

END_NAMESPACE

#endif // __PARSE_DAG_H__
