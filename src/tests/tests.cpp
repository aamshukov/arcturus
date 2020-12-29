#include <core/pch.hpp>
#include <CppUnitTest.h>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/list.hpp>
#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/vertex.hpp>
#include <core/dominator_vertex.hpp>
#include <core/edge.hpp>
#include <core/graph.hpp>
#include <core/graph_algorithms.hpp>

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
#include <frontend/parser/parse_context.hpp>
#include <frontend/parser/parser.hpp>
#include <frontend/parser/parser.inl>
#include <frontend/parser/recursive_descent_parser.hpp>
#include <frontend/parser/recursive_descent_parser.inl>

#include <frontend/semantics/semantics.hpp>
#include <frontend/semantics/semantics.inl>

#include <ir/quadruple.hpp>
#include <ir/code.hpp>
#include <ir/code.inl>
#include <ir/basic_block.hpp>
#include <ir/basic_block.inl>
#include <ir/control_flow_graph.hpp>
#include <ir/ir_visitor.hpp>
#include <ir/ir.hpp>
#include <ir/ir.inl>

#include <backend/optimization/pass.hpp>

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
#include <arcturus_parse_context.hpp>
#include <arcturus_parser.hpp>
#include <arcturus_quadruple.hpp>
#include <arcturus_control_flow_graph.hpp>
#include <arcturus_ir.hpp>
#include <arcturus_pass.hpp>
#include <arcturus_controller.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)
USINGNAMESPACE(orchestration)
USINGNAMESPACE(arcturus)

