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

            TEST_METHOD(BuildCode)
            {
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

                graph_algorithms<dominator_vertex>::dfs_to_vector(gr, result);

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

                graph_algorithms<dominator_vertex>::dfs_to_vector(gr, result);

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

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsMuchnik.dot)", false);

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

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsLengauerTarjan.dot)", false);

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

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsAppel.dot)", false);

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

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsCharlesFischer.dot)", false);

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

                graph_algorithms<dominator_vertex>::generate_graphviz_file(gr, LR"(d:\tmp\GraphAlgorithmsComputeDominatorsCytron.dot)", false);

                auto start = std::chrono::high_resolution_clock::now();

                graph_algorithms<dominator_vertex>::compute_dominators(gr);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

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

                graph_algorithms<dominator_vertex>::postorder_to_vector(tree, result);

                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                Logger::WriteMessage(std::to_string(elapsed / 1000000.0).c_str());
            }
    };
}
