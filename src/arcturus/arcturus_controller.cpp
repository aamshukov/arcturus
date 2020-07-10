﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/domain_helper.hpp>

#include <core/factory.hpp>
#include <core/singleton.hpp>

#include <core/status.hpp>

#include <core/diagnostics.hpp>
#include <core/statistics.hpp>

#include <core/logger.hpp>

#include <core/unicode.hpp>
#include <core/text.hpp>

#include <core/enum.hpp>
#include <core/enumerate.hpp>

#include <core/configurator.hpp>

#include <core/counter.hpp>

#include <core/context.hpp>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/vertex.hpp>
#include <core/edge.hpp>
#include <core/graph.hpp>

#include <core/timer.hpp>

#include <content/data_provider.hpp>
#include <content/file_data_provider.hpp>

#include <frontend/grammar/symbol.hpp>
#include <frontend/grammar/rule.hpp>
#include <frontend/grammar/grammar.hpp>
#include <frontend/grammar/grammar_algorithm.hpp>
#include <frontend/grammar/grammar_visualization.hpp>

#include <frontend/fsa/fsa_transition.hpp>
#include <frontend/fsa/fsa_state.hpp>
#include <frontend/fsa/fsa_state_set.hpp>
#include <frontend/fsa/fsa.hpp>
#include <frontend/fsa/fsa_algorithm.hpp>
#include <frontend/fsa/fsa_re.hpp>
#include <frontend/fsa/fsa_visualization.hpp>
#include <frontend/fsa/fsa_codegen.hpp>

#include <content/content.hpp>

#include <frontend/lexical_analyzer/token.hpp>

#include <frontend/lexical_analyzer/lexical_content.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.inl>

#include <frontend/type/type.hpp>
#include <frontend/type/type.inl>

#include <symtable/symbol.hpp>
#include <symtable/scope/scope.hpp>
#include <symtable/symbol_table.hpp>
#include <symtable/symbol_table.inl>

#include <frontend/parser/parse_tree_base.hpp>
#include <frontend/parser/parse_tree.hpp>
#include <frontend/parser/parse_dag.hpp>
#include <frontend/parser/parser.hpp>
#include <frontend/parser/parser.inl>
#include <frontend/parser/recursive_descent_parser.hpp>
#include <frontend/parser/recursive_descent_parser.inl>

#include <frontend/semantics/semantics.hpp>
#include <frontend/semantics/semantics.inl>

#include <ir/quadruple.hpp>
#include <ir/basic_block.hpp>
#include <ir/basic_block.inl>
#include <ir/control_flow_graph.hpp>
#include <ir/code.hpp>
#include <ir/code.inl>
#include <ir/ir_visitor.hpp>
#include <ir/ir.hpp>
#include <ir/ir.inl>

#include <backend/codegen/activation_record.hpp>
#include <backend/codegen/amd64/activation_record_amd64.hpp>
#include <backend/codegen/codegen.hpp>

#include <backend/optimization/pass.hpp>

#include <controller/controller.hpp>

#include <arcturus_configurator.hpp>
#include <arcturus_token.hpp>
#include <arcturus_type.hpp>
#include <arcturus_scalar_type.hpp>
#include <arcturus_array_type.hpp>
#include <arcturus_struct_type.hpp>
#include <arcturus_enum_type.hpp>
#include <arcturus_func_type.hpp>
#include <arcturus_symbol.hpp>
#include <arcturus_lexical_analyzer.hpp>
#include <arcturus_parse_tree.hpp>
#include <arcturus_parser.hpp>
#include <arcturus_controller.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)
USINGNAMESPACE(orchestration)

arcturus_controller::arcturus_controller(const typename arcturus_controller::parser_type& parser,
                                         const typename arcturus_controller::ir_type& ir,
                                         const typename arcturus_controller::passes_type& passes,
                                         const typename arcturus_controller::codegen_type& codegen,
                                         const typename arcturus_controller::context_type& context)
                   : controller<arcturus_token>(parser, ir, passes, codegen, context)
{
}

arcturus_controller::~arcturus_controller()
{
}

void arcturus_controller::initialize(const typename arcturus_controller::context_type& context)
{
    context;//??
}

void arcturus_controller::parse(const typename arcturus_controller::context_type& context)
{
    context;//??
    // lexing and buuld cst
    // populates tree in context
}

void arcturus_controller::converge(const typename arcturus_controller::context_type& context)
{
    context;//??

    // cst to ast
    // populate symbol table
    // semantics analisys
    // linearize
    // build basic blocks BB
    // build SSA form
}

void arcturus_controller::optimize(const typename arcturus_controller::context_type& context)
{
    context;//??

    //for pass in passes
    //    pass.execute
}

void arcturus_controller::codegen(const typename arcturus_controller::context_type& context)
{
    context;//??
}

void arcturus_controller::finalize(const typename arcturus_controller::context_type& context)
{
    context;//??
}

void arcturus_controller::compile()
{
    initialize(my_context);
    parse(my_context);
    converge(my_context);
    optimize(my_context);
    codegen(my_context);
    finalize(my_context);
}

END_NAMESPACE