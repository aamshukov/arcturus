//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/list.hpp>
#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/timer.hpp>

#include <content/data_provider.hpp>
#include <content/file_data_provider.hpp>

#include <frontend/grammar/grammar_symbol.hpp>
#include <frontend/grammar/grammar_rule.hpp>
#include <frontend/grammar/grammar.hpp>

#include <content/content.hpp>

#include <frontend/lexical_analyzer/token.hpp>

#include <frontend/lexical_analyzer/lexical_content.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>
#include <frontend/type/abstract_type.hpp>
#include <frontend/type/abstract_type.inl>

#include <symtable/symbol.hpp>

#include <frontend/parser/parse_tree_base.hpp>
#include <frontend/parser/parse_tree.hpp>
#include <frontend/parser/parse_context.hpp>

#include <arcturus_token.hpp>
#include <arcturus_parse_tree.hpp>
#include <arcturus_parse_context.hpp>

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(symtable)
USING_NAMESPACE(frontend)

arcturus_parse_context::arcturus_parse_context()
{
}

arcturus_parse_context::~arcturus_parse_context()
{
}

END_NAMESPACE
