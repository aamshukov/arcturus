//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/enum.hpp>
#include <core/enumerate.hpp>
#include <core/flags.hpp>
#include <core/counter.hpp>

#include <core/factory.hpp>
#include <core/singleton.hpp>

#include <core/domain_helper.hpp>

#include <core/status.hpp>
#include <core/diagnostics.hpp>
#include <core/statistics.hpp>

#include <core/text.hpp>
#include <core/unicode.hpp>

#include <core/configurator.hpp>

#include <core/dag.hpp>
#include <core/tree.hpp>

#include <core/visitor.hpp>
#include <core/visitable.hpp>

#include <core/logger.hpp>

#include <frontend/grammar/token.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <symtable/symbol.hpp>
#include <symtable/scope/scope.hpp>
#include <symtable/symbol_table.hpp>
#include <symtable/symbol_table.inl>

#include <backend/ir/quadruple.hpp>

#include <backend/ir/basic_block.hpp>
#include <backend/ir/control_flow_graph.hpp>

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)


END_NAMESPACE
