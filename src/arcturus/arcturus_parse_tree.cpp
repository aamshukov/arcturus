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

#include <frontend/grammar/symbol.hpp>
#include <frontend/grammar/rule.hpp>
#include <frontend/grammar/grammar.hpp>

#include <content/content.hpp>

#include <frontend/lexical_analyzer/token.hpp>

#include <frontend/lexical_analyzer/lexical_content.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.hpp>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <symtable/symbol.hpp>

#include <frontend/parser/parse_tree_base.hpp>
#include <frontend/parser/parse_tree.hpp>

#include <arcturus_token.hpp>
#include <arcturus_type.hpp>
#include <arcturus_lexical_analyzer.hpp>
#include <arcturus_parse_tree.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)

arcturus_parse_tree::arcturus_parse_tree(const typename arcturus_parse_tree::tree_kind& kind)
                   : my_kind(kind)
{
}

arcturus_parse_tree::~arcturus_parse_tree()
{
}

END_NAMESPACE