namespace tests
{
    TEST_CLASS(ArcturusTests)
    {
        private:
            using arcturus_instruction = arcturus_quadruple;
            using arcturus_code = code<arcturus_instruction>;

        public:
            TEST_METHOD(InitLogger)
            {
                int argc = 10;

                char_type* argv[] = { L"dummy", L"--output-file-name", LR"(d:\tmp\log.art)", L"--input-file-name", LR"(d:\tmp\pf.dat)", L"--debug", L"-d", L"--debug", L"-d", L"--output-path", L"OO",  L"--output-path", L"O2", L"--library-path", L"1", L"--library-path", L"2", L"--build-type=release", L"--build-type=release", L"--color=red", L"--color", L"red" };

                arcturus_configurator::instance().configure(argc, argv);

                auto it_file = arcturus_configurator::instance().options().find(L"output-file-name");
                auto& path = (*it_file).second;
                auto file = std::filesystem::path(path).filename();
                auto log_path = std::filesystem::path(path).parent_path();
                auto log_file = uniqueue_file_name(file, L".log");
    
                log_path /= log_file;

                arcturus_configurator::instance().options().insert(std::pair(L"output-log-file-name", log_path));

                auto& config = arcturus_configurator::instance();
                config;

                logger::instance().initialize(log_path);
            }

            TEST_METHOD(BuildCode)
            {
            }

            TEST_METHOD(CreateGraph)
            {
                graph<vertex> gr;

                const auto& v1 = *gr.add_vertex(factory::create<vertex>(1)).first;
                const auto& v2 = *gr.add_vertex(factory::create<vertex>(2)).first;
                const auto& v3 = *gr.add_vertex(factory::create<vertex>(3)).first;
                const auto& v4 = *gr.add_vertex(factory::create<vertex>(4)).first;
                const auto& v5 = *gr.add_vertex(factory::create<vertex>(5)).first;
                const auto& v6 = *gr.add_vertex(factory::create<vertex>(6)).first;
                const auto& v7 = *gr.add_vertex(factory::create<vertex>(7)).first;
                const auto& v8 = *gr.add_vertex(factory::create<vertex>(8)).first;
                const auto& v9 = *gr.add_vertex(factory::create<vertex>(9)).first;

                gr.remove_vertex(v9);

                auto& e1 = *gr.add_edge(v1, v2, 0.5).first;
                gr.remove_edge(e1);

                gr.add_edge(v1, v2, 0.1);
                gr.add_edge(v1, v5, 0.2);
                gr.add_edge(v1, v3, 0.3);
                gr.add_edge(v1, v7, 0.3);
                gr.add_edge(v2, v1, 0.4);
                gr.add_edge(v3, v2, 0.5);
                gr.add_edge(v7, v8, 0.3);
                gr.add_edge(v8, v1, 0.3);
                gr.add_edge(v8, v2, 0.3);

                gr.generate_graphviz_file(LR"(d:\tmp\CreateGraph0.dot)", false);

                gr.remove_vertex(v1);

                gr.generate_graphviz_file(LR"(d:\tmp\CreateGraph1.dot)", false);
            }

            TEST_METHOD(VisualizeGraphGraphviz)
            {
                graph<vertex> gr;

                const auto& v1 = *gr.add_vertex(factory::create<vertex>(1)).first;
                const auto& v2 = *gr.add_vertex(factory::create<vertex>(2)).first;
                const auto& v3 = *gr.add_vertex(factory::create<vertex>(3)).first;
                const auto& v4 = *gr.add_vertex(factory::create<vertex>(4)).first;
                const auto& v5 = *gr.add_vertex(factory::create<vertex>(5)).first;
                const auto& v6 = *gr.add_vertex(factory::create<vertex>(6)).first;

                gr.add_edge(v1, v2, 0.5);
                gr.add_edge(v2, v3, 0.5);
                gr.add_edge(v2, v4, 0.5);
                gr.add_edge(v2, v6, 0.5);
                gr.add_edge(v3, v5, 0.5);
                gr.add_edge(v4, v5, 0.5);
                gr.add_edge(v5, v2, 0.5);

                graph<vertex>::vertices_type predecessors;
                gr.collect_predecessors(v2, predecessors);

                graph<vertex>::vertices_type successors;
                gr.collect_successors(v2, successors);

                gr.generate_graphviz_file(LR"(d:\tmp\GraphGraphviz.dot)", false);

                // D:\Soft\graphviz\2.38\release\bin\dot -Tpng d:\tmp\GraphGraphviz.dot -o d:\tmp\GraphGraphviz.png
                // for %i in (d:\tmp\*.dot) do D:\Soft\graphviz\2.38\release\bin\dot -Tpng %i -o %i.png
            }

            TEST_METHOD(ComputeDominatorsGraph)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& v1 = *(*gr).add_vertex(factory::create<dominator_vertex>(1)).first;
                const auto& v2 = *(*gr).add_vertex(factory::create<dominator_vertex>(2)).first;
                const auto& v3 = *(*gr).add_vertex(factory::create<dominator_vertex>(3)).first;
                const auto& v4 = *(*gr).add_vertex(factory::create<dominator_vertex>(4)).first;
                const auto& v5 = *(*gr).add_vertex(factory::create<dominator_vertex>(5)).first;
                const auto& v6 = *(*gr).add_vertex(factory::create<dominator_vertex>(6)).first;
                const auto& v7 = *(*gr).add_vertex(factory::create<dominator_vertex>(7)).first;
                const auto& v8 = *(*gr).add_vertex(factory::create<dominator_vertex>(8)).first;
                const auto& v9 = *(*gr).add_vertex(factory::create<dominator_vertex>(9)).first;

                (*gr).remove_vertex(v9);

                auto& e1 = *(*gr).add_edge(v1, v2, 0.5).first;
                (*gr).remove_edge(e1);

                (*gr).add_edge(v1, v2, 0.1);
                (*gr).add_edge(v1, v5, 0.2);
                (*gr).add_edge(v1, v3, 0.3);
                (*gr).add_edge(v1, v7, 0.3);
                (*gr).add_edge(v2, v1, 0.4);
                (*gr).add_edge(v3, v2, 0.5);
                (*gr).add_edge(v7, v8, 0.3);
                (*gr).add_edge(v8, v1, 0.3);
                (*gr).add_edge(v8, v2, 0.3);

                graph_algorithms::compute_dominators(gr);

                (*gr).generate_graphviz_file(LR"(d:\tmp\ComputeDominatorsGraph.dot)", false);
            }
    };
}
