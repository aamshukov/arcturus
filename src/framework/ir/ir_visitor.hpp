//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __IR_VISITOR_H__
#define __IR_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)


//template <typename T>
//struct ir_visitor : public visitor<ir_visitor, >
//{
//    using token_type = T;
//
//    using tree_type = typename parser<token_type>::tree_type;
//    using trees_type = typename parser<token_type>::trees_type;
//
//    using dag_type = typename parser<token_type>::dag_type;
//    using dags_type = typename parser<token_type>::dags_type;
//
//    void visit(const tree_type& /*acceptor*/) override
//    {
//    }
//};

END_NAMESPACE

#endif // __IR_VISITOR_H__
