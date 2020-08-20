//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/list.hpp>
#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/vertex.hpp>
#include <core/edge.hpp>
#include <core/graph.hpp>

#include <core/timer.hpp>

//#include <content/data_provider.hpp>
//#include <content/file_data_provider.hpp>
//
//#include <frontend/grammar/symbol.hpp>
//#include <frontend/grammar/rule.hpp>
//#include <frontend/grammar/grammar.hpp>
//#include <frontend/grammar/grammar_algorithm.hpp>
//#include <frontend/grammar/grammar_visualization.hpp>
//
//#include <content/content.hpp>
//
//#include <frontend/lexical_analyzer/token.hpp>
//
//#include <frontend/lexical_analyzer/lexical_content.hpp>
//#include <frontend/lexical_analyzer/lexical_analyzer.hpp>
//#include <frontend/lexical_analyzer/lexical_analyzer.inl>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <symtable/symbol.hpp>
#include <symtable/scope/scope.hpp>
#include <symtable/symbol_table.hpp>
#include <symtable/symbol_table.inl>

#include <backend/optimization/pass.hpp>

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)


END_NAMESPACE
