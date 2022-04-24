//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/bitset.hpp>

#include <core/list.hpp>
#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/vertex.hpp>
#include <core/dominator_vertex.hpp>
#include <core/edge.hpp>
#include <core/graph.hpp>

#include <frontend/lexical_analyzer/token.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>
#include <frontend/type/abstract_type.hpp>
#include <frontend/type/abstract_type.inl>

#include <symtable/symbol.hpp>
#include <symtable/scope/scope.hpp>
#include <symtable/symbol_table.hpp>

#include <ir/quadruple.hpp>

#include <arcturus_token.hpp>
#include <arcturus_symbol.hpp>
#include <arcturus_quadruple.hpp>

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(backend)

typename arcturus_operation_code_traits::enum_map_type arcturus_operation_code_traits::mapping;

END_NAMESPACE
