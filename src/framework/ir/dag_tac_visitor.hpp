//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DAG_TAC_VISITOR_H__
#define __DAG_TAC_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)


template <typename TreeTraits>
struct dag_tac_visitor : public visitor<void, bool, parse_dag<TreeTraits>>
{
    using dag_type = parse_dag<TreeTraits>;

    void visit(dag_type& /*dag*/, const bool&) override
    {
    }
};

END_NAMESPACE

#endif // __DAG_TAC_VISITOR_H__
