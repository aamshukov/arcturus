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

#include <arcturus_token.hpp>
#include <arcturus_lexical_analyzer.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

//arcturus_lexical_analyzer::arcturus_lexical_analyzer(const content_type& content, uint8_t tab_size)
//                     : lexical_analyzer(content),
//                       my_unicode(false),
//                       my_unicode_length(0),
//                       my_unicode_backslash_count(0),
//                       //my_line_map_size(0),
//                       //my_cached_line(-1),
//                       //my_cached_line_position(-1),
//                       //my_tab_size(tab_size),
//                       my_indedents(0),
//                       my_boll(true),
//                       my_eoll(false)
//{
//    my_indents.push(0);
//}
//
//arcturus_lexical_analyzer::~java_lexical_analyzer()
//{
//}
//
//bool java_lexical_analyzer::create(const string_type& file_name, lexical_analyzer_type& result_lexical_analyzer, operation_status& status, uint8_t tab_size)
//{
//    file_data_provider provider(file_name);
//
//    auto content(factory::create<uilab::frontend::lexical_content>(1, file_name)); //??
//
//    bool result = (*content).load(provider, status);
//
//    if(result)
//    {
//        auto lexer(factory::create<java_lexical_analyzer>(content, tab_size));
//
//        //(*lexer).build_line_map();
//
//        result_lexical_analyzer.swap(lexer);
//    }
//
//    return result;
//}
//
//void java_lexical_analyzer::build_fsa_from_re(const string_type& re,
//                                              typename java_lexical_analyzer::token_type::traits::type token,
//                                              typename java_lexical_analyzer::token_type::traits::type escape_token,
//                                              const string_type& escape_predicate,
//                                              typename java_lexical_analyzer::fsa_type& result_fsa)
//{
//    operation_status status; //??
//
//    std::shared_ptr<datum_type[]> re_data;
//
//    std::size_t count = 0;
//    text::string_to_codepoints(re, re_data, count, status);
//
//    fsa::fsa_type nfa;
//    fsa_re::re_to_fsa(re_data, count, static_cast<std::size_t>(token), static_cast<std::size_t>(escape_token), escape_predicate, nfa, status);
//
//    fsa::fsa_type dfa;
//    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);
//
//    fsa::fsa_type mindfa;
//    fsa_algorithm::minimize_dfa(dfa, mindfa);
//
//    result_fsa.swap(mindfa);
//}
//
//void java_lexical_analyzer::build_numeric_literals(const string_type& file_name, const string_type& label_prefix)
//{
//    const string_type java_binary_re(L"0(b|B)(0|1)(_*(0|1))*(l|L)?");
//    const string_type java_octal_re(L"0(((0|1|2|3|4|5|6|7)*)_*(0|1|2|3|4|5|6|7))+(l|L)?");
//    const string_type java_decimal_re(L"((0)|(((1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)))(l|L)?");
//    const string_type java_hexadecimal_re(L"0(x|X)(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*(l|L)?");
//    const string_type java_decimal_floating_re(L"(((((((0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)?)\\.((0|1|2|3|4|5|6|7|8|9)(_*?(0|1|2|3|4|5|6|7|8|9))*))|(((0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)\\.))((e|E)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)?)|(((0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)((e|E)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)))(f|d|F|D)?|(((0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)(f|d|F|D))");
//    const string_type java_hexadecimal_floating_re(L"0(x|X)(((0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*)*\\.(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*|(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*\\.|(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F)(_*(0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f|A|B|C|D|E|F))*)((p|P)(\\+|\\-)?(0|1|2|3|4|5|6|7|8|9)(_*(0|1|2|3|4|5|6|7|8|9))*)(f|d|F|D)?");
//
//    operation_status status; //??
//
//    fsa::fsa_type binary_fsa;
//    build_fsa_from_re(java_binary_re, token_type::traits::type::binary_integer_lit, token_type::traits::type::unknown, empty_string(), binary_fsa);
//
//    fsa::fsa_type octal_fsa;
//    build_fsa_from_re(java_octal_re, token_type::traits::type::octal_integer_lit, token_type::traits::type::unknown, empty_string(), octal_fsa);
//
//    fsa::fsa_type decimal_fsa;
//    build_fsa_from_re(java_decimal_re, token_type::traits::type::decimal_integer_lit, token_type::traits::type::unknown, empty_string(), decimal_fsa);
//
//    fsa::fsa_type hexadecimal_fsa;
//    build_fsa_from_re(java_hexadecimal_re, token_type::traits::type::hexadecimal_integer_lit, token_type::traits::type::unknown, empty_string(), hexadecimal_fsa);
//
//    fsa::fsa_type decimal_floating_fsa;
//    build_fsa_from_re(java_decimal_floating_re, token_type::traits::type::decimal_floating_lit, token_type::traits::type::unknown, empty_string(), decimal_floating_fsa);
//
//    fsa::fsa_type hexadecimal_floating_fsa;
//    build_fsa_from_re(java_hexadecimal_floating_re, token_type::traits::type::hexadecimal_floating_lit, token_type::traits::type::unknown, empty_string(), hexadecimal_floating_fsa);
//
//    std::vector<fsa::fsa_type> fsas;
//
//    fsas.emplace_back(binary_fsa);
//    fsas.emplace_back(octal_fsa);
//    fsas.emplace_back(decimal_fsa);
//    fsas.emplace_back(hexadecimal_fsa);
//    fsas.emplace_back(decimal_floating_fsa);
//    fsas.emplace_back(hexadecimal_floating_fsa);
//
//    fsa::fsa_type nfa;
//    fsa::combine(fsas, nfa, status);
//
//    fsa::fsa_type dfa;
//    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);
//
//    fsa::fsa_type mindfa;
//    fsa_algorithm::minimize_dfa(dfa, mindfa);
//
//    fsa_codegen::generate<token_type>(mindfa, file_name, label_prefix);
//
//    fsa_visualization::generate_graphviz_file(nfa, string_type(LR"(d:\tmp\FSA\nfa.dot)"));            //??
//    fsa_visualization::generate_graphviz_file(dfa, string_type(LR"(d:\tmp\FSA\dfa.dot)"));
//    fsa_visualization::generate_graphviz_file(mindfa, string_type(LR"(d:\tmp\FSA\mindfa.dot)"));
//}
//
//void java_lexical_analyzer::build_identifiers_and_keywords(const string_type& file_name, const string_type& label_prefix)
//{
//    operation_status status; //??
//
//    fsa::fsa_type abstract_fsa;
//    build_fsa_from_re(L"abstract", token_type::traits::type::abstract_kw, token_type::traits::type::identifier, IDENTIFIER_PART, abstract_fsa);
//
//    fsa::fsa_type assert_fsa;
//    build_fsa_from_re(L"assert", token_type::traits::type::assert_kw, token_type::traits::type::identifier, IDENTIFIER_PART, assert_fsa);
//
//    fsa::fsa_type boolean_fsa;
//    build_fsa_from_re(L"boolean", token_type::traits::type::boolean_kw, token_type::traits::type::identifier, IDENTIFIER_PART, boolean_fsa);
//
//    fsa::fsa_type break_fsa;
//    build_fsa_from_re(L"break", token_type::traits::type::break_kw, token_type::traits::type::identifier, IDENTIFIER_PART, break_fsa);
//
//    fsa::fsa_type byte_fsa;
//    build_fsa_from_re(L"byte", token_type::traits::type::byte_kw, token_type::traits::type::identifier, IDENTIFIER_PART, byte_fsa);
//
//    fsa::fsa_type case_fsa;
//    build_fsa_from_re(L"case", token_type::traits::type::case_kw, token_type::traits::type::identifier, IDENTIFIER_PART, case_fsa);
//
//    fsa::fsa_type catch_fsa;
//    build_fsa_from_re(L"catch", token_type::traits::type::catch_kw, token_type::traits::type::identifier, IDENTIFIER_PART, catch_fsa);
//
//    fsa::fsa_type char_fsa;
//    build_fsa_from_re(L"char", token_type::traits::type::char_kw, token_type::traits::type::identifier, IDENTIFIER_PART, char_fsa);
//
//    fsa::fsa_type class_fsa;
//    build_fsa_from_re(L"class", token_type::traits::type::class_kw, token_type::traits::type::identifier, IDENTIFIER_PART, class_fsa);
//
//    fsa::fsa_type const_fsa;
//    build_fsa_from_re(L"const", token_type::traits::type::const_kw, token_type::traits::type::identifier, IDENTIFIER_PART, const_fsa);
//
//    fsa::fsa_type continue_fsa;
//    build_fsa_from_re(L"continue", token_type::traits::type::continue_kw, token_type::traits::type::identifier, IDENTIFIER_PART, continue_fsa);
//
//    fsa::fsa_type default_fsa;
//    build_fsa_from_re(L"default", token_type::traits::type::default_kw, token_type::traits::type::identifier, IDENTIFIER_PART, default_fsa);
//
//    fsa::fsa_type do_fsa;
//    build_fsa_from_re(L"do", token_type::traits::type::do_kw, token_type::traits::type::identifier, IDENTIFIER_PART, do_fsa);
//
//    fsa::fsa_type double_fsa;
//    build_fsa_from_re(L"double", token_type::traits::type::double_kw, token_type::traits::type::identifier, IDENTIFIER_PART, double_fsa);
//
//    fsa::fsa_type else_fsa;
//    build_fsa_from_re(L"else", token_type::traits::type::else_kw, token_type::traits::type::identifier, IDENTIFIER_PART, else_fsa);
//
//    fsa::fsa_type enum_fsa;
//    build_fsa_from_re(L"enum", token_type::traits::type::enum_kw, token_type::traits::type::identifier, IDENTIFIER_PART, enum_fsa);
//
//    fsa::fsa_type extends_fsa;
//    build_fsa_from_re(L"extends", token_type::traits::type::extends_kw, token_type::traits::type::identifier, IDENTIFIER_PART, extends_fsa);
//
//    fsa::fsa_type final_fsa;
//    build_fsa_from_re(L"final", token_type::traits::type::final_kw, token_type::traits::type::identifier, IDENTIFIER_PART, final_fsa);
//
//    fsa::fsa_type finally_fsa;
//    build_fsa_from_re(L"finally", token_type::traits::type::finally_kw, token_type::traits::type::identifier, IDENTIFIER_PART, finally_fsa);
//
//    fsa::fsa_type float_fsa;
//    build_fsa_from_re(L"float", token_type::traits::type::float_kw, token_type::traits::type::identifier, IDENTIFIER_PART, float_fsa);
//
//    fsa::fsa_type for_fsa;
//    build_fsa_from_re(L"for", token_type::traits::type::for_kw, token_type::traits::type::identifier, IDENTIFIER_PART, for_fsa);
//
//    fsa::fsa_type if_fsa;
//    build_fsa_from_re(L"if", token_type::traits::type::if_kw, token_type::traits::type::identifier, IDENTIFIER_PART, if_fsa);
//
//    fsa::fsa_type goto_fsa;
//    build_fsa_from_re(L"goto", token_type::traits::type::goto_kw, token_type::traits::type::identifier, IDENTIFIER_PART, goto_fsa);
//
//    fsa::fsa_type implements_fsa;
//    build_fsa_from_re(L"implements", token_type::traits::type::implements_kw, token_type::traits::type::identifier, IDENTIFIER_PART, implements_fsa);
//
//    fsa::fsa_type import_fsa;
//    build_fsa_from_re(L"import", token_type::traits::type::import_kw, token_type::traits::type::identifier, IDENTIFIER_PART, import_fsa);
//
//    fsa::fsa_type instanceof_fsa;
//    build_fsa_from_re(L"instanceof", token_type::traits::type::instanceof_kw, token_type::traits::type::identifier, IDENTIFIER_PART, instanceof_fsa);
//
//    fsa::fsa_type int_fsa;
//    build_fsa_from_re(L"int", token_type::traits::type::int_kw, token_type::traits::type::identifier, IDENTIFIER_PART, int_fsa);
//
//    fsa::fsa_type interface_fsa;
//    build_fsa_from_re(L"interface", token_type::traits::type::interface_kw, token_type::traits::type::identifier, IDENTIFIER_PART, interface_fsa);
//
//    fsa::fsa_type long_fsa;
//    build_fsa_from_re(L"long", token_type::traits::type::long_kw, token_type::traits::type::identifier, IDENTIFIER_PART, long_fsa);
//
//    fsa::fsa_type native_fsa;
//    build_fsa_from_re(L"native", token_type::traits::type::native_kw, token_type::traits::type::identifier, IDENTIFIER_PART, native_fsa);
//
//    fsa::fsa_type new_fsa;
//    build_fsa_from_re(L"new", token_type::traits::type::new_kw, token_type::traits::type::identifier, IDENTIFIER_PART, new_fsa);
//
//    fsa::fsa_type package_fsa;
//    build_fsa_from_re(L"package", token_type::traits::type::package_kw, token_type::traits::type::identifier, IDENTIFIER_PART, package_fsa);
//
//    fsa::fsa_type private_fsa;
//    build_fsa_from_re(L"private", token_type::traits::type::private_kw, token_type::traits::type::identifier, IDENTIFIER_PART, private_fsa);
//
//    fsa::fsa_type protected_fsa;
//    build_fsa_from_re(L"protected", token_type::traits::type::protected_kw, token_type::traits::type::identifier, IDENTIFIER_PART, protected_fsa);
//
//    fsa::fsa_type public_fsa;
//    build_fsa_from_re(L"public", token_type::traits::type::public_kw, token_type::traits::type::identifier, IDENTIFIER_PART, public_fsa);
//
//    fsa::fsa_type return_fsa;
//    build_fsa_from_re(L"return", token_type::traits::type::return_kw, token_type::traits::type::identifier, IDENTIFIER_PART, return_fsa);
//
//    fsa::fsa_type short_fsa;
//    build_fsa_from_re(L"short", token_type::traits::type::short_kw, token_type::traits::type::identifier, IDENTIFIER_PART, short_fsa);
//
//    fsa::fsa_type static_fsa;
//    build_fsa_from_re(L"static", token_type::traits::type::static_kw, token_type::traits::type::identifier, IDENTIFIER_PART, static_fsa);
//
//    fsa::fsa_type strictfp_fsa;
//    build_fsa_from_re(L"strictfp", token_type::traits::type::strictfp_kw, token_type::traits::type::identifier, IDENTIFIER_PART, strictfp_fsa);
//
//    fsa::fsa_type super_fsa;
//    build_fsa_from_re(L"super", token_type::traits::type::super_kw, token_type::traits::type::identifier, IDENTIFIER_PART, super_fsa);
//
//    fsa::fsa_type switch_fsa;
//    build_fsa_from_re(L"switch", token_type::traits::type::switch_kw, token_type::traits::type::identifier, IDENTIFIER_PART, switch_fsa);
//
//    fsa::fsa_type synchronized_fsa;
//    build_fsa_from_re(L"synchronized", token_type::traits::type::synchronized_kw, token_type::traits::type::identifier, IDENTIFIER_PART, synchronized_fsa);
//
//    fsa::fsa_type this_fsa;
//    build_fsa_from_re(L"this", token_type::traits::type::this_kw, token_type::traits::type::identifier, IDENTIFIER_PART, this_fsa);
//
//    fsa::fsa_type throw_fsa;
//    build_fsa_from_re(L"throw", token_type::traits::type::throw_kw, token_type::traits::type::identifier, IDENTIFIER_PART, throw_fsa);
//
//    fsa::fsa_type transient_fsa;
//    build_fsa_from_re(L"transient", token_type::traits::type::transient_kw, token_type::traits::type::identifier, IDENTIFIER_PART, transient_fsa);
//
//    fsa::fsa_type try_fsa;
//    build_fsa_from_re(L"try", token_type::traits::type::try_kw, token_type::traits::type::identifier, IDENTIFIER_PART, try_fsa);
//
//    fsa::fsa_type void_fsa;
//    build_fsa_from_re(L"void", token_type::traits::type::void_kw, token_type::traits::type::identifier, IDENTIFIER_PART, void_fsa);
//
//    fsa::fsa_type volatile_fsa;
//    build_fsa_from_re(L"volatile", token_type::traits::type::volatile_kw, token_type::traits::type::identifier, IDENTIFIER_PART, volatile_fsa);
//
//    fsa::fsa_type while_fsa;
//    build_fsa_from_re(L"while", token_type::traits::type::while_kw, token_type::traits::type::identifier, IDENTIFIER_PART, while_fsa);
//
//    fsa::fsa_type true_fsa;
//    build_fsa_from_re(L"true", token_type::traits::type::true_lit, token_type::traits::type::identifier, IDENTIFIER_PART, true_fsa);
//
//    fsa::fsa_type false_fsa;
//    build_fsa_from_re(L"false", token_type::traits::type::false_lit, token_type::traits::type::identifier, IDENTIFIER_PART, false_fsa);
//
//    fsa::fsa_type null_fsa;
//    build_fsa_from_re(L"null", token_type::traits::type::null_lit, token_type::traits::type::identifier, IDENTIFIER_PART, null_fsa);
//
//    fsa::fsa_type var_fsa;
//    build_fsa_from_re(L"var", token_type::traits::type::var_kw, token_type::traits::type::identifier, IDENTIFIER_PART, var_fsa);
//
//    fsa::fsa_type identifier_fsa;
//    build_fsa_from_re(format(L"(%s)(%s)*", IDENTIFIER_START, IDENTIFIER_PART), token_type::traits::type::identifier, token_type::traits::type::unknown, IDENTIFIER_PART, identifier_fsa);
//
//    std::vector<fsa::fsa_type> fsas;
//
//    fsas.emplace_back(abstract_fsa);
//    fsas.emplace_back(assert_fsa);
//    fsas.emplace_back(boolean_fsa);
//    fsas.emplace_back(break_fsa);
//    fsas.emplace_back(byte_fsa);
//    fsas.emplace_back(case_fsa);
//    fsas.emplace_back(catch_fsa);
//    fsas.emplace_back(char_fsa);
//    fsas.emplace_back(class_fsa);
//    fsas.emplace_back(const_fsa);
//    fsas.emplace_back(continue_fsa);
//    fsas.emplace_back(default_fsa);
//    fsas.emplace_back(do_fsa);
//    fsas.emplace_back(double_fsa);
//    fsas.emplace_back(else_fsa);
//    fsas.emplace_back(enum_fsa);
//    fsas.emplace_back(extends_fsa);
//    fsas.emplace_back(final_fsa);
//    fsas.emplace_back(finally_fsa);
//    fsas.emplace_back(float_fsa);
//    fsas.emplace_back(for_fsa);
//    fsas.emplace_back(if_fsa);
//    fsas.emplace_back(goto_fsa);
//    fsas.emplace_back(implements_fsa);
//    fsas.emplace_back(import_fsa);
//    fsas.emplace_back(instanceof_fsa);
//    fsas.emplace_back(int_fsa);
//    fsas.emplace_back(interface_fsa);
//    fsas.emplace_back(long_fsa);
//    fsas.emplace_back(native_fsa);
//    fsas.emplace_back(new_fsa);
//    fsas.emplace_back(package_fsa);
//    fsas.emplace_back(private_fsa);
//    fsas.emplace_back(protected_fsa);
//    fsas.emplace_back(public_fsa);
//    fsas.emplace_back(return_fsa);
//    fsas.emplace_back(short_fsa);
//    fsas.emplace_back(static_fsa);
//    fsas.emplace_back(strictfp_fsa);
//    fsas.emplace_back(super_fsa);
//    fsas.emplace_back(switch_fsa);
//    fsas.emplace_back(synchronized_fsa);
//    fsas.emplace_back(this_fsa);
//    fsas.emplace_back(throw_fsa);
//    fsas.emplace_back(transient_fsa);
//    fsas.emplace_back(try_fsa);
//    fsas.emplace_back(void_fsa);
//    fsas.emplace_back(volatile_fsa);
//    fsas.emplace_back(while_fsa);
//    fsas.emplace_back(true_fsa);
//    fsas.emplace_back(false_fsa);
//    fsas.emplace_back(null_fsa);
//    fsas.emplace_back(var_fsa);
//    fsas.emplace_back(identifier_fsa);
//
//    fsa::fsa_type nfa;
//    fsa::combine(fsas, nfa, status);
//
//    fsa::fsa_type dfa;
//    fsa_algorithm::nfa_to_dfa(nfa, dfa, status);
//
//    fsa::fsa_type mindfa;
//    fsa_algorithm::minimize_dfa(dfa, mindfa);
//
//    const int16_t identifier_rank = 1;
//
//    for(const auto& state_kvp : (*mindfa).states())
//    {
//        auto& state(state_kvp.second);
//
//        for(const auto& transition_kvp : (*state).transitions())
//        {
//            auto& transition(transition_kvp.second);
//
//            const auto& end_state((*mindfa).states()[(*transition).end_state()]);
//
//            if((*end_state).token() == static_cast<std::size_t>(token_type::traits::type::identifier))
//            {
//                if((*transition).predicate().compare(IDENTIFIER_START) == 0 ||
//                   (*transition).predicate().compare(IDENTIFIER_PART) == 0)
//                {
//                    (*transition).rank() = identifier_rank;
//                }
//            }
//        }
//    }
//
//    fsa_codegen::generate<token_type>(mindfa, file_name, label_prefix);
//
//    fsa_visualization::generate_graphviz_file(nfa, string_type(LR"(d:\tmp\FSA\nfa.dot)"));            //??
//    fsa_visualization::generate_graphviz_file(dfa, string_type(LR"(d:\tmp\FSA\dfa.dot)"));
//    fsa_visualization::generate_graphviz_file(mindfa, string_type(LR"(d:\tmp\FSA\mindfa.dot)"));
//}
//
//typename java_lexical_analyzer::datum_type java_lexical_analyzer::consume_unicode_escape(bool check_for_surrogates)
//{
//    datum_type result = text::invalid_codepoint;
//
//    datum_type codepoint;
//
//    do
//    {
//        codepoint = *++my_ptr;
//    }
//    while(codepoint == L'u');
//
//    datum_type calculated = 0;
//
//    std::size_t i = 0;
//    std::size_t n = 4;
//
//    for(; i < n && my_ptr < my_end_content; i++)
//    {
//        if(!std::isxdigit(codepoint))
//        {
//            my_ptr--; // rollback illegally consumed codepoint
//            break;
//        }
//
//        codepoint = text::ascii_numbers[codepoint]; // u_digit(codepoint, 16);
//        calculated = (calculated << 4) | codepoint;
//
//        if(i < n - 1)
//        {
//            codepoint = *++my_ptr;
//        }
//    }
//
//    if(i == n && my_ptr < my_end_content)
//    {
//        codepoint = calculated;
//
//        if(check_for_surrogates && text::is_high_surrogate(codepoint))
//        {
//            datum_type high_surrogate = codepoint;
//
//            codepoint = *++my_ptr;
//
//            if(codepoint == datum_type('\\'))
//            {
//                datum_type low_surrogate = consume_unicode_escape(false);
//
//                if(text::is_low_surrogate(low_surrogate))
//                {
//                    result = text::make_codepoint(high_surrogate, low_surrogate);
//                }
//                else
//                {
//                    //log_error(L"Invalid unicode escape at location '%ld, %ld', invalid low surrogate.", CURRENT_LOCATION);
//                    //diagnostics.add_error(format(L"Invalid unicode escape at location '%ld, %ld', invalid low surrogate.", CURRENT_LOCATION));
//                }
//            }
//            else
//            {
//                //log_error(L"Invalid unicode escape at location '%ld, %ld', missing low surrogate.", CURRENT_LOCATION);
//                my_ptr--; // unget
//            }
//        }
//        else
//        {
//            result = codepoint;
//        }
//    }
//    else
//    {
//        //log_error(L"Invalid unicode escape at location '%ld, %ld', invalid length.", CURRENT_LOCATION);
//    }
//
//    return result;
//}
//
//typename java_lexical_analyzer::datum_type java_lexical_analyzer::advance()
//{
//    datum_type result = text::invalid_codepoint;
//
//    if(my_ptr < my_end_content)
//    {
//        my_unicode = false;
//        my_unicode_length = 0;
//
//        if(*my_ptr == datum_type(L'\\'))
//        {
//            if(my_unicode_backslash_count % 2 == 0)
//            {
//                const datum_type *ptr(my_ptr);
//
//                if(*(my_ptr + 1) == datum_type(L'u'))
//                {
//                    my_ptr++; // advance to 'u'
//
//                    result = consume_unicode_escape();
//
//                    if(result != text::invalid_codepoint)
//                    {
//                        my_unicode = true;
//                        my_unicode_length = static_cast<uint8_t>(std::ptrdiff_t(my_ptr - ptr));
//                    }
//
//                    my_ptr++;
//                }
//                else
//                {
//                    my_unicode_backslash_count++;
//                    result = *++my_ptr;
//                }
//            }
//            else
//            {
//                my_unicode_backslash_count++;
//                result = *++my_ptr;
//            }
//        }
//        else
//        {
//            my_unicode_backslash_count = 0;
//            result = *++my_ptr;
//        }
//
//        if(my_unicode)
//        {
//            //?? line-map?
//        }
//    }
//
//    if(my_ptr > my_end_content)
//    {
//        my_ptr = my_end_content;
//    }
//
//    return result;
//}
//
//typename java_lexical_analyzer::datum_type java_lexical_analyzer::peek(uint16_t k)
//{
//    datum_type result = text::invalid_codepoint;
//
//    const datum_type *ptr(my_ptr);
//    bool unicode = my_unicode;
//
//    for(uint16_t i = 0; i < k; i++)
//    {
//        result = advance();
//    }
//
//    my_ptr = ptr;
//    my_unicode = unicode;
//
//    return result;
//}
//
//void java_lexical_analyzer::rewind()
//{
//    if(my_unicode)
//    {
//        my_ptr -= my_unicode_length;
//    }
//    else
//    {
//        my_ptr--;
//    }
//}
//
////// OpenJDK 9+ for more details ...
////void java_lexical_analyzer::build_line_map()
////{
////    const datum_type *ptr(my_ptr);
////    const datum_type *start_content(my_start_content);
////    const datum_type *end_content(my_end_content);
////
////    my_tab_map.resize((*my_content).count());
////
////    std::size_t k = 0;
////
////    line_map_type line_map(std::make_unique<loc_type[]>((*my_content).count()));
////
////    while(ptr < end_content)
////    {
////        line_map[k++] = std::ptrdiff_t(ptr - start_content);
////
////        do
////        {
////            if(*ptr == L'\r' || *ptr == L'\n')
////            {
////                if((*ptr == L'\r' && *(ptr + 1) == L'\n') || (*ptr == L'\n' && *(ptr + 1) == L'\r'))
////                {
////                    ptr += 2;
////                }
////                else
////                {
////                    ptr++;
////                }
////
////                break;
////            }
////            else if(*ptr == L'\t')
////            {
////                my_tab_map[std::ptrdiff_t(ptr - start_content)] = true;
////            }
////        }
////        while(++ptr < end_content);
////    }
////
////    my_line_map = std::make_unique<loc_type[]>(k);
////    my_line_map_size = k;
////
////    memcpy(my_line_map.get(), line_map.get(), k * sizeof(loc_type));
////}
////
////// OpenJDK 9+ for more details ...
////loc_type java_lexical_analyzer::find_line_number(loc_type position)
////{
////    loc_type result = 0;
////
////    if(my_line_map_size > 0)
////    {
////        if(position == my_cached_line_position)
////        {
////            result = my_cached_line;
////        }
////        else
////        {
////            my_cached_line_position = position;
////
////            loc_type low = 0;
////            loc_type high = my_line_map_size - 1;
////
////            while(low <= high)
////            {
////                loc_type mid = (low + high) >> 1;
////                loc_type val = my_line_map[mid];
////
////                if(val < position)
////                {
////                    low = mid + 1;
////                }
////                else if(val > position)
////                {
////                    high = mid - 1;
////                }
////                else
////                {
////                    result = my_cached_line = mid + 1;
////                    goto done;
////                }
////            }
////
////            result = my_cached_line = low;
////done:
////            ;
////        }
////    }
////
////    return result;
////}
////
////// OpenJDK 9+ for more details ...
////loc_type java_lexical_analyzer::get_line_number(loc_type position)
////{
////    return find_line_number(position);
////}
////
////// OpenJDK 9+ for more details ...
////loc_type java_lexical_analyzer::get_column_number(loc_type position)
////{
////    loc_type result = 0;
////
////    if(my_line_map_size > 0)
////    {
////        loc_type offset = my_line_map[find_line_number(position) - 1]; // 1 is the first line
////        loc_type column = 0;
////
////        for(loc_type k = offset; k < position; k++)
////        {
////            if(my_tab_map[k])
////            {
////                column = ((column / my_tab_size) * my_tab_size) + my_tab_size;
////            }
////            else
////            {
////                column++;
////            }
////        }
////
////        result = column + 1; // 1 is the first column
////    }
////
////    return result;
////}
//
//void java_lexical_analyzer::calculate_indentation()
//{
//    const datum_type *ptr(my_ptr);
//
//    if(my_boll)
//    {
//        std::size_t indent = 0;
//
//        while(my_ptr < my_end_content && *my_ptr++ == L' ') // consider only spaces as indentation
//        {
//            indent++;
//        }
//
//        my_ptr--; // correct ptr
//
//        bool newline = *my_ptr == L'\n' || (*my_ptr == L'\r' && *(my_ptr + 1) == L'\n') || (*my_ptr == L'\n' && *(my_ptr + 1) == L'\r');
//
//        if(!(indent == 0 && newline)/* empty line */ && my_eoll)
//        {
//            if(indent == my_indents.top())
//            {
//                // same indent, restart from the begining of the line
//                // splits to two parts  '             ' and 'line5'
//                my_ptr = ptr;
//            }
//            else if(indent > my_indents.top())
//            {
//                my_indedents++;
//                my_indents.push(indent);
//            }
//            else // if(indent < my_indents[my_indent])
//            {
//                while(!my_indents.empty() && indent < my_indents.top())
//                {
//                    my_indents.pop();
//                    my_indedents--;
//                }
//            }
//
//            if(indent != my_indents.top())
//            {
//                //?? error indent/dedent
//                my_ptr = ptr;
//                my_indedents = 0;
//                my_token.type = token_type::traits::type::unknown;
//            }
//        }
//
//        my_boll = false;
//        my_eoll = false;
//    }
//
//    if(my_indedents != 0)
//    {
//        // def
//        //     line 1
//        //   
//        //     line2
//        //         line3
//        //             line4
//        //             line5 <-- DEDENT DEDENT
//        //     line6
//        //     line7 <-- DEDENT
//        // end
//        if(my_indedents < 0)
//        {
//            my_indedents++;
//            my_token.type = token_type::traits::type::dedent;
//        }
//        else
//        {
//            my_indedents--;
//            my_token.type = token_type::traits::type::indent;
//        }
//
//        my_token.flags.modify_flags(token_type::flags_type::synthetic, token_type::flags_type::genuine);
//    }
//}
//
//#ifdef _WINDOWS_
//#   pragma warning( disable : 4102 )
//#endif
//void java_lexical_analyzer::next_lexeme_impl()
//{
//    if(my_ptr < my_end_content)
//    {
//        calculate_indentation();
//
//        if(my_token.type == token_type::traits::type::indent || my_token.type == token_type::traits::type::dedent)
//        {
//            return;
//        }
//
//        datum_type codepoint = current();
//
//        if(text::is_java_identifier_start(codepoint))
//        {
//            // identifier or keyword
//            // !!! -- generated code -- !!!
//
//
//            
//_q_idkws_0:
//            if(my_ptr >= my_end_content)
//                goto _eos_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_2;
//            }
//            else if(codepoint == L'b')
//            {
//                codepoint = advance();
//                goto _q_idkws_3;
//            }
//            else if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_4;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_idkws_5;
//            }
//            else if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_6;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_idkws_7;
//            }
//            else if(codepoint == L'g')
//            {
//                codepoint = advance();
//                goto _q_idkws_8;
//            }
//            else if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_16;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_9;
//            }
//            else if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_17;
//            }
//            else if(codepoint == L'p')
//            {
//                codepoint = advance();
//                goto _q_idkws_18;
//            }
//            else if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_19;
//            }
//            else if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_32;
//            }
//            else if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_33;
//            }
//            else if(codepoint == L'v')
//            {
//                codepoint = advance();
//                goto _q_idkws_34;
//            }
//            else if(codepoint == L'w')
//            {
//                codepoint = advance();
//                goto _q_idkws_35;
//            }
//            else if(text::is_java_identifier_start(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_1:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_2:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'b')
//            {
//                codepoint = advance();
//                goto _q_idkws_37;
//            }
//            else if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_38;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_3:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_11;
//            }
//            else if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_20;
//            }
//            else if(codepoint == L'y')
//            {
//                codepoint = advance();
//                goto _q_idkws_43;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_4:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_58;
//            }
//            else if(codepoint == L'h')
//            {
//                codepoint = advance();
//                goto _q_idkws_60;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_61;
//            }
//            else if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_73;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_5:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_84;
//            }
//            else if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_192;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_6:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_39;
//            }
//            else if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_87;
//            }
//            else if(codepoint == L'x')
//            {
//                codepoint = advance();
//                goto _q_idkws_44;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_7:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_91;
//            }
//            else if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_74;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_12;
//            }
//            else if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_99;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_8:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_47;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_9:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_75;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_10:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_82;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_11:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_93;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_12:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_65;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_13:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_143;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_14:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_162;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_15:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_189;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_16:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_idkws_193;
//            }
//            else if(codepoint == L'm')
//            {
//                codepoint = advance();
//                goto _q_idkws_100;
//            }
//            else if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_102;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_17:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_48;
//            }
//            else if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_103;
//            }
//            else if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_92;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_18:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_104;
//            }
//            else if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_113;
//            }
//            else if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_114;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_19:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_51;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_20:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_62;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_21:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_72;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_22:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_145;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_23:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_79;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_24:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_81;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_25:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_83;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_26:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_161;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_27:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_167;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_28:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_108;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_29:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_15;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_30:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_186;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_31:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_191;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_32:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'h')
//            {
//                codepoint = advance();
//                goto _q_idkws_13;
//            }
//            else if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_116;
//            }
//            else if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_101;
//            }
//            else if(codepoint == L'w')
//            {
//                codepoint = advance();
//                goto _q_idkws_117;
//            }
//            else if(codepoint == L'y')
//            {
//                codepoint = advance();
//                goto _q_idkws_76;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_33:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'h')
//            {
//                codepoint = advance();
//                goto _q_idkws_127;
//            }
//            else if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_128;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_34:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_129;
//            }
//            else if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_130;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_35:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'h')
//            {
//                codepoint = advance();
//                goto _q_idkws_118;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_36:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'h')
//            {
//                codepoint = advance();
//                goto _q_idkws_149;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_37:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_52;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_38:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_22;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_39:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_134;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_40:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_155;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_41:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_157;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_42:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_125;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_43:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_131;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_44:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_23;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_45:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_28;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_46:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_30;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_47:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_136;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_48:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_120;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_49:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_124;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_50:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_119;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_51:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_88;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_52:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_144;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_53:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_68;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_54:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_109;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_55:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_180;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_56:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_86;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_57:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_188;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_58:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_132;
//            }
//            else if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_105;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_59:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_156;
//            }
//            else if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_122;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_60:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_133;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_61:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_40;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_62:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_153;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_63:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_89;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_64:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_158;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_65:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_159;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_66:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_50;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_67:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_55;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_68:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_77;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_69:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_170;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_70:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_110;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_71:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_112;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_72:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_175;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_73:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_59;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_74:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_64;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_75:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_138;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_76:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_106;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_77:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_111;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_78:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_42;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_79:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_166;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_80:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_90;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_81:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_57;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_82:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_126;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_83:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_187;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_84:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_idkws_63;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_85:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_idkws_71;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_86:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_idkws_183;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_87:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_135;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_88:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_148;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_89:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_97;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_90:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_182;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_91:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_41;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_92:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_139;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_93:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_21;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_94:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_123;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_95:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_163;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_96:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_165;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_97:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_176;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_98:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_184;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_99:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_194;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_100:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'p')
//            {
//                codepoint = advance();
//                goto _q_idkws_137;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_101:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'p')
//            {
//                codepoint = advance();
//                goto _q_idkws_26;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_102:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_53;
//            }
//            else if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_195;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_103:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'w')
//            {
//                codepoint = advance();
//                goto _q_idkws_196;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_104:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_140;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_105:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_154;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_106:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_36;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_107:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_56;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_108:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_46;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_109:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_174;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_110:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_181;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_111:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_29;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_112:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_185;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_113:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_141;
//            }
//            else if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_45;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_114:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'b')
//            {
//                codepoint = advance();
//                goto _q_idkws_94;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_115:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'b')
//            {
//                codepoint = advance();
//                goto _q_idkws_96;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_116:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_49;
//            }
//            else if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_121;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_117:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_54;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_118:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_95;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_119:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_98;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_120:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_142;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_121:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_107;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_122:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_80;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_123:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_171;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_124:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_173;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_125:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_25;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_126:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_190;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_127:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_150;
//            }
//            else if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_14;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_128:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_idkws_78;
//            }
//            else if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_151;
//            }
//            else if(codepoint == L'y')
//            {
//                codepoint = advance();
//                goto _q_idkws_197;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_129:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_198;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_130:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'i')
//            {
//                codepoint = advance();
//                goto _q_idkws_152;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_66;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_131:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_199;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_132:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_200;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_133:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_201;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_134:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_202;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_135:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'm')
//            {
//                codepoint = advance();
//                goto _q_idkws_203;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_136:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_204;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_137:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_27;
//            }
//            else if(codepoint == L'o')
//            {
//                codepoint = advance();
//                goto _q_idkws_146;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_138:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'g')
//            {
//                codepoint = advance();
//                goto _q_idkws_205;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_139:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_206;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_140:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'k')
//            {
//                codepoint = advance();
//                goto _q_idkws_69;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_141:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'v')
//            {
//                codepoint = advance();
//                goto _q_idkws_67;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_142:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'v')
//            {
//                codepoint = advance();
//                goto _q_idkws_169;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_143:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_160;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_144:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_70;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_145:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_164;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_146:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_168;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_147:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_85;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_148:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_172;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_149:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_10;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_150:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_207;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_151:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_208;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_152:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_idkws_209;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_153:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'k')
//            {
//                codepoint = advance();
//                goto _q_idkws_210;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_154:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'h')
//            {
//                codepoint = advance();
//                goto _q_idkws_211;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_155:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_212;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_156:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_213;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_157:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_214;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_158:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_215;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_159:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_216;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_160:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_217;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_161:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'r')
//            {
//                codepoint = advance();
//                goto _q_idkws_218;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_162:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'w')
//            {
//                codepoint = advance();
//                goto _q_idkws_219;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_163:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_220;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_164:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_221;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_165:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_222;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_166:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_idkws_177;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_167:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'm')
//            {
//                codepoint = advance();
//                goto _q_idkws_24;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_168:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_223;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_169:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_224;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_170:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'g')
//            {
//                codepoint = advance();
//                goto _q_idkws_179;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_171:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_225;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_172:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_226;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_173:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_idkws_227;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_174:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'h')
//            {
//                codepoint = advance();
//                goto _q_idkws_228;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_175:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'n')
//            {
//                codepoint = advance();
//                goto _q_idkws_229;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_176:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_230;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_177:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_231;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_178:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'y')
//            {
//                codepoint = advance();
//                goto _q_idkws_232;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_179:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_233;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_180:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_234;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_181:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_235;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_182:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_236;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_183:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'p')
//            {
//                codepoint = advance();
//                goto _q_idkws_237;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_184:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_238;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_185:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_239;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_186:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_idkws_240;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_187:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L't')
//            {
//                codepoint = advance();
//                goto _q_idkws_241;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_188:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L's')
//            {
//                codepoint = advance();
//                goto _q_idkws_242;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_189:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_idkws_243;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_190:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'z')
//            {
//                codepoint = advance();
//                goto _q_idkws_31;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_191:
//            // final state - [identifier]
//            my_token.type = token_type::traits::type::identifier;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_idkws_244;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_192:
//            // final state - [do_kw]
//            my_token.type = token_type::traits::type::do_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'u')
//            {
//                codepoint = advance();
//                goto _q_idkws_115;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_193:
//            // final state - [if_kw]
//            my_token.type = token_type::traits::type::if_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_194:
//            // final state - [for_kw]
//            my_token.type = token_type::traits::type::for_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_195:
//            // final state - [int_kw]
//            my_token.type = token_type::traits::type::int_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'e')
//            {
//                codepoint = advance();
//                goto _q_idkws_147;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_196:
//            // final state - [new_kw]
//            my_token.type = token_type::traits::type::new_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_197:
//            // final state - [try_kw]
//            my_token.type = token_type::traits::type::try_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_198:
//            // final state - [var_kw]
//            my_token.type = token_type::traits::type::var_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_199:
//            // final state - [byte_kw]
//            my_token.type = token_type::traits::type::byte_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_200:
//            // final state - [case_kw]
//            my_token.type = token_type::traits::type::case_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_201:
//            // final state - [char_kw]
//            my_token.type = token_type::traits::type::char_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_202:
//            // final state - [else_kw]
//            my_token.type = token_type::traits::type::else_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_203:
//            // final state - [enum_kw]
//            my_token.type = token_type::traits::type::enum_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_204:
//            // final state - [goto_kw]
//            my_token.type = token_type::traits::type::goto_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_205:
//            // final state - [long_kw]
//            my_token.type = token_type::traits::type::long_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_206:
//            // final state - [null_lit]
//            my_token.type = token_type::traits::type::null_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_207:
//            // final state - [this_kw]
//            my_token.type = token_type::traits::type::this_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_208:
//            // final state - [true_lit]
//            my_token.type = token_type::traits::type::true_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_209:
//            // final state - [void_kw]
//            my_token.type = token_type::traits::type::void_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_210:
//            // final state - [break_kw]
//            my_token.type = token_type::traits::type::break_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_211:
//            // final state - [catch_kw]
//            my_token.type = token_type::traits::type::catch_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_212:
//            // final state - [class_kw]
//            my_token.type = token_type::traits::type::class_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_213:
//            // final state - [const_kw]
//            my_token.type = token_type::traits::type::const_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_214:
//            // final state - [false_lit]
//            my_token.type = token_type::traits::type::false_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_215:
//            // final state - [final_kw]
//            my_token.type = token_type::traits::type::final_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_idkws_178;
//            }
//            else if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_216:
//            // final state - [float_kw]
//            my_token.type = token_type::traits::type::float_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_217:
//            // final state - [short_kw]
//            my_token.type = token_type::traits::type::short_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_218:
//            // final state - [super_kw]
//            my_token.type = token_type::traits::type::super_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_219:
//            // final state - [throw_kw]
//            my_token.type = token_type::traits::type::throw_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_220:
//            // final state - [while_kw]
//            my_token.type = token_type::traits::type::while_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_221:
//            // final state - [assert_kw]
//            my_token.type = token_type::traits::type::assert_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_222:
//            // final state - [double_kw]
//            my_token.type = token_type::traits::type::double_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_223:
//            // final state - [import_kw]
//            my_token.type = token_type::traits::type::import_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_224:
//            // final state - [native_kw]
//            my_token.type = token_type::traits::type::native_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_225:
//            // final state - [public_kw]
//            my_token.type = token_type::traits::type::public_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_226:
//            // final state - [return_kw]
//            my_token.type = token_type::traits::type::return_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_227:
//            // final state - [static_kw]
//            my_token.type = token_type::traits::type::static_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_228:
//            // final state - [switch_kw]
//            my_token.type = token_type::traits::type::switch_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_229:
//            // final state - [boolean_kw]
//            my_token.type = token_type::traits::type::boolean_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_230:
//            // final state - [default_kw]
//            my_token.type = token_type::traits::type::default_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_231:
//            // final state - [extends_kw]
//            my_token.type = token_type::traits::type::extends_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_232:
//            // final state - [finally_kw]
//            my_token.type = token_type::traits::type::finally_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_233:
//            // final state - [package_kw]
//            my_token.type = token_type::traits::type::package_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_234:
//            // final state - [private_kw]
//            my_token.type = token_type::traits::type::private_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_235:
//            // final state - [abstract_kw]
//            my_token.type = token_type::traits::type::abstract_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_236:
//            // final state - [continue_kw]
//            my_token.type = token_type::traits::type::continue_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_237:
//            // final state - [strictfp_kw]
//            my_token.type = token_type::traits::type::strictfp_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_238:
//            // final state - [volatile_kw]
//            my_token.type = token_type::traits::type::volatile_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_239:
//            // final state - [interface_kw]
//            my_token.type = token_type::traits::type::interface_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_240:
//            // final state - [protected_kw]
//            my_token.type = token_type::traits::type::protected_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_241:
//            // final state - [transient_kw]
//            my_token.type = token_type::traits::type::transient_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_242:
//            // final state - [implements_kw]
//            my_token.type = token_type::traits::type::implements_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_243:
//            // final state - [instanceof_kw]
//            my_token.type = token_type::traits::type::instanceof_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_q_idkws_244:
//            // final state - [synchronized_kw]
//            my_token.type = token_type::traits::type::synchronized_kw;
//            if(my_ptr >= my_end_content)
//                goto _exit_idkws;
//            if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))
//            {
//                codepoint = advance();
//                goto _q_idkws_1;
//            }
//            goto _exit_idkws;
//_eos_idkws:
//            my_token.type = token_type::traits::type::eos;
//_exit_idkws:
//            //my_ptr--;
//
//
//
//
//
//            
//            
//            
//
//
//
//
//
//
//
//
//
//
//
//
//            // !!! -- end of generated code -- !!!
//            if(my_token.type == token_type::traits::type::unknown)
//            {
//                log_error(L"Invalid identifier at '%ld, %ld'.", CURRENT_LOCATION);
//            }
//        }
//        else if(text::is_digit(codepoint))
//        {
//            // numeric literal
//            // !!! -- generated code -- !!!
//
//
//
//
//
//            
//            
//_q_num_0:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_1;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_19;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            goto _exit_num;
//_q_num_1:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            goto _exit_num;
//_q_num_2:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            goto _exit_num;
//_q_num_3:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_47;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_47;
//            }
//            goto _exit_num;
//_q_num_4:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            goto _exit_num;
//_q_num_5:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_41;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_41;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_41;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_41;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_41;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_41;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_41;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_41;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_5;
//            }
//            goto _exit_num;
//_q_num_6:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_20;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_6;
//            }
//            goto _exit_num;
//_q_num_7:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_7;
//            }
//            goto _exit_num;
//_q_num_8:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_8;
//            }
//            goto _exit_num;
//_q_num_9:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_9;
//            }
//            goto _exit_num;
//_q_num_10:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_10;
//            }
//            goto _exit_num;
//_q_num_11:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_11;
//            }
//            goto _exit_num;
//_q_num_12:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'A')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'B')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'C')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'P')
//            {
//                codepoint = advance();
//                goto _q_num_13;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'b')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'p')
//            {
//                codepoint = advance();
//                goto _q_num_13;
//            }
//            goto _exit_num;
//_q_num_13:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_16;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_16;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            goto _exit_num;
//_q_num_14:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_25;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_27;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_28;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_17;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_29;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_30;
//            }
//            goto _exit_num;
//_q_num_15:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_47;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_47;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_15;
//            }
//            goto _exit_num;
//_q_num_16:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            goto _exit_num;
//_q_num_17:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_14;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_17;
//            }
//            goto _exit_num;
//_q_num_18:
//            if(my_ptr >= my_end_content)
//                goto _eos_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_18;
//            }
//            goto _exit_num;
//_q_num_19:
//            // final state - [decimal_integer_lit]
//            my_token.type = token_type::traits::type::decimal_integer_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_25;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'B')
//            {
//                codepoint = advance();
//                goto _q_num_3;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_27;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_28;
//            }
//            else if(codepoint == L'L')
//            {
//                codepoint = advance();
//                goto _q_num_22;
//            }
//            else if(codepoint == L'X')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_5;
//            }
//            else if(codepoint == L'b')
//            {
//                codepoint = advance();
//                goto _q_num_3;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_29;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_30;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_num_23;
//            }
//            else if(codepoint == L'x')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            goto _exit_num;
//_q_num_20:
//            // final state - [decimal_integer_lit]
//            my_token.type = token_type::traits::type::decimal_integer_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_25;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_27;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_28;
//            }
//            else if(codepoint == L'L')
//            {
//                codepoint = advance();
//                goto _q_num_22;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_6;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_29;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_30;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_num_23;
//            }
//            goto _exit_num;
//_q_num_21:
//            // final state - [decimal_integer_lit]
//            my_token.type = token_type::traits::type::decimal_integer_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_25;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_21;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_36;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_37;
//            }
//            else if(codepoint == L'L')
//            {
//                codepoint = advance();
//                goto _q_num_22;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_7;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_38;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_39;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_num_23;
//            }
//            goto _exit_num;
//_q_num_22:
//            // final state - [decimal_integer_lit]
//            my_token.type = token_type::traits::type::decimal_integer_lit;
//            advance();
//            goto _exit_num;
//_q_num_23:
//            // final state - [decimal_integer_lit]
//            my_token.type = token_type::traits::type::decimal_integer_lit;
//            advance();
//            goto _exit_num;
//_q_num_24:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_31;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_32;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_8;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_33;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_34;
//            }
//            goto _exit_num;
//_q_num_25:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_24;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_31;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_32;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_33;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_34;
//            }
//            goto _exit_num;
//_q_num_26:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_25;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_36;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_37;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_9;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_38;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_39;
//            }
//            goto _exit_num;
//_q_num_27:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_28:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_29:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_30:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_31:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_32:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_33:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_34:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_35:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_35;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_31;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_32;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_10;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_33;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_34;
//            }
//            goto _exit_num;
//_q_num_36:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_37:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_38:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_39:
//            // final state - [decimal_floating_lit]
//            my_token.type = token_type::traits::type::decimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_40:
//            // final state - [octal_integer_lit]
//            my_token.type = token_type::traits::type::octal_integer_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_25;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_36;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_37;
//            }
//            else if(codepoint == L'L')
//            {
//                codepoint = advance();
//                goto _q_num_42;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_11;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_38;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_39;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_num_43;
//            }
//            goto _exit_num;
//_q_num_41:
//            // final state - [octal_integer_lit]
//            my_token.type = token_type::traits::type::octal_integer_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'+')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'-')
//            {
//                codepoint = advance();
//                goto _q_num_2;
//            }
//            else if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_25;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_40;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_26;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_27;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_4;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_28;
//            }
//            else if(codepoint == L'L')
//            {
//                codepoint = advance();
//                goto _q_num_42;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_5;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_29;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_30;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_num_43;
//            }
//            goto _exit_num;
//_q_num_42:
//            // final state - [octal_integer_lit]
//            my_token.type = token_type::traits::type::octal_integer_lit;
//            advance();
//            goto _exit_num;
//_q_num_43:
//            // final state - [octal_integer_lit]
//            my_token.type = token_type::traits::type::octal_integer_lit;
//            advance();
//            goto _exit_num;
//_q_num_44:
//            // final state - [hexadecimal_integer_lit]
//            my_token.type = token_type::traits::type::hexadecimal_integer_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'.')
//            {
//                codepoint = advance();
//                goto _q_num_12;
//            }
//            else if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'A')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'B')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'C')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'E')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'L')
//            {
//                codepoint = advance();
//                goto _q_num_45;
//            }
//            else if(codepoint == L'P')
//            {
//                codepoint = advance();
//                goto _q_num_13;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'a')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'b')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'c')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_44;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_num_46;
//            }
//            else if(codepoint == L'p')
//            {
//                codepoint = advance();
//                goto _q_num_13;
//            }
//            goto _exit_num;
//_q_num_45:
//            // final state - [hexadecimal_integer_lit]
//            my_token.type = token_type::traits::type::hexadecimal_integer_lit;
//            advance();
//            goto _exit_num;
//_q_num_46:
//            // final state - [hexadecimal_integer_lit]
//            my_token.type = token_type::traits::type::hexadecimal_integer_lit;
//            advance();
//            goto _exit_num;
//_q_num_47:
//            // final state - [binary_integer_lit]
//            my_token.type = token_type::traits::type::binary_integer_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_47;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_47;
//            }
//            else if(codepoint == L'L')
//            {
//                codepoint = advance();
//                goto _q_num_48;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_15;
//            }
//            else if(codepoint == L'l')
//            {
//                codepoint = advance();
//                goto _q_num_49;
//            }
//            goto _exit_num;
//_q_num_48:
//            // final state - [binary_integer_lit]
//            my_token.type = token_type::traits::type::binary_integer_lit;
//            advance();
//            goto _exit_num;
//_q_num_49:
//            // final state - [binary_integer_lit]
//            my_token.type = token_type::traits::type::binary_integer_lit;
//            advance();
//            goto _exit_num;
//_q_num_50:
//            // final state - [hexadecimal_floating_lit]
//            my_token.type = token_type::traits::type::hexadecimal_floating_lit;
//            if(my_ptr >= my_end_content)
//                goto _exit_num;
//            if(codepoint == L'0')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'1')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'2')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'3')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'4')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'5')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'6')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'7')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'8')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'9')
//            {
//                codepoint = advance();
//                goto _q_num_50;
//            }
//            else if(codepoint == L'D')
//            {
//                codepoint = advance();
//                goto _q_num_51;
//            }
//            else if(codepoint == L'F')
//            {
//                codepoint = advance();
//                goto _q_num_52;
//            }
//            else if(codepoint == L'_')
//            {
//                codepoint = advance();
//                goto _q_num_18;
//            }
//            else if(codepoint == L'd')
//            {
//                codepoint = advance();
//                goto _q_num_53;
//            }
//            else if(codepoint == L'f')
//            {
//                codepoint = advance();
//                goto _q_num_54;
//            }
//            goto _exit_num;
//_q_num_51:
//            // final state - [hexadecimal_floating_lit]
//            my_token.type = token_type::traits::type::hexadecimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_52:
//            // final state - [hexadecimal_floating_lit]
//            my_token.type = token_type::traits::type::hexadecimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_53:
//            // final state - [hexadecimal_floating_lit]
//            my_token.type = token_type::traits::type::hexadecimal_floating_lit;
//            advance();
//            goto _exit_num;
//_q_num_54:
//            // final state - [hexadecimal_floating_lit]
//            my_token.type = token_type::traits::type::hexadecimal_floating_lit;
//            advance();
//            goto _exit_num;
//_eos_num:
//            my_token.type = token_type::traits::type::eos;
//_exit_num:
//            //my_ptr--;
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//            // !!! -- end of generated code -- !!!
//            if(my_token.type == token_type::traits::type::unknown)
//            {
//                log_error(L"Invalid numerical literal at '%ld, %ld'. Only ASCII allowed in constants and floating literals use ASCII only.", CURRENT_LOCATION);
//            }
//        }
//        else
//        {
//            switch(codepoint)
//            {
//                case L'(':
//                    my_token.type = token_type::traits::type::left_paren_sep;
//                    advance();
//                    break;
//
//                case L')':
//                    my_token.type = token_type::traits::type::right_paren_sep;
//                    advance();
//                    break;
//
//                case L'{':
//                    my_token.type = token_type::traits::type::left_brace_sep;
//                    advance();
//                    break;
//
//                case L'}':
//                    my_token.type = token_type::traits::type::right_brace_sep;
//                    advance();
//                    break;
//
//                case L'[':
//                    my_token.type = token_type::traits::type::left_bracket_sep;
//                    advance();
//                    break;
//
//                case L']':
//                    my_token.type = token_type::traits::type::righ_bracket_sep;
//                    advance();
//                    break;
//
//                case L';':
//                    my_token.type = token_type::traits::type::semicolon_sep;
//                    advance();
//                    break;
//
//                case L',':
//                    my_token.type = token_type::traits::type::comma_sep;
//                    advance();
//                    break;
//
//                case L'.':
//                {
//                    // .  ...  or fraction part
//                    codepoint = advance();
//
//                    if(codepoint == L'.')
//                    {
//                        codepoint = advance();
//
//                        if(codepoint == L'.')
//                        {
//                            my_token.type = token_type::traits::type::dots_sep;
//                            advance();
//                        }
//                        else
//                        {
//                            log_error(L"Invalid ellipsis '...', missing '.'.");
//                        }
//                    }
//                    else if(text::is_digit(codepoint))
//                    {
//                        // fraction part
//                        goto _q_num_25;
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::dot_sep;
//                    }
//
//                    break;
//                }
//                case L'@':
//                    my_token.type = token_type::traits::type::at_sep;
//                    advance();
//                    break;
//
//                case L':':
//                    // : ::
//                    codepoint = advance();
//
//                    if(codepoint == L':')
//                    {
//                        my_token.type = token_type::traits::type::colons_sep;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::colon_sep;
//                    }
//
//                    break;
//
//                case L'+':
//                {
//                    // + += ++
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::addition_assignment_op;
//                        advance();
//                    }
//                    else if(codepoint == L'+')
//                    {
//                        my_token.type = token_type::traits::type::increment_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::addition_op;
//                    }
//
//                    break;
//                }
//                case L'-':
//                {
//                    // - -= -- ->
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::subtraction_assignment_op;
//                        advance();
//                    }
//                    else if(codepoint == L'-')
//                    {
//                        my_token.type = token_type::traits::type::decrement_op;
//                        advance();
//                    }
//                    else if(codepoint == L'>')
//                    {
//                        my_token.type = token_type::traits::type::lambda_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::subtraction_op;
//                    }
//
//                    break;
//                }
//                case L'*':
//                    // * *=
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::multiplication_assignment_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::multiplication_op;
//                    }
//
//                    break;
//
//                case L'/':
//                {
//                    // /, /= or comments //,  /*
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::division_assignment_op;
//                        advance();
//                    }
//                    else if(codepoint == L'/')
//                    {
//                        do
//                        {
//                            codepoint = advance();
//
//                            if(codepoint == L'\r' || codepoint == L'\n')
//                            {
//                                break;
//                            }
//                        }
//                        while(my_ptr < my_end_content);
//
//                        my_token.type = token_type::traits::type::singleline_comments;
//                    }
//                    else if(codepoint == L'*')
//                    {
//                        do
//                        {
//                            codepoint = advance();
//
//                            datum_type la_codepoint = peek();
//
//                            if((codepoint == L'*' && la_codepoint == L'/'))
//                            {
//                                advance(); // consume *
//                                advance(); // consume /
//                                break;
//                            }
//                        }
//                        while(my_ptr < my_end_content);
//
//                        my_token.type = token_type::traits::type::multiline_comments;
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::division_op;
//                    }
//
//                    break;
//                }
//                case L'&':
//                {
//                    // & && &=
//                    codepoint = advance();
//
//                    if(codepoint == L'&')
//                    {
//                        my_token.type = token_type::traits::type::logical_and_op;
//                        advance();
//                    }
//                    else if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::bitwise_assignment_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::bitwise_and_op;
//                    }
//
//                    break;
//                }
//                case L'|':
//                {
//                    // | || |=
//                    codepoint = advance();
//
//                    if(codepoint == L'|')
//                    {
//                        my_token.type = token_type::traits::type::logical_or_op;
//                        advance();
//                    }
//                    else if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::bitwise_or_assignment_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::bitwise_or_op;
//                    }
//
//                    break;
//                }
//                case L'^':
//                    // ^ ^=
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::bitwise_xor_assignment_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::bitwise_xor_op;
//                    }
//
//                    break;
//
//                case L'!':
//                    // ! !=
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::logical_not_assignment_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::logical_not_op;
//                    }
//
//                    break;
//
//                case L'%':
//                    // % %=
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::module_assignment_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::module_op;
//                    }
//
//                    break;
//
//                case L'=':
//                    // = ==
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::equality_op;
//                        advance();
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::assignment_op;
//                    }
//
//                    break;
//
//                case L'~':
//                    my_token.type = token_type::traits::type::bitwise_not_op;
//                    advance();
//                    break;
//
//                case L'?':
//                    my_token.type = token_type::traits::type::ternary_true_op;
//                    advance();
//                    break;
//
//                case L'>':
//                {
//                    // > >=  > >  >> >>=  > > >  >>> >>>=
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::greater_than_equal_op;
//                        advance();
//                    }
//                    else if(codepoint == L'>')
//                    {
//                        codepoint = advance();
//
//                        if(codepoint == L'>')
//                        {
//                            codepoint = advance();
//
//                            if(codepoint == L'=')
//                            {
//                                my_token.type = token_type::traits::type::unsigned_right_shift_assignment_op;
//                                advance();
//                            }
//                            else
//                            {
//                                my_token.type = token_type::traits::type::unsigned_right_shift_op;
//                            }
//                        }
//                        else if(codepoint == L'=')
//                        {
//                            my_token.type = token_type::traits::type::signed_right_shift_assignment_op;
//                            advance();
//                        }
//                        else
//                        {
//                            my_token.type = token_type::traits::type::signed_right_shift_op;
//                        }
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::greater_than_op;
//                    }
//
//                    break;
//                }
//
//                case L'<':
//                {
//                    // < <=  < <  << <<=
//                    codepoint = advance();
//
//                    if(codepoint == L'=')
//                    {
//                        my_token.type = token_type::traits::type::less_than_equal_op;
//                        advance();
//                    }
//                    else if(codepoint == L'<')
//                    {
//                        codepoint = advance();
//
//                        if(codepoint == L'=')
//                        {
//                            my_token.type = token_type::traits::type::left_shift_assignment_op;
//                            advance();
//                        }
//                        else
//                        {
//                            my_token.type = token_type::traits::type::left_shift_op;
//                        }
//                    }
//                    else
//                    {
//                        my_token.type = token_type::traits::type::less_than_op;
//                    }
//
//                    break;
//                }
//                case L' ':
//                case L'\t':
//                case L'\f':
//                    do
//                    {
//                        codepoint = advance();
//
//                        if(codepoint != L' ' && codepoint != L'\t' && codepoint != L'\f')
//                        {
//                            break;
//                        }
//                    }
//                    while(my_ptr < my_end_content);
//
//                    my_token.type = token_type::traits::type::ws;
//                    break;
//
//                case L'\n':
//                    advance();
//
//                    if(current() == L'\r')
//                    {
//                        advance();
//                    }
//
//                    my_token.type = token_type::traits::type::eol;
//
//                    my_boll = true; //??
//                    my_eoll = true; //??
//
//                    break;
//
//                case L'\r':
//                    advance();
//
//                    if(current() == L'\n')
//                    {
//                        advance();
//                    }
//
//                    my_token.type = token_type::traits::type::eol;
//
//                    my_boll = true; //??
//                    my_eoll = true; //??
//
//                    break;
//
//                #define ADVANCE_CHAR_LITERAL(FUNC)                                                                                          \
//                    if(codepoint == L'\\' && *(my_ptr + 1) != L'u')                                                                         \
//                    {                                                                                                                       \
//                        codepoint = advance(); /* skip '\'*/                                                                                \
//                                                                                                                                            \
//                        switch(codepoint)                                                                                                   \
//                        {                                                                                                                   \
//                            case L'a':                                                                                                      \
//                                FUNC 0x07;                                                                                                  \
//                                break;                                                                                                      \
//                            case L'b':                                                                                                      \
//                                FUNC 0x08;                                                                                                  \
//                                break;                                                                                                      \
//                            case L't':                                                                                                      \
//                                FUNC 0x09;                                                                                                  \
//                                break;                                                                                                      \
//                            case L'v':                                                                                                      \
//                                FUNC 0x0B;                                                                                                  \
//                                break;                                                                                                      \
//                            case L'n':                                                                                                      \
//                                FUNC 0x0A;                                                                                                  \
//                                break;                                                                                                      \
//                            case L'f':                                                                                                      \
//                                FUNC 0x0C;                                                                                                  \
//                                break;                                                                                                      \
//                            case L'r':                                                                                                      \
//                                FUNC 0x0D;                                                                                                  \
//                                break;                                                                                                      \
//                            case L'"':                                                                                                      \
//                                FUNC 0x22;                                                                                                  \
//                                break;                                                                                                      \
//                            case L'\'':                                                                                                     \
//                                FUNC 0x27;                                                                                                  \
//                                break;                                                                                                      \
//                            case L'\\':                                                                                                     \
//                                FUNC 0x5C;                                                                                                  \
//                                break;                                                                                                      \
//                                                                                                                                            \
//                            case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7':                         \
//                            {                                                                                                               \
//                                datum_type ch = codepoint;                                                                                  \
//                                                                                                                                            \
//                                datum_type cd0 = ch; /* may start 3FF sequence */                                                           \
//                                datum_type cd1 = text::ascii_numbers[ch]; /* u_digit(codepoint, 8); */                                      \
//                                                                                                                                            \
//                                ch = advance();                                                                                             \
//                                                                                                                                            \
//                                if(ch >= L'0' && ch <= L'7')                                                                                \
//                                {                                                                                                           \
//                                    cd1 = cd1 * 8 + text::ascii_numbers[ch]; /* u_digit(codepoint, 8); */                                   \
//                                                                                                                                            \
//                                    ch = advance();                                                                                         \
//                                                                                                                                            \
//                                    if(cd0 <= L'3' && ch >= L'0' && ch <= L'7')                                                             \
//                                    {                                                                                                       \
//                                        cd1 = cd1 * 8 + text::ascii_numbers[ch]; /* u_digit(codepoint, 8); */                               \
//                                    }                                                                                                       \
//                                    else                                                                                                    \
//                                    {                                                                                                       \
//                                        rewind();                                                                                           \
//                                    }                                                                                                       \
//                                }                                                                                                           \
//                                else                                                                                                        \
//                                {                                                                                                           \
//                                    rewind();                                                                                               \
//                                }                                                                                                           \
//                                                                                                                                            \
//                                FUNC cd1;                                                                                                   \
//                                                                                                                                            \
//                                break;                                                                                                      \
//                            }                                                                                                               \
//                                                                                                                                            \
//                            default:                                                                                                        \
//                                erroneous = true;                                                                                           \
//                                log_error(L"Invalid escape character '\\%s'.", text::codepoint_to_string(codepoint).c_str());               \
//                                break;                                                                                                      \
//                        }                                                                                                                   \
//                    }                                                                                                                       \
//                    else                                                                                                                    \
//                    {                                                                                                                       \
//                        FUNC codepoint;                                                                                                     \
//                    }
//
//                case L'\'':
//                {
//                    // char literal
//                    bool erroneous = false;
//
//                    codepoint = advance();
//
//                    if(codepoint == L'\'')
//                    {
//                        erroneous = true;
//                        log_error(L"Character literal is empty, at location '%ld, %ld'.", CURRENT_LOCATION);
//
//                        advance();
//                    }
//                    else
//                    {
//                        if(codepoint == L'\r' || codepoint == L'\n')
//                        {
//                            erroneous = true;
//                            log_error(L"Illegal end of line in character literal, at location '%ld, %ld'.", CURRENT_LOCATION);
//                        }
//                        else
//                        {
//                            ADVANCE_CHAR_LITERAL(my_token.literal =)
//
//                            codepoint = advance();
//
//                            if(my_unicode)
//                            {
//                                codepoint = advance();
//                            }
//                        }
//
//                        if(!erroneous && codepoint != L'\'')
//                        {
//                            erroneous = true;
//                            log_error(L"Unclosed character literal, too many charactaers, at location '%ld, %ld'.", CURRENT_LOCATION);
//                        }
//
//                        if(!erroneous)
//                        {
//                            my_token.type = token_type::traits::type::char_lit;
//                            advance();
//                        }
//                    }
//
//                    break;
//                }
//                case L'"':
//                {
//                    // string literal
//                    std::basic_string<datum_type> value;
//
//                    bool erroneous = false;
//
//                    codepoint = advance();
//
//                    while(my_ptr < my_end_content && codepoint != L'"' && codepoint != L'\r' && codepoint != L'\n')
//                    {
//                        ADVANCE_CHAR_LITERAL(value +=)
//                        codepoint = advance();
//                    }
//
//                    if(codepoint != L'\"')
//                    {
//                        erroneous = true;
//                        log_error(L"Unclosed string literal, at location '%ld, %ld'.", CURRENT_LOCATION);
//                    }
//
//                    if(!erroneous)
//                    {
//                        my_token.type = token_type::traits::type::string_lit;
//
//                        my_token.literal = value;
//
//                        advance();
//                    }
//
//                    break;
//                }
//                default:
//                {
//                    advance();
//                    log_error(L"Invalid character '%s' at location '%ld, %ld'.", text::codepoint_to_string(codepoint).c_str(), CURRENT_LOCATION);
//                    break;
//                }
//            }
//        }
//    }
//    else
//    {
//        my_token.type = token_type::traits::type::eos;
//    }
//}

END_NAMESPACE
