//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __GRAMMAR_ALGORITHM_H__
#define __GRAMMAR_ALGORITHM_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class grammar_algorithm : private noncopyable
{
    public:
        using symbol_type = typename grammar::symbol_type;
        using symbols_type = typename grammar::symbols_type;

        using pool_type = typename grammar::pool_type;

        using rule_type = typename grammar::rule_type;
        using rules_type = typename grammar::rules_type;

        using set_type = typename grammar::set_type;
        using sets_type = typename grammar::sets_type;

    public:
        static void         create_production_combinations(const symbols_type& rhs,
                                                           const symbol_type& nonterminal,
                                                           std::vector<symbols_type>& new_productions);

        static void         calculate_longest_common_prefixes(rules_type& rules,
                                                              std::size_t prefix_size,
                                                              rules_type& factored_rules,
                                                              symbols_type& factored_prefix);

        static void         factor_rules(grammar& gr,
                                         const symbol_type& nonterminal,
                                         const rules_type& factored_rules,
                                         const symbols_type& factored_prefix,
                                         std::size_t& suffix_number,
                                         std::size_t& symbols_count,
                                         std::size_t& rules_count);

        static void         add_nonterminal_rule(grammar& gr, const symbol_type& nonterminal, const rule_type& production);
        static void         remove_nonterminal_rule(grammar& gr, const symbol_type& nonterminal, const rule_type& production);
        static void         clear_nonterminal_rules(grammar& gr, const symbol_type& nonterminal);

        static void         collect_nonterminals(const grammar& gr, std::vector<symbol_type>& nonterminals);
        static void         collect_terminals(const grammar& gr, std::vector<symbol_type>& terminals);

        static std::vector<std::vector<std::size_t>>
                            cartesian_product(const std::vector<std::vector<std::size_t>>& sets);

        static void         infix_operator(const sets_type& sets1, const sets_type& sets2, std::size_t k, sets_type& result);
        static void         infix_operator(const std::vector<sets_type>& sets, std::size_t k, sets_type& result);

        static bool         is_less(const set_type& seq1, const set_type& seq2);
        static void         make_vector_unique(typename grammar_algorithm::sets_type& sequence);

    public:
        static std::size_t  calculate_grammar_size(const grammar& gr);

        static void         build_nonterminals_rules(grammar& gr);

        static void         remove_broken_productions(grammar& gr);
        static void         remove_duplicate_productions(grammar& gr);

        static void         calculate_productive_nonterminals(grammar& gr);
        static void         remove_non_productive_nonterminals(grammar& gr);

        static void         remove_undefined_nonterminals(grammar& gr);

        static void         calculate_accessible_nonterminals(grammar& gr);
        static void         remove_inaccessable_nonterminals(grammar& gr);  // inaccessible, unreachable, unused nonterminals

        static void         remove_unit_productions(grammar& gr);           // remove unit/chain productions, loops, cycles

        static void         cleanup_grammar(grammar& gr);

        static void         build_nullability_set(grammar& gr);
        static void         remove_empty_productions(grammar& gr);          // make ε/λ free grammar

        static void         build_first_set(grammar& gr, uint8_t k, bool build_eff = false);
        static void         build_first_set(const symbols_type& symbols, uint8_t k, sets_type& result);

        static void         build_eff_set(const grammar& gr, const symbols_type& symbols, uint8_t k, sets_type& result);

        static void         build_follow_set(grammar& gr, uint8_t k);


        static void         build_first1_set(grammar& gr);
        static void         build_first1_set(const symbols_type& symbols, typename grammar_algorithm::sets_type& result);

        static void         build_follow1_set(grammar& gr);

        static void         build_la_set(grammar& gr, uint8_t k);
        static void         build_la_set(const rule_type& rule, uint8_t k, sets_type& result);
        static void         build_la_set(const symbols_type& symbols, int32_t nonterminal_index, uint8_t k, sets_type& result);
        static void         build_la_set(const symbols_type& symbols, const rule_type& rule, int32_t nonterminal_index, uint8_t k, sets_type& result);

        static void         remove_immediate_left_recursion(grammar& gr,
                                                            const symbol_type& nonterminal,
                                                            std::size_t& suffix_number,
                                                            std::size_t& symbols_count,
                                                            std::size_t& rules_count);
        static void         remove_left_recursion(grammar& gr);

        static void         apply_left_factoring(grammar& gr);

        static void         renumber_grammar(grammar& gr);

        static void         build_synch_set(const grammar& gr, set_type& result);
};

END_NAMESPACE

#endif // __GRAMMAR_ALGORITHM_H__
