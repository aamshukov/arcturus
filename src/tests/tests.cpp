#include <core/pch.hpp>
#include <CppUnitTest.h>

#include <core/visitable.hpp>
#include <core/visitor.hpp>

#include <core/bitset.hpp>

#include <core/list.hpp>
#include <core/tree.hpp>
#include <core/dag.hpp>

#include <core/vertex.hpp>
#include <core/dominator_vertex.hpp>
#include <core/dominance_tree.hpp>
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
#include <ir/basic_block.hpp>
#include <ir/basic_block.inl>
#include <ir/control_flow_graph.hpp>
#include <ir/ssa.hpp>
#include <ir/ssa.inl>
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
#include <arcturus_ssa.hpp>
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
        public:
            TEST_METHOD(InitLogger)
            {
                int argc = 10;

                char_type* argv[] = { L"dummy", L"--output-file-name", LR"(d:\tmp\logs\log.art)", L"--input-file-name", LR"(d:\tmp\pf.dat)", L"--debug", L"-d", L"--debug", L"-d", L"--output-path", L"OO",  L"--output-path", L"O2", L"--library-path", L"1", L"--library-path", L"2", L"--build-type=release", L"--build-type=release", L"--color=red", L"--color", L"red" };

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

            TEST_METHOD(CreateGraph)
            {
                std::shared_ptr<graph<vertex>> gr(factory::create<graph<vertex>>());

                const auto& v1 = *(*gr).add_vertex(factory::create<vertex>(1)).first;
                const auto& v2 = *(*gr).add_vertex(factory::create<vertex>(2)).first;
                const auto& v3 = *(*gr).add_vertex(factory::create<vertex>(3)).first;
                const auto& v4 = *(*gr).add_vertex(factory::create<vertex>(4)).first;
                const auto& v5 = *(*gr).add_vertex(factory::create<vertex>(5)).first;
                const auto& v6 = *(*gr).add_vertex(factory::create<vertex>(6)).first;
                const auto& v7 = *(*gr).add_vertex(factory::create<vertex>(7)).first;
                const auto& v8 = *(*gr).add_vertex(factory::create<vertex>(8)).first;
                const auto& v9 = *(*gr).add_vertex(factory::create<vertex>(9)).first;

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

                graph_algorithms<vertex>::generate_graphviz_file(gr, LR"(d:\tmp\CreateGraph0.dot)", false);

                (*gr).remove_vertex(v1);

                graph_algorithms<vertex>::generate_graphviz_file(gr, LR"(d:\tmp\CreateGraph1.dot)", false);
            }

            TEST_METHOD(VisualizeGraphGraphviz)
            {
                std::shared_ptr<graph<vertex>> gr(factory::create<graph<vertex>>());

                const auto& v1 = *(*gr).add_vertex(factory::create<vertex>(1)).first;
                const auto& v2 = *(*gr).add_vertex(factory::create<vertex>(2)).first;
                const auto& v3 = *(*gr).add_vertex(factory::create<vertex>(3)).first;
                const auto& v4 = *(*gr).add_vertex(factory::create<vertex>(4)).first;
                const auto& v5 = *(*gr).add_vertex(factory::create<vertex>(5)).first;
                const auto& v6 = *(*gr).add_vertex(factory::create<vertex>(6)).first;

                (*gr).add_edge(v1, v2, 0.5);
                (*gr).add_edge(v2, v3, 0.5);
                (*gr).add_edge(v2, v4, 0.5);
                (*gr).add_edge(v2, v6, 0.5);
                (*gr).add_edge(v3, v5, 0.5);
                (*gr).add_edge(v4, v5, 0.5);
                (*gr).add_edge(v5, v2, 0.5);

                graph<vertex>::vertices_type predecessors;
                (*gr).collect_predecessors(v2, predecessors);

                graph<vertex>::vertices_type successors;
                (*gr).collect_successors(v2, successors);

                graph_algorithms<vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphGraphviz.dot)", false);

                // D:\Soft\graphviz\2.38\release\bin\dot -Tpng d:\tmp\GraphGraphviz.dot -o d:\tmp\GraphGraphviz.png
                // for %i in (d:\tmp\*.dot) do D:\Soft\graphviz\2.38\release\bin\dot -Tpng %i -o %i.png
            }

            TEST_METHOD(CreateBitset)
            {
                std::bitset<5> bs0;
                auto s0 = bs0.to_string();

                bs0.set(0);
                s0 = bs0.to_string();

                bs0.set(2);
                s0 = bs0.to_string();

                auto bt = bs0[3];
                auto rc = bt == 1;
                bt = 1;
                s0 = bs0.to_string();
                bt.flip();
                s0 = bs0.to_string();

                bs0[4] = 1;
                s0 = bs0.to_string();

                bs0.set();
                s0 = bs0.to_string();


                bitset bbs(5);
                auto s = bbs.to_string();

                bbs.set(0);
                s = bbs.to_string();
                bbs.set(2);
                s = bbs.to_string();

                auto b = bbs[3];
                s = bbs.to_string();
                rc = b == 1;
                b = 1;
                s = bbs.to_string();
                rc = b == 1;
                b.flip();
                s = bbs.to_string();

                bbs[4] = 1;
                s = bbs.to_string();

                bbs.set();
                s = bbs.to_string();

                bbs[1] = bbs[5];

                bitset bsmv = bbs;
                Assert::IsTrue(bsmv == bbs);
                bitset bsmvn(std::move(bsmv));
                Assert::IsTrue(bsmvn == bbs);
                bbs = std::move(bsmvn);
            }

            TEST_METHOD(CreateBitsetLong)
            {
                const int n = 5073;

                std::vector<int> rands;

                std::srand((unsigned int)std::time(nullptr));

                for(auto k = 0; k < n; k++)
                {
                    rands.emplace_back(std::rand() % n);
                    bitset bbs(n);
                }

                std::bitset<n> bs0;
                bitset bbs(n);

                for(auto e : rands)
                {
                    bs0.set(e);
                    bbs.set(e);
                }

                for(auto e : rands)
                {
                    {
                        auto bt = bs0[e];
                        bt = 1;
                        bt.flip();
                    }
                    {
                        auto bt = bbs[e];
                        bt = 1;
                        bt.flip();
                    }
                }

                for(auto e : rands)
                {
                    bs0[e] = 1;
                    bbs[e] = 1;
                }

                for(auto e : rands)
                {
                    bs0.reset(e);
                    bbs.reset(e);
                }

                for(auto e : rands)
                {
                    bs0.set(e);
                    bbs.set(e);
                }

                bs0.flip();
                bbs.flip();

                auto s0 = bs0.to_string<wchar_t>();
                auto s = bbs.to_string();
                bool rc = s0 == s;
                Assert::IsTrue(rc);

                bitset bbscp(bbs);
                s = bbs.to_string();
                rc = s0 == s;
                Assert::IsTrue(rc);
                Assert::IsTrue(bbs == bbscp);

                bitset bbsopcp;
                bbsopcp = bbs;
                s = bbs.to_string();
                rc = s0 == s;
                Assert::IsTrue(rc);

                bs0.set();
                bbs.set();
                s0 = bs0.to_string<wchar_t>();
                s = bbs.to_string();
                rc = s0 == s;
                Assert::IsTrue(rc);

                bitset bsmv = bbs;
                Assert::IsTrue(bsmv == bbs);
                bitset bsmvn(std::move(bsmv));
                Assert::IsTrue(bsmvn == bbs);
                bbs = std::move(bsmvn);
            }

            TEST_METHOD(CreateBitsetLong32)
            {
                const int n = 5007;

                std::vector<int> rands;

                std::srand((unsigned int)std::time(nullptr));

                for(auto k = 0; k < n; k++)
                {
                    rands.emplace_back(std::rand() % n);
                }

                std::bitset<n> bs0;
                bitset<uint32_t> bbs(n);

                for(auto e : rands)
                {
                    bs0.set(e);
                    bbs.set(e);
                }

                for(auto e : rands)
                {
                    {
                        auto bt = bs0[e];
                        bt = 1;
                        bt.flip();
                    }
                    {
                        auto bt = bbs[e];
                        bt = 1;
                        bt.flip();
                    }
                }

                for(auto e : rands)
                {
                    bs0[e] = 1;
                    bbs[e] = 1;
                }

                for(auto e : rands)
                {
                    bs0.reset(e);
                    bbs.reset(e);
                }

                for(auto e : rands)
                {
                    bs0.set(e);
                    bbs.set(e);
                }

                bs0.flip();
                bbs.flip();

                auto s0 = bs0.to_string<wchar_t>();
                auto s = bbs.to_string();

                bool rc = s0 == s;
                
                Assert::IsTrue(rc);

                bitset bbscp(bbs);
                s = bbs.to_string();
                rc = s0 == s;

                Assert::IsTrue(rc);

                bitset<uint32_t> bbsopcp;
                bbsopcp = bbs;
                s = bbs.to_string();
                rc = s0 == s;

                Assert::IsTrue(rc);

                bs0.set();
                bbs.set();
                s0 = bs0.to_string<wchar_t>();
                s = bbs.to_string();
                rc = s0 == s;
                Assert::IsTrue(rc);

                bitset bsmv = bbs;
                Assert::IsTrue(bsmv == bbs);
                bitset bsmvn(std::move(bsmv));
                Assert::IsTrue(bsmvn == bbs);
                bbs = std::move(bsmvn);
            }

            TEST_METHOD(CreateBitsetLong16)
            {
                const int n = 5001;

                std::vector<int> rands;

                std::srand((unsigned int)std::time(nullptr));

                for(auto k = 0; k < n; k++)
                {
                    rands.emplace_back(std::rand() % n);
                }

                std::bitset<n> bs0;
                bitset<uint16_t> bbs(n);

                for(auto e : rands)
                {
                    bs0.set(e);
                    bbs.set(e);
                }

                for(auto e : rands)
                {
                    {
                        auto bt = bs0[e];
                        bt = 1;
                        bt.flip();
                    }
                    {
                        auto bt = bbs[e];
                        bt = 1;
                        bt.flip();
                    }
                }

                for(auto e : rands)
                {
                    bs0[e] = 1;
                    bbs[e] = 1;
                }

                for(auto e : rands)
                {
                    bs0.reset(e);
                    bbs.reset(e);
                }

                for(auto e : rands)
                {
                    bs0.set(e);
                    bbs.set(e);
                }

                bs0.flip();
                bbs.flip();

                auto s0 = bs0.to_string<wchar_t>();
                auto s = bbs.to_string();

                bool rc = s0 == s;
                
                Assert::IsTrue(rc);

                bitset bbscp(bbs);
                s = bbs.to_string();
                rc = s0 == s;

                Assert::IsTrue(rc);

                bitset<uint16_t> bbsopcp;
                bbsopcp = bbs;
                s = bbs.to_string();
                rc = s0 == s;

                Assert::IsTrue(rc);

                bs0.set();
                bbs.set();
                s0 = bs0.to_string<wchar_t>();
                s = bbs.to_string();
                rc = s0 == s;
                Assert::IsTrue(rc);

                bitset bsmv = bbs;
                Assert::IsTrue(bsmv == bbs);
                bitset bsmvn(std::move(bsmv));
                Assert::IsTrue(bsmvn == bbs);
                bbs = std::move(bsmvn);
            }

            TEST_METHOD(CreateBitsetLong8)
            {
                const int n = 50001;

                std::vector<int> rands;

                std::srand((unsigned int)std::time(nullptr));

                for(auto k = 0; k < n; k++)
                {
                    rands.emplace_back(std::rand() % n);
                }

                std::bitset<n> bs0;
                bitset<uint8_t> bbs(n);

                for(auto e : rands)
                {
                    bs0.set(e);
                    bbs.set(e);
                }

                for(auto e : rands)
                {
                    {
                        auto bt = bs0[e];
                        bt = 1;
                        bt.flip();
                    }
                    {
                        auto bt = bbs[e];
                        bt = 1;
                        bt.flip();
                    }
                }

                for(auto e : rands)
                {
                    bs0[e] = 1;
                    bbs[e] = 1;
                }

                for(auto e : rands)
                {
                    bs0.reset(e);
                    bbs.reset(e);
                }

                for(auto e : rands)
                {
                    bs0.set(e);
                    bbs.set(e);
                }

                bs0.flip();
                bbs.flip();

                auto s0 = bs0.to_string<wchar_t>();
                auto s = bbs.to_string();

                bool rc = s0 == s;
                
                Assert::IsTrue(rc);

                bitset bbscp(bbs);
                s = bbs.to_string();
                rc = s0 == s;

                Assert::IsTrue(rc);

                bitset<uint8_t> bbsopcp;
                bbsopcp = bbs;
                s = bbs.to_string();
                rc = s0 == s;

                Assert::IsTrue(rc);

                bs0.set();
                bbs.set();
                s0 = bs0.to_string<wchar_t>();
                s = bbs.to_string();
                rc = s0 == s;
                Assert::IsTrue(rc);

                bitset bsmv = bbs;
                Assert::IsTrue(bsmv == bbs);
                bitset bsmvn(std::move(bsmv));
                Assert::IsTrue(bsmvn == bbs);
                bbs = std::move(bsmvn);
            }

            TEST_METHOD(FindFirstBitset)
            {
                const int n = 50001;

                std::vector<int> rands;

                std::srand((unsigned int)std::time(nullptr));

                for(auto k = 0; k < n; k++)
                {
                    rands.emplace_back(std::rand() % n);
                }

                bitset<> bbs(n);

                for(auto e : rands)
                {
                    bbs.reset();
                    bbs[e] = 1;
                    auto pos = bbs.find_first();
                    Assert::IsTrue(pos == e, (std::to_wstring(pos) + L":" + std::to_wstring(e)).c_str());
                }

                bbs.reset();
                auto pos = bbs.find_first();
                Assert::IsTrue(pos == bitset<>::npos);

                bbs[0] = 1;
                auto s = bbs.to_string();
                pos = bbs.find_first();
                Assert::IsTrue(pos == 0);

                bbs.reset();
                bbs[bbs.size() - 1] = 1;
                s = bbs.to_string();
                pos = bbs.find_first();
                Assert::IsTrue(pos == bbs.size() - 1);
            }
            
            TEST_METHOD(FindNextBitset)
            {
                using data_type = uint64_t;

                const int n = 50701;

                bitset<data_type> bbs(n);

                std::vector<int> rands;

                std::srand((unsigned int)std::time(nullptr));

                for(auto k = 0; k < n; k++)
                {
                    auto e = std::rand() % n;
                    rands.emplace_back(e);
                    bbs[e] = 1;
                }

                std::sort(rands.begin(), rands.end());
                rands.erase(std::unique(rands.begin(), rands.end() ), rands.end());

                auto pos = bbs.find_first();
                auto s = bbs.to_string();

                for(auto k = 1; k < rands.size(); k++)
                {
                    auto e = rands[k];
                    pos = bbs.find_next(pos);
                    if(k < n - 1)
                        Assert::IsTrue(pos == e, (std::to_wstring(pos) + L":" + std::to_wstring(e)).c_str());
                    else
                        Assert::IsTrue(pos == bitset<data_type>::npos, (std::to_wstring(pos) + L"*" + std::to_wstring(e)).c_str());
                }

                bbs.reset();
                pos = bbs.find_first();
                Assert::IsTrue(pos == bitset<data_type>::npos);
                pos = bbs.find_next(pos);
                Assert::IsTrue(pos == bitset<data_type>::npos);

                bbs[0] = 1;
                s = bbs.to_string();
                pos = bbs.find_first();
                Assert::IsTrue(pos == 0);
                pos = bbs.find_next(pos);
                Assert::IsTrue(pos == bitset<data_type>::npos);

                bbs.reset();
                bbs[bbs.size() - 1] = 1;
                s = bbs.to_string();
                pos = bbs.find_first();
                Assert::IsTrue(pos == bbs.size() - 1);
                pos = bbs.find_next(pos);
                Assert::IsTrue(pos == bitset<data_type>::npos);
            }

            TEST_METHOD(GraphAlgorithmsDfsToVectorTiming)
            {
                const int n = 5001;

                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                (*gr).root() = *(*gr).add_vertex(factory::create<dominator_vertex>(0)).first;

                std::srand((unsigned int)std::time(nullptr));

                for(auto k = 0; k < n; k += 2)
                {
                    const auto& v1 = *(*gr).add_vertex(factory::create<dominator_vertex>(std::rand() % n)).first;
                    const auto& v2 = *(*gr).add_vertex(factory::create<dominator_vertex>(std::rand() % n)).first;

                    (*gr).add_edge(v1, v2, 0.1);

                    (*gr).add_edge((*gr).root(), v1, 0.1);
                    (*gr).add_edge((*gr).root(), v2, 0.1);
                }

                std::vector<std::shared_ptr<dominator_vertex>> result;

                qpf_timer timer;
                
                auto start = std::chrono::high_resolution_clock::now();

                timer.start();

                graph_algorithms<dominator_vertex>::dfs_preorder_to_vector(gr, result);

                timer.stop();

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                string_type elapsed_time = timer.elapsed_time_as_string();
                string_type total_elapsed_time = timer.total_elapsed_time_as_string();

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
                Logger::WriteMessage("\n");
                Logger::WriteMessage(elapsed_time.c_str());
                Logger::WriteMessage("\n");
                Logger::WriteMessage(total_elapsed_time.c_str());

                Assert::AreEqual((*gr).vertices().size(), result.size());
            }

            TEST_METHOD(GraphAlgorithmsDfsToVector)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& v1 = *(*gr).add_vertex(factory::create<dominator_vertex>(1)).first;
                const auto& v2 = *(*gr).add_vertex(factory::create<dominator_vertex>(2)).first;
                const auto& v3 = *(*gr).add_vertex(factory::create<dominator_vertex>(3)).first;
                const auto& v4 = *(*gr).add_vertex(factory::create<dominator_vertex>(4)).first;
                const auto& v5 = *(*gr).add_vertex(factory::create<dominator_vertex>(5)).first;

                (*gr).add_edge(v1, v2, 0.1);
                (*gr).add_edge(v1, v3, 0.2);
                (*gr).add_edge(v2, v4, 0.3);
                (*gr).add_edge(v2, v5, 0.3);

                (*gr).root() = v1;

                std::vector<std::shared_ptr<dominator_vertex>> result;

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::dfs_preorder_to_vector(gr, result);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsDfsToVector.dot)", false);

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
                Logger::WriteMessage("\n");

                Assert::AreEqual((*gr).vertices().size(), result.size());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsNaiveMuchnik)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& entry = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"entry")).first;
                const auto& b1 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"B1")).first;
                const auto& b2 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"B2")).first;
                const auto& b3 = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"B3")).first;
                const auto& b4 = *(*gr).add_vertex(factory::create<dominator_vertex>(14, L"B4")).first;
                const auto& b5 = *(*gr).add_vertex(factory::create<dominator_vertex>(15, L"B5")).first;
                const auto& b6 = *(*gr).add_vertex(factory::create<dominator_vertex>(16, L"B6")).first;
                const auto& exit = *(*gr).add_vertex(factory::create<dominator_vertex>(100, L"exit")).first;

                (*gr).add_edge(entry, b1, 0.1);

                (*gr).add_edge(b1, b2, 0.2);
                (*gr).add_edge(b1, b3, 0.2);

                (*gr).add_edge(b2, exit, 0.3);

                (*gr).add_edge(b3, b4, 0.4);

                (*gr).add_edge(b4, b5, 0.5);
                (*gr).add_edge(b4, b6, 0.5);

                (*gr).add_edge(b5, exit, 0.6);

                (*gr).add_edge(b6, b4, 0.7);

                (*gr).root() = entry;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsNaiveMuchnik.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsNaiveLengauerTarjan)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& r = *(*gr).add_vertex(factory::create<dominator_vertex>(1,  L"R")).first;
                const auto& a = *(*gr).add_vertex(factory::create<dominator_vertex>(2,  L"A")).first;
                const auto& b = *(*gr).add_vertex(factory::create<dominator_vertex>(3,  L"B")).first;
                const auto& c = *(*gr).add_vertex(factory::create<dominator_vertex>(4,  L"C")).first;
                const auto& d = *(*gr).add_vertex(factory::create<dominator_vertex>(5,  L"D")).first;
                const auto& e = *(*gr).add_vertex(factory::create<dominator_vertex>(6,  L"E")).first;
                const auto& f = *(*gr).add_vertex(factory::create<dominator_vertex>(7,  L"F")).first;
                const auto& g = *(*gr).add_vertex(factory::create<dominator_vertex>(8,  L"G")).first;
                const auto& h = *(*gr).add_vertex(factory::create<dominator_vertex>(9,  L"H")).first;
                const auto& i = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"I")).first;
                const auto& j = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"J")).first;
                const auto& k = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"K")).first;
                const auto& l = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"L")).first;

                (*gr).add_edge(r, a, 0.1);
                (*gr).add_edge(r, b, 0.1);
                (*gr).add_edge(r, c, 0.1);

                (*gr).add_edge(a, d, 0.1);

                (*gr).add_edge(b, a, 0.1);
                (*gr).add_edge(b, d, 0.1);
                (*gr).add_edge(b, e, 0.1);

                (*gr).add_edge(c, f, 0.1);
                (*gr).add_edge(c, g, 0.1);

                (*gr).add_edge(d, l, 0.1);

                (*gr).add_edge(e, h, 0.1);

                (*gr).add_edge(f, i, 0.1);

                (*gr).add_edge(g, i, 0.1);
                (*gr).add_edge(g, j, 0.1);

                (*gr).add_edge(h, e, 0.1);
                (*gr).add_edge(h, k, 0.1);

                (*gr).add_edge(i, k, 0.1);

                (*gr).add_edge(j, i, 0.1);

                (*gr).add_edge(k, r, 0.1);
                (*gr).add_edge(k, i, 0.1);

                (*gr).add_edge(l, h, 0.1);

                (*gr).root() = r;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsNaiveLengauerTarjan.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsNaiveAppel)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(1,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(2,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(3,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(4,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(5,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(6,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(7,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(8,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(9,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"12")).first;

                (*gr).add_edge(n1, n2, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n4, 0.1);

                (*gr).add_edge(n3, n2, 0.1);

                (*gr).add_edge(n4, n2, 0.1);
                (*gr).add_edge(n4, n5, 0.1);
                (*gr).add_edge(n4, n6, 0.1);

                (*gr).add_edge(n5, n7, 0.1);
                (*gr).add_edge(n5, n8, 0.1);

                (*gr).add_edge(n6, n7, 0.1);

                (*gr).add_edge(n7, n11, 0.1);

                (*gr).add_edge(n8, n9, 0.1);

                (*gr).add_edge(n9, n8, 0.1);
                (*gr).add_edge(n9, n10, 0.1);

                (*gr).add_edge(n10, n5, 0.1);
                (*gr).add_edge(n10, n12, 0.1);

                (*gr).add_edge(n11, n12, 0.1);

                (*gr).root() = n1;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsNaiveAppel.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsNaiveCharlesFischer)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(1,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(2,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(3,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(4,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(5,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(6,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(7,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(8,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(9,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"12")).first;
                const auto& n13 = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"13")).first;

                (*gr).add_edge(n1, n2, 0.1);
                (*gr).add_edge(n1, n3, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n4, 0.1);
                (*gr).add_edge(n2, n11, 0.1);

                (*gr).add_edge(n3, n4, 0.1);
                (*gr).add_edge(n3, n9, 0.1);

                (*gr).add_edge(n4, n5, 0.1);
                (*gr).add_edge(n4, n8, 0.1);

                (*gr).add_edge(n5, n3, 0.1);
                (*gr).add_edge(n5, n6, 0.1);

                (*gr).add_edge(n6, n5, 0.1);
                (*gr).add_edge(n6, n7, 0.1);

                (*gr).add_edge(n8, n6, 0.1);

                (*gr).add_edge(n9, n8, 0.1);
                (*gr).add_edge(n9, n10, 0.1);

                (*gr).add_edge(n10, n6, 0.1);

                (*gr).add_edge(n11, n12, 0.1);
                (*gr).add_edge(n11, n13, 0.1);

                (*gr).add_edge(n12, n7, 0.1);
                (*gr).add_edge(n12, n11, 0.1);

                (*gr).add_edge(n13, n12, 0.1);

                (*gr).root() = n1;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsNaiveCharlesFischer.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsNaiveCytron)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& en  = *(*gr).add_vertex(factory::create<dominator_vertex>(10,  L"Entry")).first;
                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(11,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(12,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(13,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(14,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(15,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(16,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(17,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(18,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(19,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(20,  L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(21,  L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(22,  L"12")).first;
                const auto& ex  = *(*gr).add_vertex(factory::create<dominator_vertex>(110, L"Exit")).first;

                (*gr).add_edge(en, n1, 0.1);
                (*gr).add_edge(en, ex, 0.1);

                (*gr).add_edge(n1, n2, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n7, 0.1);

                (*gr).add_edge(n3, n4, 0.1);
                (*gr).add_edge(n3, n5, 0.1);

                (*gr).add_edge(n4, n6, 0.1);

                (*gr).add_edge(n5, n6, 0.1);

                (*gr).add_edge(n6, n8, 0.1);

                (*gr).add_edge(n7, n8, 0.1);

                (*gr).add_edge(n8, n9, 0.1);

                (*gr).add_edge(n9, n10, 0.1);
                (*gr).add_edge(n9, n11, 0.1);

                (*gr).add_edge(n10, n11, 0.1);

                (*gr).add_edge(n11, n9, 0.1);
                (*gr).add_edge(n11, n12, 0.1);

                (*gr).add_edge(n12, n2, 0.1);
                (*gr).add_edge(n12, ex, 0.1);

                (*gr).root() = en;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsNaiveCytron.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                std::unordered_map<string_type, string_type> idoms;
                for(auto& vertex : (*gr).vertices())
                {
                    if((*vertex).idominator() != nullptr)
                        idoms[(*vertex).label()] = (*(*vertex).idominator()).label();
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsBuildDominanceTreeNaiveMuchnik)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& entry = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"entry")).first;
                const auto& b1 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"B1")).first;
                const auto& b2 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"B2")).first;
                const auto& b3 = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"B3")).first;
                const auto& b4 = *(*gr).add_vertex(factory::create<dominator_vertex>(14, L"B4")).first;
                const auto& b5 = *(*gr).add_vertex(factory::create<dominator_vertex>(15, L"B5")).first;
                const auto& b6 = *(*gr).add_vertex(factory::create<dominator_vertex>(16, L"B6")).first;
                const auto& exit = *(*gr).add_vertex(factory::create<dominator_vertex>(100, L"exit")).first;

                (*gr).add_edge(entry, b1, 0.1);

                (*gr).add_edge(b1, b2, 0.2);
                (*gr).add_edge(b1, b3, 0.2);

                (*gr).add_edge(b2, exit, 0.3);

                (*gr).add_edge(b3, b4, 0.4);

                (*gr).add_edge(b4, b5, 0.5);
                (*gr).add_edge(b4, b6, 0.5);

                (*gr).add_edge(b5, exit, 0.6);

                (*gr).add_edge(b6, b4, 0.7);

                (*gr).root() = entry;

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                std::shared_ptr<dominance_tree> tree;

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::build_dominance_tree(gr, tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(tree, LR"(d:\tmp\GraphAlgorithmsBuildDominanceTreeNaiveMuchnik.Tree.dot)");

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsBuildDominanceTreeNaiveLengauerTarjan)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& r = *(*gr).add_vertex(factory::create<dominator_vertex>(1,  L"R")).first;
                const auto& a = *(*gr).add_vertex(factory::create<dominator_vertex>(2,  L"A")).first;
                const auto& b = *(*gr).add_vertex(factory::create<dominator_vertex>(3,  L"B")).first;
                const auto& c = *(*gr).add_vertex(factory::create<dominator_vertex>(4,  L"C")).first;
                const auto& d = *(*gr).add_vertex(factory::create<dominator_vertex>(5,  L"D")).first;
                const auto& e = *(*gr).add_vertex(factory::create<dominator_vertex>(6,  L"E")).first;
                const auto& f = *(*gr).add_vertex(factory::create<dominator_vertex>(7,  L"F")).first;
                const auto& g = *(*gr).add_vertex(factory::create<dominator_vertex>(8,  L"G")).first;
                const auto& h = *(*gr).add_vertex(factory::create<dominator_vertex>(9,  L"H")).first;
                const auto& i = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"I")).first;
                const auto& j = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"J")).first;
                const auto& k = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"K")).first;
                const auto& l = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"L")).first;

                (*gr).add_edge(r, a, 0.1);
                (*gr).add_edge(r, b, 0.1);
                (*gr).add_edge(r, c, 0.1);

                (*gr).add_edge(a, d, 0.1);

                (*gr).add_edge(b, a, 0.1);
                (*gr).add_edge(b, d, 0.1);
                (*gr).add_edge(b, e, 0.1);

                (*gr).add_edge(c, f, 0.1);
                (*gr).add_edge(c, g, 0.1);

                (*gr).add_edge(d, l, 0.1);

                (*gr).add_edge(e, h, 0.1);

                (*gr).add_edge(f, i, 0.1);

                (*gr).add_edge(g, i, 0.1);
                (*gr).add_edge(g, j, 0.1);

                (*gr).add_edge(h, e, 0.1);
                (*gr).add_edge(h, k, 0.1);

                (*gr).add_edge(i, k, 0.1);

                (*gr).add_edge(j, i, 0.1);

                (*gr).add_edge(k, r, 0.1);
                (*gr).add_edge(k, i, 0.1);

                (*gr).add_edge(l, h, 0.1);

                (*gr).root() = r;

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                std::shared_ptr<dominance_tree> tree;

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::build_dominance_tree(gr, tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(tree, LR"(d:\tmp\GraphAlgorithmsDominanceTreeNaiveLengauerTarjan.Tree.dot)");

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsBuildDominanceTreeNaiveAppel)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(1,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(2,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(3,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(4,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(5,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(6,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(7,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(8,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(9,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"12")).first;

                (*gr).add_edge(n1, n2, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n4, 0.1);

                (*gr).add_edge(n3, n2, 0.1);

                (*gr).add_edge(n4, n2, 0.1);
                (*gr).add_edge(n4, n5, 0.1);
                (*gr).add_edge(n4, n6, 0.1);

                (*gr).add_edge(n5, n7, 0.1);
                (*gr).add_edge(n5, n8, 0.1);

                (*gr).add_edge(n6, n7, 0.1);

                (*gr).add_edge(n7, n11, 0.1);

                (*gr).add_edge(n8, n9, 0.1);

                (*gr).add_edge(n9, n8, 0.1);
                (*gr).add_edge(n9, n10, 0.1);

                (*gr).add_edge(n10, n5, 0.1);
                (*gr).add_edge(n10, n12, 0.1);

                (*gr).add_edge(n11, n12, 0.1);

                (*gr).root() = n1;

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                std::shared_ptr<dominance_tree> tree;

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::build_dominance_tree(gr, tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(tree, LR"(d:\tmp\GraphAlgorithmsDominanceTreeNaiveAppel.Tree.dot)");

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsBuildDominanceTreeNaiveCharlesFischer)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(1,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(2,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(3,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(4,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(5,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(6,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(7,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(8,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(9,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"12")).first;
                const auto& n13 = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"13")).first;

                (*gr).add_edge(n1, n2, 0.1);
                (*gr).add_edge(n1, n3, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n4, 0.1);
                (*gr).add_edge(n2, n11, 0.1);

                (*gr).add_edge(n3, n4, 0.1);
                (*gr).add_edge(n3, n9, 0.1);

                (*gr).add_edge(n4, n5, 0.1);
                (*gr).add_edge(n4, n8, 0.1);

                (*gr).add_edge(n5, n3, 0.1);
                (*gr).add_edge(n5, n6, 0.1);

                (*gr).add_edge(n6, n5, 0.1);
                (*gr).add_edge(n6, n7, 0.1);

                (*gr).add_edge(n8, n6, 0.1);

                (*gr).add_edge(n9, n8, 0.1);
                (*gr).add_edge(n9, n10, 0.1);

                (*gr).add_edge(n10, n6, 0.1);

                (*gr).add_edge(n11, n12, 0.1);
                (*gr).add_edge(n11, n13, 0.1);

                (*gr).add_edge(n12, n7, 0.1);
                (*gr).add_edge(n12, n11, 0.1);

                (*gr).add_edge(n13, n12, 0.1);

                (*gr).root() = n1;

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                std::shared_ptr<dominance_tree> tree;

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::build_dominance_tree(gr, tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(tree, LR"(d:\tmp\GraphAlgorithmsDominanceTreeNaiveCharlesFischer.Tree.dot)");

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsBuildDominanceTreeNaiveCytron)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& en  = *(*gr).add_vertex(factory::create<dominator_vertex>(10,  L"Entry")).first;
                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(11,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(12,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(13,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(14,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(15,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(16,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(17,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(18,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(19,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(20,  L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(21,  L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(22,  L"12")).first;
                const auto& ex  = *(*gr).add_vertex(factory::create<dominator_vertex>(110, L"Exit")).first;

                (*gr).add_edge(en, n1, 0.1);
                (*gr).add_edge(en, ex, 0.1);

                (*gr).add_edge(n1, n2, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n7, 0.1);

                (*gr).add_edge(n3, n4, 0.1);
                (*gr).add_edge(n3, n5, 0.1);

                (*gr).add_edge(n4, n6, 0.1);

                (*gr).add_edge(n5, n6, 0.1);

                (*gr).add_edge(n6, n8, 0.1);

                (*gr).add_edge(n7, n8, 0.1);

                (*gr).add_edge(n8, n9, 0.1);

                (*gr).add_edge(n9, n10, 0.1);
                (*gr).add_edge(n9, n11, 0.1);

                (*gr).add_edge(n10, n11, 0.1);

                (*gr).add_edge(n11, n9, 0.1);
                (*gr).add_edge(n11, n12, 0.1);

                (*gr).add_edge(n12, n2, 0.1);
                (*gr).add_edge(n12, ex, 0.1);

                (*gr).root() = en;

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                std::shared_ptr<dominance_tree> tree;

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::build_dominance_tree(gr, tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(tree, LR"(d:\tmp\GraphAlgorithmsDominanceTreeNaiveCytron.Tree.dot)");

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsPostOrderToVectorNaiveMuchnik)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& entry = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"entry")).first;
                const auto& b1 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"B1")).first;
                const auto& b2 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"B2")).first;
                const auto& b3 = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"B3")).first;
                const auto& b4 = *(*gr).add_vertex(factory::create<dominator_vertex>(14, L"B4")).first;
                const auto& b5 = *(*gr).add_vertex(factory::create<dominator_vertex>(15, L"B5")).first;
                const auto& b6 = *(*gr).add_vertex(factory::create<dominator_vertex>(16, L"B6")).first;
                const auto& exit = *(*gr).add_vertex(factory::create<dominator_vertex>(100, L"exit")).first;

                (*gr).add_edge(entry, b1, 0.1);

                (*gr).add_edge(b1, b2, 0.2);
                (*gr).add_edge(b1, b3, 0.2);

                (*gr).add_edge(b2, exit, 0.3);

                (*gr).add_edge(b3, b4, 0.4);

                (*gr).add_edge(b4, b5, 0.5);
                (*gr).add_edge(b4, b6, 0.5);

                (*gr).add_edge(b5, exit, 0.6);

                (*gr).add_edge(b6, b4, 0.7);

                (*gr).root() = entry;

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                std::shared_ptr<dominance_tree> tree;

                graph_algorithms<dominator_vertex>::build_dominance_tree(gr, tree);

                std::vector<std::shared_ptr<dominance_tree>> result;

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::dfs_postorder_to_vector(tree, result);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominanceFrontierNaiveCytron)
            {
                std::shared_ptr<graph<dominator_vertex>> graph(factory::create<graph<dominator_vertex>>());

                const auto& en  = *(*graph).add_vertex(factory::create<dominator_vertex>(10,  L"Entry")).first;
                const auto& n1  = *(*graph).add_vertex(factory::create<dominator_vertex>(11,  L"1")).first;
                const auto& n2  = *(*graph).add_vertex(factory::create<dominator_vertex>(12,  L"2")).first;
                const auto& n3  = *(*graph).add_vertex(factory::create<dominator_vertex>(13,  L"3")).first;
                const auto& n4  = *(*graph).add_vertex(factory::create<dominator_vertex>(14,  L"4")).first;
                const auto& n5  = *(*graph).add_vertex(factory::create<dominator_vertex>(15,  L"5")).first;
                const auto& n6  = *(*graph).add_vertex(factory::create<dominator_vertex>(16,  L"6")).first;
                const auto& n7  = *(*graph).add_vertex(factory::create<dominator_vertex>(17,  L"7")).first;
                const auto& n8  = *(*graph).add_vertex(factory::create<dominator_vertex>(18,  L"8")).first;
                const auto& n9  = *(*graph).add_vertex(factory::create<dominator_vertex>(19,  L"9")).first;
                const auto& n10 = *(*graph).add_vertex(factory::create<dominator_vertex>(20,  L"10")).first;
                const auto& n11 = *(*graph).add_vertex(factory::create<dominator_vertex>(21,  L"11")).first;
                const auto& n12 = *(*graph).add_vertex(factory::create<dominator_vertex>(22,  L"12")).first;
                const auto& ex  = *(*graph).add_vertex(factory::create<dominator_vertex>(110, L"Exit")).first;

                (*graph).add_edge(en, n1, 0.1);
                (*graph).add_edge(en, ex, 0.1);

                (*graph).add_edge(n1, n2, 0.1);

                (*graph).add_edge(n2, n3, 0.1);
                (*graph).add_edge(n2, n7, 0.1);

                (*graph).add_edge(n3, n4, 0.1);
                (*graph).add_edge(n3, n5, 0.1);

                (*graph).add_edge(n4, n6, 0.1);

                (*graph).add_edge(n5, n6, 0.1);

                (*graph).add_edge(n6, n8, 0.1);

                (*graph).add_edge(n7, n8, 0.1);

                (*graph).add_edge(n8, n9, 0.1);

                (*graph).add_edge(n9, n10, 0.1);
                (*graph).add_edge(n9, n11, 0.1);

                (*graph).add_edge(n10, n11, 0.1);

                (*graph).add_edge(n11, n9, 0.1);
                (*graph).add_edge(n11, n12, 0.1);

                (*graph).add_edge(n12, n2, 0.1);
                (*graph).add_edge(n12, ex, 0.1);

                (*graph).root() = en;

                graph_algorithms<dominator_vertex>::compute_dominators(graph);

                std::shared_ptr<dominance_tree> dominance_tree;

                graph_algorithms<dominator_vertex>::build_dominance_tree(graph, dominance_tree);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominance_frontiers(graph, dominance_tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(graph, LR"(d:\tmp\GraphAlgorithmsComputeDominanceFrontierNaiveCytron.Graph.dot)", false);
                graph_algorithms<dominator_vertex>::generate_graphviz_file(dominance_tree, LR"(d:\tmp\GraphAlgorithmsComputeDominanceFrontierNaiveCytron.Tree.dot)");

                std::map<string_type, std::vector<string_type>> frontiers;
                for(auto& vertex : (*graph).vertices())
                {
                    std::vector<string_type> frontier;
                    for(auto& f : (*vertex).frontiers())
                        frontier.push_back((*f).label());
                    frontiers[(*vertex).label()] = frontier;
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominanceFrontierNaiveMuchnik)
            {
                // Fig. 8.21
                std::shared_ptr<graph<dominator_vertex>> graph(factory::create<graph<dominator_vertex>>());

                const auto& entry = *(*graph).add_vertex(factory::create<dominator_vertex>(10, L"entry")).first;
                const auto& b1 = *(*graph).add_vertex(factory::create<dominator_vertex>(11, L"B1")).first;
                const auto& b2 = *(*graph).add_vertex(factory::create<dominator_vertex>(12, L"B2")).first;
                const auto& b3 = *(*graph).add_vertex(factory::create<dominator_vertex>(13, L"B3")).first;
                const auto& b4 = *(*graph).add_vertex(factory::create<dominator_vertex>(14, L"B4")).first;
                const auto& b5 = *(*graph).add_vertex(factory::create<dominator_vertex>(15, L"B5")).first;
                const auto& b6 = *(*graph).add_vertex(factory::create<dominator_vertex>(16, L"B6")).first;
                const auto& exit = *(*graph).add_vertex(factory::create<dominator_vertex>(100, L"exit")).first;

                (*graph).add_edge(entry, b1, 0.1);

                (*graph).add_edge(b1, b2, 0.2);

                (*graph).add_edge(b2, b3, 0.3);
                (*graph).add_edge(b2, b4, 0.3);

                (*graph).add_edge(b3, b2, 0.4);

                (*graph).add_edge(b4, b5, 0.5);
                (*graph).add_edge(b4, b6, 0.5);

                (*graph).add_edge(b5, exit, 0.6);

                (*graph).add_edge(b6, exit, 0.7);

                (*graph).root() = entry;

                graph_algorithms<dominator_vertex>::compute_dominators(graph);

                std::shared_ptr<dominance_tree> dominance_tree;

                graph_algorithms<dominator_vertex>::build_dominance_tree(graph, dominance_tree);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominance_frontiers(graph, dominance_tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(graph, LR"(d:\tmp\GraphAlgorithmsComputeDominanceFrontierNaiveMuchnik.Graph.dot)", false);
                graph_algorithms<dominator_vertex>::generate_graphviz_file(dominance_tree, LR"(d:\tmp\GraphAlgorithmsComputeDominanceFrontierNaiveMuchnik.Tree.dot)");

                std::map<string_type, std::vector<string_type>> frontiers;
                for(auto& vertex : (*graph).vertices())
                {
                    std::vector<string_type> frontier;
                    for(auto& f : (*vertex).frontiers())
                        frontier.push_back((*f).label());
                    frontiers[(*vertex).label()] = frontier;
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominanceFrontierNaiveAppel)
            {
                // FIGURE 19.4
                std::shared_ptr<graph<dominator_vertex>> graph(factory::create<graph<dominator_vertex>>());

                const auto& n1 = *(*graph).add_vertex(factory::create<dominator_vertex>(1, L"1")).first;
                const auto& n2 = *(*graph).add_vertex(factory::create<dominator_vertex>(2, L"2")).first;
                const auto& n3 = *(*graph).add_vertex(factory::create<dominator_vertex>(3, L"3")).first;
                const auto& n4 = *(*graph).add_vertex(factory::create<dominator_vertex>(4, L"4")).first;
                const auto& n5 = *(*graph).add_vertex(factory::create<dominator_vertex>(5, L"5")).first;
                const auto& n6 = *(*graph).add_vertex(factory::create<dominator_vertex>(6, L"6")).first;
                const auto& n7 = *(*graph).add_vertex(factory::create<dominator_vertex>(7, L"7")).first;

                (*graph).add_edge(n1, n2, 0.1);

                (*graph).add_edge(n2, n3, 0.1);
                (*graph).add_edge(n2, n4, 0.1);

                (*graph).add_edge(n3, n5, 0.1);
                (*graph).add_edge(n3, n6, 0.1);

                (*graph).add_edge(n5, n7, 0.1);

                (*graph).add_edge(n6, n7, 0.1);

                (*graph).add_edge(n7, n2, 0.1);

                (*graph).root() = n1;

                graph_algorithms<dominator_vertex>::compute_dominators(graph);

                std::shared_ptr<dominance_tree> dominance_tree;

                graph_algorithms<dominator_vertex>::build_dominance_tree(graph, dominance_tree);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominance_frontiers(graph, dominance_tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(graph, LR"(d:\tmp\GraphAlgorithmsComputeDominanceFrontierNaiveAppel.Graph.dot)", false);
                graph_algorithms<dominator_vertex>::generate_graphviz_file(dominance_tree, LR"(d:\tmp\GraphAlgorithmsComputeDominanceFrontierNaiveAppel.Tree.dot)");

                std::map<string_type, std::vector<string_type>> frontiers;
                for(auto& vertex : (*graph).vertices())
                {
                    std::vector<string_type> frontier;
                    for(auto& f : (*vertex).frontiers())
                        frontier.push_back((*f).label());
                    frontiers[(*vertex).label()] = frontier;
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominanceFrontierNaiveSlides_hw1)
            {
                std::shared_ptr<graph<dominator_vertex>> graph(factory::create<graph<dominator_vertex>>());

                const auto& a = *(*graph).add_vertex(factory::create<dominator_vertex>(11, L"A")).first;
                const auto& b = *(*graph).add_vertex(factory::create<dominator_vertex>(12, L"B")).first;
                const auto& c = *(*graph).add_vertex(factory::create<dominator_vertex>(13, L"C")).first;
                const auto& d = *(*graph).add_vertex(factory::create<dominator_vertex>(14, L"D")).first;
                const auto& e = *(*graph).add_vertex(factory::create<dominator_vertex>(15, L"E")).first;

                (*graph).add_edge(a, b, 0.2);
                (*graph).add_edge(a, c, 0.2);

                (*graph).add_edge(b, d, 0.3);

                (*graph).add_edge(c, e, 0.4);
                (*graph).add_edge(c, d, 0.4);

                (*graph).add_edge(e, c, 0.7);

                (*graph).root() = a;

                graph_algorithms<dominator_vertex>::compute_dominators(graph);

                std::shared_ptr<dominance_tree> dominance_tree;

                graph_algorithms<dominator_vertex>::build_dominance_tree(graph, dominance_tree);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominance_frontiers(graph, dominance_tree);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                graph_algorithms<dominator_vertex>::generate_graphviz_file(graph, LR"(d:\tmp\GraphAlgorithmsComputeDominanceFrontierNaiveSlides_hw1.Graph.dot)", false);
                graph_algorithms<dominator_vertex>::generate_graphviz_file(dominance_tree, LR"(d:\tmp\GraphAlgorithmsComputeDominanceFrontierNaiveSlides_hw1.Tree.dot)");

                std::map<string_type, std::vector<string_type>> frontiers;
                for(auto& vertex : (*graph).vertices())
                {
                    std::vector<string_type> frontier;
                    for(auto& f : (*vertex).frontiers())
                        frontier.push_back((*f).label());
                    frontiers[(*vertex).label()] = frontier;
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsLengauerTarjan)
            {
                std::shared_ptr<graph<dominator_vertex>> graph(factory::create<graph<dominator_vertex>>());

                const auto& r = *(*graph).add_vertex(factory::create<dominator_vertex>(1,  L"R")).first;
                const auto& a = *(*graph).add_vertex(factory::create<dominator_vertex>(2,  L"A")).first;
                const auto& b = *(*graph).add_vertex(factory::create<dominator_vertex>(3,  L"B")).first;
                const auto& c = *(*graph).add_vertex(factory::create<dominator_vertex>(4,  L"C")).first;
                const auto& d = *(*graph).add_vertex(factory::create<dominator_vertex>(5,  L"D")).first;
                const auto& e = *(*graph).add_vertex(factory::create<dominator_vertex>(6,  L"E")).first;
                const auto& f = *(*graph).add_vertex(factory::create<dominator_vertex>(7,  L"F")).first;
                const auto& g = *(*graph).add_vertex(factory::create<dominator_vertex>(8,  L"G")).first;
                const auto& h = *(*graph).add_vertex(factory::create<dominator_vertex>(9,  L"H")).first;
                const auto& i = *(*graph).add_vertex(factory::create<dominator_vertex>(10, L"I")).first;
                const auto& j = *(*graph).add_vertex(factory::create<dominator_vertex>(11, L"J")).first;
                const auto& k = *(*graph).add_vertex(factory::create<dominator_vertex>(12, L"K")).first;
                const auto& l = *(*graph).add_vertex(factory::create<dominator_vertex>(13, L"L")).first;

                (*graph).add_edge(r, a, 0.1);
                (*graph).add_edge(r, b, 0.1);
                (*graph).add_edge(r, c, 0.1);

                (*graph).add_edge(a, d, 0.1);

                (*graph).add_edge(b, a, 0.1);
                (*graph).add_edge(b, d, 0.1);
                (*graph).add_edge(b, e, 0.1);

                (*graph).add_edge(c, f, 0.1);
                (*graph).add_edge(c, g, 0.1);

                (*graph).add_edge(d, l, 0.1);

                (*graph).add_edge(e, h, 0.1);

                (*graph).add_edge(f, i, 0.1);

                (*graph).add_edge(g, i, 0.1);
                (*graph).add_edge(g, j, 0.1);

                (*graph).add_edge(h, e, 0.1);
                (*graph).add_edge(h, k, 0.1);

                (*graph).add_edge(i, k, 0.1);

                (*graph).add_edge(j, i, 0.1);

                (*graph).add_edge(k, r, 0.1);
                (*graph).add_edge(k, i, 0.1);

                (*graph).add_edge(l, h, 0.1);

                (*graph).root() = r;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(graph, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsLengauerTarjan.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators_lengauer_tarjan(graph);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                std::unordered_map<string_type, std::vector<string_type>> doms;
                std::unordered_map<string_type, string_type> idoms;
                for(auto& vertex : (*graph).vertices())
                {
                    std::vector<string_type> dom;
                    for(auto& d : (*vertex).dominators())
                        dom.push_back((*d).label());
                    doms[(*vertex).label()] = dom;
                    if((*vertex).idominator() != nullptr)
                        idoms[(*vertex).label()] = (*(*vertex).idominator()).label();
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsMuchnik)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& entry = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"entry")).first;
                const auto& b1 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"B1")).first;
                const auto& b2 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"B2")).first;
                const auto& b3 = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"B3")).first;
                const auto& b4 = *(*gr).add_vertex(factory::create<dominator_vertex>(14, L"B4")).first;
                const auto& b5 = *(*gr).add_vertex(factory::create<dominator_vertex>(15, L"B5")).first;
                const auto& b6 = *(*gr).add_vertex(factory::create<dominator_vertex>(16, L"B6")).first;
                const auto& exit = *(*gr).add_vertex(factory::create<dominator_vertex>(100, L"exit")).first;

                (*gr).add_edge(entry, b1, 0.1);

                (*gr).add_edge(b1, b2, 0.2);
                (*gr).add_edge(b1, b3, 0.2);

                (*gr).add_edge(b2, exit, 0.3);

                (*gr).add_edge(b3, b4, 0.4);

                (*gr).add_edge(b4, b5, 0.5);
                (*gr).add_edge(b4, b6, 0.5);

                (*gr).add_edge(b5, exit, 0.6);

                (*gr).add_edge(b6, b4, 0.7);

                (*gr).root() = entry;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsMuchnik.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators_lengauer_tarjan(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                std::unordered_map<string_type, std::vector<string_type>> doms;
                std::unordered_map<string_type, string_type> idoms;
                for(auto& vertex : (*gr).vertices())
                {
                    std::vector<string_type> dom;
                    for(auto& d : (*vertex).dominators())
                        dom.push_back((*d).label());
                    doms[(*vertex).label()] = dom;
                    if((*vertex).idominator() != nullptr)
                        idoms[(*vertex).label()] = (*(*vertex).idominator()).label();
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsAppel)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(1,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(2,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(3,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(4,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(5,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(6,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(7,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(8,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(9,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"12")).first;

                (*gr).add_edge(n1, n2, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n4, 0.1);

                (*gr).add_edge(n3, n2, 0.1);

                (*gr).add_edge(n4, n2, 0.1);
                (*gr).add_edge(n4, n5, 0.1);
                (*gr).add_edge(n4, n6, 0.1);

                (*gr).add_edge(n5, n7, 0.1);
                (*gr).add_edge(n5, n8, 0.1);

                (*gr).add_edge(n6, n7, 0.1);

                (*gr).add_edge(n7, n11, 0.1);

                (*gr).add_edge(n8, n9, 0.1);

                (*gr).add_edge(n9, n8, 0.1);
                (*gr).add_edge(n9, n10, 0.1);

                (*gr).add_edge(n10, n5, 0.1);
                (*gr).add_edge(n10, n12, 0.1);

                (*gr).add_edge(n11, n12, 0.1);

                (*gr).root() = n1;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsAppel.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators_lengauer_tarjan(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                std::unordered_map<string_type, std::vector<string_type>> doms;
                std::unordered_map<string_type, string_type> idoms;
                for(auto& vertex : (*gr).vertices())
                {
                    std::vector<string_type> dom;
                    for(auto& d : (*vertex).dominators())
                        dom.push_back((*d).label());
                    doms[(*vertex).label()] = dom;
                    if((*vertex).idominator() != nullptr)
                        idoms[(*vertex).label()] = (*(*vertex).idominator()).label();
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsCharlesFischer)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(1,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(2,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(3,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(4,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(5,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(6,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(7,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(8,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(9,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(10, L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(11, L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(12, L"12")).first;
                const auto& n13 = *(*gr).add_vertex(factory::create<dominator_vertex>(13, L"13")).first;

                (*gr).add_edge(n1, n2, 0.1);
                (*gr).add_edge(n1, n3, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n4, 0.1);
                (*gr).add_edge(n2, n11, 0.1);

                (*gr).add_edge(n3, n4, 0.1);
                (*gr).add_edge(n3, n9, 0.1);

                (*gr).add_edge(n4, n5, 0.1);
                (*gr).add_edge(n4, n8, 0.1);

                (*gr).add_edge(n5, n3, 0.1);
                (*gr).add_edge(n5, n6, 0.1);

                (*gr).add_edge(n6, n5, 0.1);
                (*gr).add_edge(n6, n7, 0.1);

                (*gr).add_edge(n8, n6, 0.1);

                (*gr).add_edge(n9, n8, 0.1);
                (*gr).add_edge(n9, n10, 0.1);

                (*gr).add_edge(n10, n6, 0.1);

                (*gr).add_edge(n11, n12, 0.1);
                (*gr).add_edge(n11, n13, 0.1);

                (*gr).add_edge(n12, n7, 0.1);
                (*gr).add_edge(n12, n11, 0.1);

                (*gr).add_edge(n13, n12, 0.1);

                (*gr).root() = n1;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsCharlesFischer.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators_lengauer_tarjan(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                std::unordered_map<string_type, std::vector<string_type>> doms;
                std::unordered_map<string_type, string_type> idoms;
                for(auto& vertex : (*gr).vertices())
                {
                    std::vector<string_type> dom;
                    for(auto& d : (*vertex).dominators())
                        dom.push_back((*d).label());
                    doms[(*vertex).label()] = dom;
                    if((*vertex).idominator() != nullptr)
                        idoms[(*vertex).label()] = (*(*vertex).idominator()).label();
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsCytron)
            {
                std::shared_ptr<graph<dominator_vertex>> gr(factory::create<graph<dominator_vertex>>());

                const auto& en  = *(*gr).add_vertex(factory::create<dominator_vertex>(10,  L"Entry")).first;
                const auto& n1  = *(*gr).add_vertex(factory::create<dominator_vertex>(11,  L"1")).first;
                const auto& n2  = *(*gr).add_vertex(factory::create<dominator_vertex>(12,  L"2")).first;
                const auto& n3  = *(*gr).add_vertex(factory::create<dominator_vertex>(13,  L"3")).first;
                const auto& n4  = *(*gr).add_vertex(factory::create<dominator_vertex>(14,  L"4")).first;
                const auto& n5  = *(*gr).add_vertex(factory::create<dominator_vertex>(15,  L"5")).first;
                const auto& n6  = *(*gr).add_vertex(factory::create<dominator_vertex>(16,  L"6")).first;
                const auto& n7  = *(*gr).add_vertex(factory::create<dominator_vertex>(17,  L"7")).first;
                const auto& n8  = *(*gr).add_vertex(factory::create<dominator_vertex>(18,  L"8")).first;
                const auto& n9  = *(*gr).add_vertex(factory::create<dominator_vertex>(19,  L"9")).first;
                const auto& n10 = *(*gr).add_vertex(factory::create<dominator_vertex>(20,  L"10")).first;
                const auto& n11 = *(*gr).add_vertex(factory::create<dominator_vertex>(21,  L"11")).first;
                const auto& n12 = *(*gr).add_vertex(factory::create<dominator_vertex>(22,  L"12")).first;
                const auto& ex  = *(*gr).add_vertex(factory::create<dominator_vertex>(110, L"Exit")).first;

                (*gr).add_edge(en, n1, 0.1);
                (*gr).add_edge(en, ex, 0.1);

                (*gr).add_edge(n1, n2, 0.1);

                (*gr).add_edge(n2, n3, 0.1);
                (*gr).add_edge(n2, n7, 0.1);

                (*gr).add_edge(n3, n4, 0.1);
                (*gr).add_edge(n3, n5, 0.1);

                (*gr).add_edge(n4, n6, 0.1);

                (*gr).add_edge(n5, n6, 0.1);

                (*gr).add_edge(n6, n8, 0.1);

                (*gr).add_edge(n7, n8, 0.1);

                (*gr).add_edge(n8, n9, 0.1);

                (*gr).add_edge(n9, n10, 0.1);
                (*gr).add_edge(n9, n11, 0.1);

                (*gr).add_edge(n10, n11, 0.1);

                (*gr).add_edge(n11, n9, 0.1);
                (*gr).add_edge(n11, n12, 0.1);

                (*gr).add_edge(n12, n2, 0.1);
                (*gr).add_edge(n12, ex, 0.1);

                (*gr).root() = en;

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsCytron.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators_lengauer_tarjan(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                std::unordered_map<string_type, string_type> idoms;
                for(auto& vertex : (*gr).vertices())
                {
                    if((*vertex).idominator() != nullptr)
                        idoms[(*vertex).label()] = (*(*vertex).idominator()).label();
                }

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }

            TEST_METHOD(GraphAlgorithmsComputeDominatorsPerformance)
            {
                const int n = 5011;

                std::shared_ptr<graph<dominator_vertex>> graph(factory::create<graph<dominator_vertex>>());

                (*graph).root() = *(*graph).add_vertex(factory::create<dominator_vertex>(0)).first;

                std::srand((unsigned int)std::time(nullptr));

                for(auto k = 0; k < n; k += 2)
                {
                    const auto& v1 = *(*graph).add_vertex(factory::create<dominator_vertex>(std::rand() % n)).first;
                    (*v1).label() = std::to_wstring((*v1).id());
                    const auto& v2 = *(*graph).add_vertex(factory::create<dominator_vertex>(std::rand() % n)).first;
                    (*v2).label() = std::to_wstring((*v2).id());

                    (*graph).add_edge(v1, v2, 0.1);

                    (*graph).add_edge((*graph).root(), v1, 0.1);
                    (*graph).add_edge((*graph).root(), v2, 0.1);
                }

                //graph_algorithms<dominator_vertex>::generate_graphviz_file(graph, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsPerformance.dot)", false);

                auto start_lengauer_tarjan = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators_lengauer_tarjan(graph);

                auto finish_lengauer_tarjan = std::chrono::high_resolution_clock::now();
                auto elapsed_lengauer_tarjan = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_lengauer_tarjan - start_lengauer_tarjan).count();

                Logger::WriteMessage(("Lengauer Tarjan: " + std::to_string(elapsed_lengauer_tarjan / 1000000.0)).c_str());
                Logger::WriteMessage("\n");

                std::unordered_map<string_type, string_type> idoms_lengauer_tarjan;
                for(auto& vertex : (*graph).vertices())
                {
                    if((*vertex).idominator() != nullptr)
                    {
                        idoms_lengauer_tarjan[(*vertex).label()] = (*(*vertex).idominator()).label();
                        (*vertex).idominator().reset();
                    }
                }

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators(graph);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                Logger::WriteMessage(("Naive bitset:       " + std::to_string(elapsed / 1000000.0)).c_str());

                std::unordered_map<string_type, string_type> idoms;
                for(auto& vertex : (*graph).vertices())
                {
                    if((*vertex).idominator() != nullptr)
                        idoms[(*vertex).label()] = (*(*vertex).idominator()).label();
                }

                bool rc = idoms_lengauer_tarjan == idoms;
                Assert::IsTrue(rc);
            }

            TEST_METHOD(BuildSSaFormCytron)
            {
                //                                                  op                arg1    arg2    res
                //                                                  ..................................... 
                //  I ← 1                                         1 assignment_hir    1               I
                //  J ← 1                                         2 assignment_hir    1               J
                //  K ← 1                                         3 assignment_hir    1               K
                //  L ← 1                                         4 assignment_hir    1               L
                //  repeat                                        5 label                             M100
                //      if(P)                                     6 if_false_hir      P               M0
                //          J ← I                                 7 assignment_hir    I               J
                //          if(Q)                                 8 if_false_hir      Q               M1
                //              L ← 2                             9 assignment_hir    2               L
                //                                               10 goto                              M2
                //          else                                 11 label                             M1
                //              L ← 3                            12 assignment_hir    3               L
                //                                               13 label                             M2
                //          K ← K + 1                            14 assignment_hir    K       1       K
                //                                               15 goto                              M11
                //      else                                     16 label                             M0
                //          K ← K + 2                            17 assignment_hir    K       2       K
                //                                               18 goto                              M11
                //                                               19 label                             M11
                //      repeat                                   20 label                             M10
                //          if(R)                                21 if_false_hir      R               M3
                //              L ← L + 4                        22 assignment_hir    L       4       L
                //                                               23 label                             M3
                //      until(S)                                 24 if_true_hir       S               M10
                //      I ← I + 6                                25 assignment_hir    I       6       I
                //  until(T)                                     26 if_true_hir       T               M100
                using arcturus_instruction = arcturus_quadruple;
                using arcturus_code = code<arcturus_instruction>;

                using op = arcturus_operation_code_traits::operation_code;

                counter symb_counter;
                counter instr_counter;

                symb_counter.number(); // starts from 1
                instr_counter.number(); // starts from 1

                auto symbol_1(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_1).name() = text::chars_to_codepoints("1", 1);

                auto symbol_2(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_2).name() = text::chars_to_codepoints("2", 1);

                auto symbol_3(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_3).name() = text::chars_to_codepoints("3", 1);

                auto symbol_4(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_4).name() = text::chars_to_codepoints("4", 1);

                auto symbol_6(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_6).name() = text::chars_to_codepoints("6", 1);

                auto symbol_i(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_i).name() = text::chars_to_codepoints("I", 1);

                auto symbol_j(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_j).name() = text::chars_to_codepoints("J", 1);

                auto symbol_k(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_k).name() = text::chars_to_codepoints("K", 1);

                auto symbol_l(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_l).name() = text::chars_to_codepoints("L", 1);

                auto symbol_p(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_p).name() = text::chars_to_codepoints("P", 1);

                auto symbol_q(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_q).name() = text::chars_to_codepoints("Q", 1);

                auto symbol_r(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_r).name() = text::chars_to_codepoints("R", 1);

                auto symbol_s(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_s).name() = text::chars_to_codepoints("S", 1);

                auto symbol_t(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_t).name() = text::chars_to_codepoints("T", 1);

                auto symbol_m0(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_m0).name() = text::chars_to_codepoints("M0", 2);

                auto symbol_m1(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_m1).name() = text::chars_to_codepoints("M1", 2);

                auto symbol_m2(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_m2).name() = text::chars_to_codepoints("M2", 2);

                auto symbol_m3(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_m3).name() = text::chars_to_codepoints("M3", 2);

                auto symbol_m10(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_m10).name() = text::chars_to_codepoints("M10", 3);

                auto symbol_m11(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_m11).name() = text::chars_to_codepoints("M11", 3);

                auto symbol_m100(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_m100).name() = text::chars_to_codepoints("M100", 4);

                auto symbol_m101(factory::create<arcturus_symbol>(symb_counter.number()));
                (*symbol_m101).name() = text::chars_to_codepoints("M101", 4);

                auto code(factory::create<arcturus_code>());

                std::shared_ptr<arcturus_instruction> instr1;
                std::shared_ptr<arcturus_instruction> instr2;
                std::shared_ptr<arcturus_instruction> instr3;
                std::shared_ptr<arcturus_instruction> instr4;
                std::shared_ptr<arcturus_instruction> instr5;
                std::shared_ptr<arcturus_instruction> instr6;
                std::shared_ptr<arcturus_instruction> instr7;
                std::shared_ptr<arcturus_instruction> instr8;
                std::shared_ptr<arcturus_instruction> instr9;
                std::shared_ptr<arcturus_instruction> instr10;
                std::shared_ptr<arcturus_instruction> instr11;
                std::shared_ptr<arcturus_instruction> instr12;
                std::shared_ptr<arcturus_instruction> instr13;
                std::shared_ptr<arcturus_instruction> instr14;
                std::shared_ptr<arcturus_instruction> instr15;
                std::shared_ptr<arcturus_instruction> instr16;
                std::shared_ptr<arcturus_instruction> instr17;
                std::shared_ptr<arcturus_instruction> instr18;
                std::shared_ptr<arcturus_instruction> instr19;
                std::shared_ptr<arcturus_instruction> instr20;
                std::shared_ptr<arcturus_instruction> instr21;
                std::shared_ptr<arcturus_instruction> instr22;
                std::shared_ptr<arcturus_instruction> instr23;
                std::shared_ptr<arcturus_instruction> instr24;
                std::shared_ptr<arcturus_instruction> instr25;
                std::shared_ptr<arcturus_instruction> instr26;

                // 1 assignment_hir    1               I
                instr1 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::assignment_hir,
                                                               std::make_pair(symbol_1, 0),
                                                               std::make_pair(symbol_i, 0));
                (*code).add_instruction(instr1);

                // 2 assignment_hir    1               J
                instr2 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::assignment_hir,
                                                               std::make_pair(symbol_1, 0),
                                                               std::make_pair(symbol_j, 0));
                (*code).add_instruction(instr2);

                // 3 assignment_hir    1               K
                instr3 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::assignment_hir,
                                                               std::make_pair(symbol_1, 0),
                                                               std::make_pair(symbol_k, 0));
                (*code).add_instruction(instr3);

                // 4 assignment_hir    1               L
                instr4 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::assignment_hir,
                                                               std::make_pair(symbol_1, 0),
                                                               std::make_pair(symbol_l, 0));
                (*code).add_instruction(instr4);

                //  5 label                             M100
                instr5 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::label_hir,
                                                               std::make_pair(symbol_m100, 0));
                (*code).add_instruction(instr5);

                //  6 if_false_hir      P               M0
                instr6 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::if_false_hir,
                                                               std::make_pair(symbol_p, 0),
                                                               instr16);
                (*code).add_instruction(instr6);

                //  7 assignment_hir    I               J
                instr7 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::assignment_hir,
                                                               std::make_pair(symbol_i, 0),
                                                               std::make_pair(symbol_j, 0));
                (*code).add_instruction(instr7);

                //  8 if_false_hir      Q               M1
                instr8 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::if_false_hir,
                                                               std::make_pair(symbol_q, 0),
                                                               instr11);
                (*code).add_instruction(instr8);

                //  9 assignment_hir    2               L
                instr9 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                               op::assignment_hir,
                                                               std::make_pair(symbol_2, 0),
                                                               std::make_pair(symbol_l, 0));
                (*code).add_instruction(instr9);

                // 10 goto                              M2
                instr10 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::goto_hir,
                                                                instr13);
                (*code).add_instruction(instr10);

                // 11 label                             M1
                instr11 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::label_hir,
                                                                std::make_pair(symbol_m1, 0));
                (*instr8).result = instr11;
                (*code).add_instruction(instr11);

                // 12 assignment_hir    3               L
                instr12 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::assignment_hir,
                                                                std::make_pair(symbol_3, 0),
                                                                std::make_pair(symbol_l, 0));
                (*code).add_instruction(instr12);

                // 13 label                             M2
                instr13 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::label_hir,
                                                                std::make_pair(symbol_m2, 0));
                (*instr10).result = instr13;
                (*code).add_instruction(instr13);

                // 14 assignment_hir    K       1       K
                instr14 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::assignment_hir,
                                                                std::make_pair(symbol_k, 0),
                                                                std::make_pair(symbol_1, 0),
                                                                std::make_pair(symbol_k, 0));
                (*code).add_instruction(instr14);

                // 15 goto                              M11
                instr15 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::goto_hir,
                                                                instr19);
                (*code).add_instruction(instr15);

                // 16 label                             M0
                instr16 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::label_hir,
                                                                std::make_pair(symbol_m0, 0));
                (*instr6).result = instr16;
                (*code).add_instruction(instr16);

                // 17 assignment_hir    K       2       K
                instr17 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::assignment_hir,
                                                                std::make_pair(symbol_k, 0),
                                                                std::make_pair(symbol_2, 0),
                                                                std::make_pair(symbol_k, 0));
                (*code).add_instruction(instr17);

                // 18 goto                              M11
                instr18 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::goto_hir,
                                                                instr19);
                (*code).add_instruction(instr18);

                // 19 label                             M11
                instr19 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::label_hir,
                                                                std::make_pair(symbol_m11, 0));
                (*instr15).result = instr19;
                (*instr18).result = instr19;
                (*code).add_instruction(instr19);

                // 20 label                             M10
                instr20 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::label_hir,
                                                                std::make_pair(symbol_m10, 0));
                //(*instr15).result = instr20;
                (*code).add_instruction(instr20);

                // 21 if_false_hir      R               M3
                instr21 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::if_false_hir,
                                                                std::make_pair(symbol_r, 0),
                                                                instr23);
                (*code).add_instruction(instr21);

                // 22 assignment_hir    L       4       L
                instr22 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::assignment_hir,
                                                                std::make_pair(symbol_l, 0),
                                                                std::make_pair(symbol_4, 0),
                                                                std::make_pair(symbol_l, 0));
                (*code).add_instruction(instr22);

                // 23 label                             M3
                instr23 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::label_hir,
                                                                std::make_pair(symbol_m3, 0));
                (*instr21).result = instr23;
                (*code).add_instruction(instr23);

                // 24 if_true_hir       S               M10
                instr24 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::if_true_hir,
                                                                std::make_pair(symbol_s, 0),
                                                                instr20);
                (*code).add_instruction(instr24);

                // 25 assignment_hir    I       6       I
                instr25 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::assignment_hir,
                                                                std::make_pair(symbol_i, 0),
                                                                std::make_pair(symbol_6, 0),
                                                                std::make_pair(symbol_i, 0));
                (*code).add_instruction(instr25);

                // 26 if_true_hir       T               M100
                instr26 = factory::create<arcturus_instruction>(instr_counter.number(),
                                                                op::if_true_hir,
                                                                std::make_pair(symbol_t, 0),
                                                                instr5);
                (*code).add_instruction(instr26);

                for(auto it = (*code).instructions(); it != (*code).end_instruction(); it = std::static_pointer_cast<arcturus_quadruple>((*it).next()))
                {
                    Logger::WriteMessage((*it).to_string().c_str());
                    Logger::WriteMessage(L"\n");
                }

                auto cfg(factory::create<arcturus_control_flow_graph>());

                (*cfg).collect_basic_blocks(code);

                (*cfg).generate_graphviz_file(LR"(d:\tmp\BuildSSaFormCytron.CFG.dot)");

                arcturus_ssa::build_ssa_form(cfg);

                (*cfg).generate_graphviz_file(LR"(d:\tmp\BuildSSaFormCytron.SSA.dot)");
            }
    };
}
