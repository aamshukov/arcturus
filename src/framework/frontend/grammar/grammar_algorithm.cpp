//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/unicode.hpp>
#include <core/text.hpp>
#include <core/enum.hpp>

#include <core/domain_helper.hpp>

#include <core/factory.hpp>
#include <core/logger.hpp>

#include <core/visitor.hpp>
#include <core/tree.hpp>

#include <frontend/grammar/symbol.hpp>
#include <frontend/grammar/rule.hpp>
#include <frontend/grammar/grammar.hpp>

#include <frontend/grammar/grammar_visualization.hpp>
#include <frontend/grammar/grammar_algorithm.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

// α β ε λ ∅ ∈ ∉ Σ ∪ ⊕

std::size_t grammar_algorithm::calculate_grammar_size(const grammar& gr)
{
    // calculate grammar size, which is
    // sum of all (|α|+1) for all A -> α, which ∈ G
    log_info(L"Calculating grammar size ...");

    std::size_t result = 0;

    for(const auto& rule : gr.rules())
    {
        result += (*rule).rhs_nonterminal_count() + (*rule).rhs_terminal_count();
    }

    log_info(L"Calculated grammar size, size = %d.", result);

    return result;
}

void grammar_algorithm::build_nonterminals_rules(grammar& gr)
{
    log_info(L"Building nonterminals rules mapping ...");

    // collect nonterminals
    std::vector<symbol_type> nonterminals;

    collect_nonterminals(gr, nonterminals);

    // build
    for(const auto& nonterminal : nonterminals)
    {
        clear_nonterminal_rules(gr, nonterminal);

        for(const auto& production : gr.rules())
        {
            if((*(*production).lhs().front()).id() == (*nonterminal).id())
            {
                add_nonterminal_rule(gr, nonterminal, production);
            }
        }
    }

    log_info(L"Built nonterminals rules mapping.");
}

void grammar_algorithm::add_nonterminal_rule(grammar& gr,
                                             const typename grammar_algorithm::symbol_type& nonterminal,
                                             const typename grammar_algorithm::rule_type& production)
{
    auto it(gr.nt_rules().find((*nonterminal).name()));

    if(it == gr.nt_rules().end())
    {
        // add new entry
        rules_type new_rules;
        new_rules.emplace_back(production);
        gr.nt_rules().emplace(grammar::nts_rules_type::value_type((*nonterminal).name(), std::move(new_rules)));
    }
    else
    {
        // update existing entry
        auto& nt_rules((*it).second);
        nt_rules.emplace_back(production);
    }
}

void grammar_algorithm::remove_nonterminal_rule(grammar& gr,
                                                const typename grammar_algorithm::symbol_type& nonterminal,
                                                const typename grammar_algorithm::rule_type& production)
{
    auto it(gr.nt_rules().find((*nonterminal).name()));

    if(it != gr.nt_rules().end())
    {
        auto& nt_rules((*it).second);

        nt_rules.erase(std::find_if(nt_rules.begin(),
                                    nt_rules.end(),
                                    [&production](const auto& rule_kvp){ return (*rule_kvp).id() == (*production).id(); }));
    }
}

void grammar_algorithm::clear_nonterminal_rules(grammar& gr, const typename grammar_algorithm::symbol_type& nonterminal)
{
    auto it(gr.nt_rules().find((*nonterminal).name()));

    if(it != gr.nt_rules().end())
    {
        auto& nt_rules((*it).second);
        nt_rules.clear();
    }
}

void grammar_algorithm::remove_broken_productions(grammar& gr)
{
    // remove any rule which has broken Lhs(=0) or Rhs(=0)
    log_info(L"Removing broken rules, Lhs(=0) or Rhs(=0) ...");

    for(auto it = gr.rules().begin(); it != gr.rules().end();)
    {
        if((*(*it)).lhs().empty() || (*(*it)).rhs().empty())
        {
            log_info(L"Removing broken rule: %s ...", grammar_visualization::decorate_rule(*it).c_str());
            it = gr.rules().erase(it);
        }
        else
        {
            ++it;
        }
    }

    build_nonterminals_rules(gr);

    log_info(L"Removed broken rules, Lhs(=0) or Rhs(=0).");
}

void grammar_algorithm::remove_duplicate_productions(grammar& gr)
{
    log_info(L"Removing duplicate productions ...");

    for(auto it = gr.rules().begin(); it != gr.rules().end(); ++it)
    {
        for(auto it0 = gr.rules().begin(); it0 != gr.rules().end();)
        {
            if((*(*it0)).id() != (*(*it)).id() && **it0 == **it)
            {
                log_info(L"Removing duplicate production: %s ...", grammar_visualization::decorate_rule(*it0).c_str());
                it0 = gr.rules().erase(it0);
            }
            else
            {
                ++it0;
            }
        }
    }

    build_nonterminals_rules(gr);

    log_info(L"Removed duplicate productions.");
}

void grammar_algorithm::calculate_productive_nonterminals(grammar& gr)
{
    // The set of "terminal derivable" non-terminals can be computed by the following algorithm:
    //  (a) Set "terminal derivable" equal to the set of non-terminals appearing on the left side
    //      of productions of the form N -> t(erminals) / rhs must have terminals only
    //  (b) Until doing so adds no new non-terminals to "terminal derivation", examine each production
    //      in the grammar adding to "terminal derivable" all left-hand-sides of productions whose
    //      right-hand-side consist entirely of symbols in "terminal derivable"
    // +
    // Sudkamp, 3rd edition, p.117
    log_info(L"Calculating productive nonterminals ...");

    std::set<symbol_type, symbol::symbol_key_comparator> term_set;
    std::set<symbol_type, symbol::symbol_key_comparator> prev_set;

    // collect nonterminals
    std::vector<symbol_type> nonterminals;

    collect_nonterminals(gr, nonterminals);

    // init TERM
    // TERM = { A | there is a rule A -> w ∈ P with w ∈ Σ* }
    //                              rule with all terminals (including λ) in RHS
    for(const auto& gr_rule : gr.rules())
    {
        bool terminal_derivable = true;

        for(const auto& symb : (*gr_rule).rhs())
        {
            if((*symb).nonterminal())
            {
                terminal_derivable = false;
                break;
            }
        }

        if(terminal_derivable)
        {
            const auto& lhs_symb((*gr_rule).lhs().front());

            (*lhs_symb).productive() = true;
            term_set.emplace((*gr_rule).lhs().front());
        }
    }

    // calculate
    do
    {
        prev_set = term_set;

        for(const auto& nonterminal : nonterminals)
        {
            for(const auto& gr_rule : gr.rules())
            {
                if((*(*gr_rule).lhs().front()).id() == (*nonterminal).id())
                {
                    bool terminal_derivable = true;

                    for(const auto& symb : (*gr_rule).rhs())
                    {
                        // w ∈ (PREV ∪ Σ)* - all symbols in w either in PREV or terminals (including λ)
                        if(!(((*symb).nonterminal() && prev_set.find(symb) != prev_set.end()) || (*symb).terminal()))
                        {
                            terminal_derivable = false;
                            break;
                        }
                    }

                    if(terminal_derivable)
                    {
                        const auto& lhs_symb((*gr_rule).lhs().front());

                        // TERM = TERM ∪ { A }
                        (*lhs_symb).productive() = true;
                        term_set.emplace((*gr_rule).lhs().front());
                    }
                }
            }
        }
    }
    while(term_set != prev_set);

    log_info(L"Calculated productive nonterminals.");
}

void grammar_algorithm::remove_non_productive_nonterminals(grammar& gr)
{
    log_info(L"Removing non-productive nonterminals ...");

    calculate_productive_nonterminals(gr);

    for(const auto& symb_kvp : gr.pool())
    {
        const auto& pool_symb(symb_kvp.second);

        if((*pool_symb).nonterminal() && !(*pool_symb).productive())
        {
            for(auto it = gr.rules().begin(); it != gr.rules().end();)
            {
                bool remove_rule = false;

                for(const auto& symb : (*(*it)).lhs())
                {
                    if((*symb).id() == (*pool_symb).id())
                    {
                        log_info(L"Found non-productive nonterminal in LHS: %s ...", (*symb).name().c_str());
                        remove_rule = true;
                    }
                }

                for(const auto& symb : (*(*it)).rhs())
                {
                    if((*symb).id() == (*pool_symb).id())
                    {
                        log_info(L"Found non-productive nonterminal in RHS: %s ...", (*symb).name().c_str());
                        remove_rule = true;
                    }
                }

                if(remove_rule)
                {
                    log_info(L"Removing rule with non-productive nonterminal: %s ...", grammar_visualization::decorate_rule(*it).c_str());
                    it = gr.rules().erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    log_info(L"Removed non-productive nonterminals.");
}

void grammar_algorithm::remove_undefined_nonterminals(grammar& gr)
{
    log_info(L"Removing undefined nonterminals ...");

    // Undefined non-terminals (Grune and Jacobs)
    //  The right-hand sides of some rules may contain non-terminals for which no production rule is given
    //  - (non-terminals) do not appear on the left hand side of any rule.
    std::set<uint32_t> lhs_symbols; // all lhs symbols

    // collect all lhs symbols
    for(const auto& rule : gr.rules())
    {
        lhs_symbols.insert((*(*rule).lhs().front()).id());
    }

    // find rule which has undefined symbol(s)
    volatile bool completed = false;

    while(!completed)
    {
        completed = true;

        for(auto it = gr.rules().begin(); it != gr.rules().end();)
        {
            bool remove_rule = false;

            for(const auto& symb : (*(*it)).rhs())
            {
                if((*symb).nonterminal() && lhs_symbols.find((*symb).id()) == lhs_symbols.end())
                {
                    log_info(L"Found undefined nonterminal: %s ...", (*symb).name().c_str());
                    remove_rule = true;
                }
            }

            if(remove_rule)
            {
                log_info(L"Removing rule with undefined nonterminal: %s ...", grammar_visualization::decorate_rule(*it).c_str());
                it = gr.rules().erase(it);
                completed = false;
            }
            else
            {
                ++it;
            }
        }
    }

    build_nonterminals_rules(gr);

    log_info(L"Removedg undefined nonterminals.");
}

void grammar_algorithm::calculate_accessible_nonterminals(grammar& gr)
{
    // Sudkamp, 3rd edition, p.119
    log_info(L"Calculating accessible nonterminals ...");

    std::set<symbol_type, symbol::symbol_key_comparator> reach_set;

    // REACH = { S }
    (*gr.start_symbol()).accessible() = true;
    reach_set.emplace(gr.start_symbol());

    std::set<symbol_type, symbol::symbol_key_comparator> prev_set;

    do
    {
        std::set<symbol_type, symbol::symbol_key_comparator> new_set;

        std::set_difference(reach_set.begin(), reach_set.end(), prev_set.begin(), prev_set.end(), std::inserter(new_set, new_set.begin()));

        prev_set = reach_set;

        for(const auto& symb : new_set)
        {
            for(const auto& gr_rule : gr.rules())
            {
                if((*(*gr_rule).lhs().front()).id() == (*symb).id())
                {
                    for(const auto& rhs_symb : (*gr_rule).rhs())
                    {
                        if((*rhs_symb).nonterminal())
                        {
                            (*rhs_symb).accessible() = true;
                            reach_set.emplace(rhs_symb);
                        }
                    }
                }
            }
        }
    }
    while(reach_set != prev_set);

    log_info(L"Calculated accessible nonterminals.");
}

void grammar_algorithm::remove_inaccessable_nonterminals(grammar& gr)
{
    // ... in the end, the unmarked non-terminals are not reachable and their rules can be removed ...
    log_info(L"Removing inaccessibile nonterminals ...");

    calculate_accessible_nonterminals(gr);

    for(const auto& symb_kvp : gr.pool())
    {
        const auto& pool_symb(symb_kvp.second);

        if((*pool_symb).nonterminal() && !(*pool_symb).accessible())
        {
            for(auto it = gr.rules().begin(); it != gr.rules().end();)
            {
                bool remove_rule = false;

                for(const auto& symb : (*(*it)).lhs())
                {
                    if((*symb).id() == (*pool_symb).id())
                    {
                        log_info(L"Found inaccessible nonterminal in LHS: %s ...", (*symb).name().c_str());
                        remove_rule = true;
                    }
                }

                for(const auto& symb : (*(*it)).rhs())
                {
                    if((*symb).id() == (*pool_symb).id())
                    {
                        log_info(L"Found inaccessible nonterminal in RHS: %s ...", (*symb).name().c_str());
                        remove_rule = true;
                    }
                }

                if(remove_rule)
                {
                    log_info(L"Removing rule with inaccessible nonterminal: %s ...", grammar_visualization::decorate_rule(*it).c_str());
                    it = gr.rules().erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    build_nonterminals_rules(gr);

    log_info(L"Removed inaccessibile nonterminals.");
}

void grammar_algorithm::remove_unit_productions(grammar& gr)
{
    // Sudkamp, 3rd edition, p.114
    log_info(L"Removing unit productions ...");

    // collect nonterminals
    std::vector<symbol_type> nonterminals;

    collect_nonterminals(gr, nonterminals);

    // build CHAIN(A) sets
    std::map<symbol_type, std::set<symbol_type, symbol::symbol_key_comparator>, symbol::symbol_key_comparator> chain_sets;

    std::vector<rule_type> chain_rules;

    for(const auto& nonterminal : nonterminals)
    {
        std::set<symbol_type, symbol::symbol_key_comparator> chain_set;

        // CHAIN(A) = { A }
        chain_set.emplace(nonterminal);

        std::set<symbol_type, symbol::symbol_key_comparator> prev_set;

        do
        {
            std::set<symbol_type, symbol::symbol_key_comparator> new_set;

            std::set_difference(chain_set.begin(), chain_set.end(), prev_set.begin(), prev_set.end(), std::inserter(new_set, new_set.begin()));

            prev_set = chain_set;

            for(const auto& symb : new_set)
            {
                for(const auto& gr_rule : gr.rules())
                {
                    // for each rule B -> C
                    if((*(*gr_rule).lhs().front()).id() == (*symb).id() && (*gr_rule).rhs().size() == 1 && (*(*gr_rule).rhs().front()).nonterminal())
                    {
                        chain_set.emplace((*gr_rule).rhs().front());
                        chain_rules.emplace_back(gr_rule);
                    }
                }
            }
        }
        while(chain_set != prev_set);

        chain_set.erase(nonterminal); // simplify substitution
        chain_sets.emplace(nonterminal, chain_set);

        string_type chain_set_str(format(L"CHAIN(%s) = { ", (*nonterminal).name().c_str()));
        std::vector<symbol_type> symbols;
        std::for_each(chain_set.begin(), chain_set.end(), [&symbols](const auto& symb){ symbols.emplace_back(symb); });
        std::sort(symbols.begin(),
                  symbols.end(),
                  [] (const symbol_type& sym1, const symbol_type& sym2)
                  {
                       return (*sym1).id() < (*sym2).id();
                  });
        std::for_each(symbols.begin(), symbols.end(), [&chain_set_str](const auto& symb){ chain_set_str += (*symb).name() + L"  "; });
        chain_set_str += L"}";
        log_info(chain_set_str.c_str());
    }

    // remove chain productions
    for(const auto& chain_rule : chain_rules)
    {
        auto it = std::find(gr.rules().begin(), gr.rules().end(), chain_rule);

        if(it != gr.rules().end())
        {
            gr.rules().erase(it);
        }
    }

    // substitute productions
    chain_rules.clear();

    uint32_t chain_rules_count = 500000;

    for(const auto chain_set_kvp : chain_sets)
    {
        const auto& nonterminal(chain_set_kvp.first);
        const auto& chain_set(chain_set_kvp.second);

        // collect all productions from CHAIN(A) A -> B where B -> w and create a new production A -> w
        for(const auto& chain_symb : chain_set)
        {
            for(const auto& gr_rule : gr.rules())
            {
                if((*(*gr_rule).lhs().front()).id() == (*chain_symb).id())
                {
                    // add a new production A -> w
                    rule_type production(factory::create<rule>(static_cast<uint32_t>(chain_rules_count++), (*nonterminal).name())); // rule name is the LHS's symbol name

                    (*production).add_lhs_symbol(nonterminal);
                    std::for_each((*gr_rule).rhs().begin(), (*gr_rule).rhs().end(), [&production](const auto& symb){ (*production).add_rhs_symbol(symb); });

                    chain_rules.emplace_back(production);
                }
            }
        }
    }

    gr.rules().insert(gr.rules().end(), chain_rules.begin(), chain_rules.end());

    renumber_grammar(gr);

    build_nonterminals_rules(gr);

    log_info(L"Removing unit productions.");
}

void grammar_algorithm::cleanup_grammar(grammar& gr)
{
    log_info(L"Cleaning up grammar ...");

    remove_broken_productions(gr);
    remove_duplicate_productions(gr);

    // To clean up a grammar, it is necessary to first remove the non-productive nonterminals,
    // then the undefined ones and then the unused (inaccessible, unreachable) ones.
    // These three groups together are called useless non-terminals
    remove_non_productive_nonterminals(gr);
    remove_undefined_nonterminals(gr);
    remove_inaccessable_nonterminals(gr);

    remove_unit_productions(gr); // unit (chain) rules

    log_info(L"Cleaned up grammar.");
}

void grammar_algorithm::build_nullability_set(grammar& gr)
{
    // The set of nullable non-terminals can be computed by the following algorithm:
    //  (a) Set "nullable" equal to the set of non-terminals appearing on the left side
    //      of productions of the form N -> e
    //  (b) Until doing so adds no new non-terminals to "nullable", examine each production
    //      in the grammar adding to "nullable" all left-hand-sides of productions whose
    //      right-hand-side consist entirely of symbols in "nullable"
    // +
    // Sudkamp, 3rd edition, p.108
    log_info(L"Building nullability set ...");

    // collect nonterminals
    std::vector<symbol_type> nonterminals;

    collect_nonterminals(gr, nonterminals);

    std::set<symbol_type, symbol::symbol_key_comparator> null_set;

    // init NULL
    // NULL = { A | A -> λ ∈ P }
    for(const auto& gr_rule : gr.rules())
    {
        if((*gr_rule).empty())
        {
            null_set.emplace((*gr_rule).lhs().front());
        }
    }

    // calculate
    std::set<symbol_type, symbol::symbol_key_comparator> prev_set;

    do
    {
        prev_set = null_set;

        for(const auto& nonterminal : nonterminals)
        {
            for(const auto& gr_rule : gr.rules())
            {
                if((*(*gr_rule).lhs().front()).id() == (*nonterminal).id())
                {
                    // w ∈ PREV* - all symbols in w either nullable or λ
                    bool nullable = true;

                    for(const auto& symb : (*gr_rule).rhs())
                    {
                        if (!(((*symb).nonterminal() && prev_set.find(symb) != prev_set.end()) || (*symb).id() == (*symbol::epsilon).id()))
                        {
                            nullable = false;
                            break;
                        }
                    }

                    if(nullable)
                    {
                        const auto& lhs_symb((*gr_rule).lhs().front());

                        // NULL = NULL ∪ { A }
                        (*lhs_symb).nullable() = true;
                        null_set.emplace((*gr_rule).lhs().front());

                        break;
                    }
                }
            }
        }
    }
    while(null_set != prev_set);

    // collect nullable nonterminals
    std::vector<symbol_type> nullable_nonterminals;

    for(const auto& symb_kvp : gr.pool())
    {
        const auto& pool_symb(symb_kvp.second);

        if((*pool_symb).nonterminal() && (*pool_symb).nullable())
        {
            nullable_nonterminals.emplace_back(pool_symb);
        }
    }

    std::sort(nullable_nonterminals.begin(),
              nullable_nonterminals.end(),
              [] (const symbol_type& sym1, const symbol_type& sym2)
              {
                  return (*sym1).id() < (*sym2).id();
              });

    string_type nullability_set_decorated;

    nullability_set_decorated += L"  { ";

    for(const auto& nullable_nonterminal : nullable_nonterminals)
    {
        nullability_set_decorated += (*nullable_nonterminal).name() + L" ";
    }

    nullability_set_decorated += L"}";

    log_info(nullability_set_decorated.c_str());

    log_info(L"Built nullability set.");
}

void grammar_algorithm::remove_empty_productions(grammar& gr)
{
    log_info(L"Removing empty productions ...");

    // collect nullable nonterminals except S -> λ
    std::vector<symbol_type> nullable_nonterminals;

    const auto& start_symbol(gr.start_symbol());

    for(const auto& symb_kvp : gr.pool())
    {
        const auto& pool_symb(symb_kvp.second);

        if((*pool_symb).id() != (*start_symbol).id() && (*pool_symb).nonterminal() && (*pool_symb).nullable()) // ignore the start symbol S
        {
            nullable_nonterminals.emplace_back(pool_symb);
        }
    }

    std::sort(nullable_nonterminals.begin(),
              nullable_nonterminals.end(),
              [] (const symbol_type& sym1, const symbol_type& sym2)
              {
                  return (*sym1).id() < (*sym2).id();
              });

    // remove empty productions
    uint32_t new_productions_count = 900000;

    for(const auto& nullable_nonterminal : nullable_nonterminals)
    {
        // for each production containing A, replace it with several productions calculated by create_production_combinations()
        std::vector<rule_type> new_productions;

        for(const auto& gr_rule : gr.rules())
        {
            std::vector<symbols_type> productions;

            create_production_combinations((*gr_rule).rhs(), nullable_nonterminal, productions);

            if(!productions.empty())
            {
                // production contains nullable nonterminal(s) A
                symbol_type lhs_symbol((*gr_rule).lhs().front());

                for(const auto& rhs : productions)
                {
                    rule_type production(factory::create<rule>(static_cast<uint32_t>(new_productions_count++), (*lhs_symbol).name())); // rule name is the LHS's symbol name

                    (*production).add_lhs_symbol(lhs_symbol);
                    std::for_each(rhs.begin(), rhs.end(), [&production](const auto& symb){ (*production).add_rhs_symbol(symb); });

                    new_productions.emplace_back(production);
                }
            }
        }

        std::for_each(new_productions.begin(), new_productions.end(), [&gr](const auto& production){ gr.rules().emplace_back(production); });
        remove_duplicate_productions(gr);
    }

    // find A -> λ and discard
    for(auto it = gr.rules().begin(); it != gr.rules().end();)
    {
        if((*(*it)).empty())
        {
            log_info(L"Removing empty production: %s ...", grammar_visualization::decorate_rule(*it).c_str());
            it = gr.rules().erase(it);
        }
        else
        {
            ++it;
        }
    }

    // preserve S -> λ
    if((*start_symbol).nullable())
    {
        rule_type production(factory::create<rule>(static_cast<uint32_t>(new_productions_count), (*start_symbol).name())); // rule name is the LHS's symbol name

        (*production).add_lhs_symbol(start_symbol);
        (*production).add_rhs_symbol(symbol::epsilon);

        gr.rules().emplace_back(production);
    }

    renumber_grammar(gr);

    build_nonterminals_rules(gr);

    log_info(L"Removed empty productions.");
}

void grammar_algorithm::build_first_set(grammar& gr, uint8_t k, bool build_eff)
{
    // FIRST(k) set
    //
    // Based on AU, Russian edition, page 397 and on Sudkump p. 498
    //  1. for each a ∈ T do F'(a) = {a}
    //  2. for each A ∈ N do F(A) = {λ} if A is nullable or empty otherwise
    //  3. repeat
    //      3.1 for each A ∈ N do F'(A) = F(A)
    //      3.2 for each rule A -> u1 u2 ... uN with n > 0 do
    //          F(A) = F(A) ∪ TRUNKk(F'(u1) F'(u2) ... F'(uN))
    //     until F(A) = F'(A) for all A ∈ N
    //  4. FIRSTk(A) = F(A)
    //
    // FIRST set is list of lists for every non-terminal in grammar, size of "lists" is up to K
    //    ( )
    //     | \
    //     |   --- 
    //     |      |
    // (0,1..K) (0,1..K)
    //
    // EFF(k) set: FIRST(k)\{λ}
    // 
    // ... recall that the concatenation of the empty set ∅ with any set yields the empty set ∅:
    //      u ∅ = ∅ u = ∅
    // but union does not:
    //      u ∪ ∅ = ∅ ∪ u = u
    if(build_eff)
    {
        log_info(L"Building EFF set for k = %d ...", k);
    }
    else
    {
        log_info(L"Building FIRST set for k = %d ...", k);
    }

    // collect terminals and nonterminals
    std::vector<symbol_type> terminals;
    std::vector<symbol_type> nonterminals;

    collect_terminals(gr, terminals);
    collect_nonterminals(gr, nonterminals);

    using first_set_type = std::map<symbol_type, sets_type, symbol::symbol_key_comparator>;

    first_set_type fas;       // F(A), A ∈ N, the current FIRST set
    first_set_type fas_prime; // F'(A), A ∈ N, the FIRST set from the previous iteration

    // 1. for each a ∈ T do F'(a) = {a}
    // FIRSTk(a) = { { a } }
    for(const auto& terminal : terminals)
    {
        if(build_eff && (*terminal).id() == (*symbol::epsilon).id())
        {
            continue;
        }

        fas.emplace(first_set_type::value_type(terminal, sets_type { set_type { terminal } }));
    }

    // FIRSTk(λ) = { { λ } }
    if(!build_eff)
    {
        if((*symbol::epsilon).first_sets().empty())
        {
            fas.emplace(first_set_type::value_type(symbol::epsilon, sets_type { set_type { symbol::epsilon } }));
        }
    }
    // FIRSTk($) = { { $ } }
    if((*symbol::eof).first_sets().empty())
    {
        fas.emplace(first_set_type::value_type(symbol::eof, sets_type { set_type { symbol::eof } }));
    }

    // FIRSTk(#) = { { # } }
    if((*symbol::op_mark).first_sets().empty())
    {
        fas.emplace(first_set_type::value_type(symbol::op_mark, sets_type { set_type { symbol::op_mark } }));
    }

    // 2. for each A ∈ N do F(A) = {λ} if A is nullable or empty otherwise
    for(const auto& nonterminal : nonterminals)
    {
        //fas[nonterminal] = sets_type {};
        //fas_prime[nonterminal] = sets_type {};

        // prepopulate at this point according to AU: p.358, item 2) where:
        //  A -> xα and either |x| = k or |x| < k and a = ε (a = ε means production has form A -> x)
        auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
        const auto& nonterminal_rules((*nonterminal_it).second);

        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            if(build_eff && (*nonterminal_rule).empty())
            {
                continue;
            }

            set_type fs;

            for(std::size_t i = 0; i < std::min<std::size_t>(k, (*nonterminal_rule).rhs().size()); i++)
            {
                const auto& symb((*nonterminal_rule).rhs()[i]);

                if((*symb).terminal())
                {
                    fs.emplace_back(symb); // including λ, when A -> λ
                }
                else
                {
                    fs.clear(); // must be xxx...A, where |xxx...| < k
                    break;
                }
            }

            if(!fs.empty())
            {
                fas[nonterminal].emplace_back(fs);
            }
        }

        auto& fa(fas[nonterminal]);

        // resize
        std::for_each(fa.begin(), fa.end(), [&k](auto& set){ if(set.size() > k) set.resize(k); });

        // make unique
        make_vector_unique(fa);
    }

    // 3. repeat
    volatile bool changing = false;

    do
    {
        // 3.1 for each A ∈ N do F'(A) = F(A)
        for(const auto& pool_symbol_kvp : gr.pool())
        {
            const auto& pool_symbol(pool_symbol_kvp.second);

            const auto& fa(fas[pool_symbol]); // F(A)
            const auto fas_prime_it(fas_prime.find(pool_symbol)); // F'(A), copy

            if(fas_prime_it == fas_prime.end())
            {
                fas_prime.emplace(first_set_type::value_type(pool_symbol, fa));
            }
            else
            {
                (*fas_prime_it).second = fa;
            }
        }

        // 3.2 for each rule A -> u1 u2 ... uN with n > 0 do
        //      F(A) = F(A) ∪ TRUNKk(F'(u1) F'(u2) ... F'(uN))
        for(const auto& nonterminal : nonterminals)
        {
            auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
            const auto& nonterminal_rules((*nonterminal_it).second);

            for(const auto& nonterminal_rule : nonterminal_rules)
            {
                if(build_eff && (*nonterminal_rule).empty())
                {
                    continue;
                }

                // collect F'(u1) F'(u2) ... F'(uN)
                std::vector<sets_type> rhs_fus_prime;

                std::for_each((*nonterminal_rule).rhs().begin(),
                              (*nonterminal_rule).rhs().end(),
                              [&fas_prime, &rhs_fus_prime](const auto& symb)
                              {
                                  rhs_fus_prime.emplace_back(fas_prime[symb]);
                              });

                // instead of TRUNKk(F'(u1) F'(u2) ... F'(uN)) using infix-op { Fi-1(Y1) (+)k Fi-1(Y2) (+)k ... (+) Fi-1(Yn) } ∪ Fi-1(Y1)
                sets_type rhs_fus_prime_result;

                infix_operator(rhs_fus_prime, k, rhs_fus_prime_result);

                // F(A) ∪ TRUNKk(...)
                auto& fa(fas[nonterminal]); // F(A)

                std::for_each(rhs_fus_prime_result.begin(),
                              rhs_fus_prime_result.end(),
                              [&fa](const auto& rhs_fui_prime)
                              {
                                  fa.emplace_back(rhs_fui_prime);
                              });

                // make unique
                make_vector_unique(fa);
            }
        }

        // until F(A) = F'(A) for all A ∈ N
        changing = false;

        for(const auto& nonterminal : nonterminals)
        {
            // F(A)
            const auto& fa(fas[nonterminal]);

            //log_info(L"F(%s)  = %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_sets(fa).c_str());

            // F'(A)
            const auto& fa_prime(fas_prime[nonterminal]);

            //log_info(L"F'(%s) = %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_sets(fa_prime).c_str());

            if(fa != fa_prime)
            {
                changing = true;
                //break; // no break if need to keep going to print current sets
            }
        }
    }
    while(changing);

    // 4. FIRSTk(A) = F(A)
    for(const auto& fa_kvp : fas)
    {
        const auto& nonterminal(fa_kvp.first);

        auto fa(fa_kvp.second);

        if(build_eff)
        {
            std::for_each(fa.begin(), fa.end(), [&nonterminal](const auto& set){ (*nonterminal).eff_sets().emplace_back(set); });
        }
        else
        {
            std::for_each(fa.begin(), fa.end(), [&nonterminal](const auto& set){ (*nonterminal).first_sets().emplace_back(set); });
        }
    }

    // visualization
    if(build_eff)
    {
        log_info(L"EFF SETS:");
    }
    else
    {
        log_info(L"FIRST SETS:");
    }
    
    for(const auto& pool_symbol_kvp : gr.pool())
    {
        const auto& pool_symbol(pool_symbol_kvp.second);

        if(build_eff)
        {
            log_info(L"EFF(%s) = %s", (*pool_symbol).name().c_str(), grammar_visualization::decorate_sets((*pool_symbol).eff_sets()).c_str());
        }
        else
        {
            log_info(L"FIRST(%s) = %s", (*pool_symbol).name().c_str(), grammar_visualization::decorate_sets((*pool_symbol).first_sets()).c_str());
        }
    }

    if(build_eff)
    {
        log_info(L"Built EFF set for k = %d.", k);
    }
    else
    {
        log_info(L"Built FIRST set for k = %d.", k);
    }
}

void grammar_algorithm::build_first_set(const typename grammar_algorithm::symbols_type& symbols, // S -> A B C, where A B C is 'α'
                                        uint8_t k,
                                        typename grammar_algorithm::sets_type& result)
{
    //  FIRSTk(α) = FIRSTk(u1) (+)k FIRSTk(u2) (+)k ... (+)k FIRSTk(uN)
    log_info(L"Building first set of alpha for k = %d ...", k);

    std::vector<sets_type> first_sets;

    std::for_each(symbols.begin(),
                  symbols.end(),
                  [&first_sets](const auto& symb)
                  {
                      first_sets.emplace_back((*symb).first_sets());
                  });

    infix_operator(first_sets, k, result);

    // if all nullable add {λ}
    if(!symbols.empty() && std::all_of(symbols.begin(), symbols.end(), [](const auto& symb){ return (*symb).nullable(); }))
    {
        result.emplace_back(set_type { symbol::epsilon });
    }

    make_vector_unique(result); // against { { e } { e } ... }

    string_type alpha;

    std::for_each(symbols.begin(), symbols.end(), [&alpha](const auto& symb){ alpha += (*symb).name(); });

    log_info(L"FIRST(%s) = %s", alpha.c_str(), grammar_visualization::decorate_sets(result).c_str());

    log_info(L"Built first set of alpha for k = %d.", k);
}

void grammar_algorithm::build_eff_set(const grammar& gr,
                                      const typename grammar_algorithm::symbols_type& symbols, // S -> A B C, where A B C is 'α'
                                      uint8_t k,
                                      typename grammar_algorithm::sets_type& result)
{
    // EFFk(α) = {w | w ∈ FIRSTk(α) и существует вывод α => β => wx, где β != Awx for A ∈ N }
    log_info(L"Building first epsilon free, EFF, set of alpha for k = %d ...", k);

    if(!symbols.empty())
    {
        if((*symbols.front()).terminal())
        {
            // EFFk(α) = FIRSTk(α)
            if(k == 1)
            {
                build_first1_set(symbols, result);
            }
            else
            {
                build_first_set(symbols, k, result);
            }
        }
        else
        {
            // EFFk(Xα) = EFFk(X) (+)k FIRSTk(α)
            const auto& nonterminal((*(gr.pool().find((*symbols.front()).name()))).second);

            if(symbols.size() == 1)
            {
                result = (*nonterminal).eff_sets();
            }
            else
            {
                sets_type eff((*nonterminal).eff_sets());

                sets_type first;
                symbols_type symbols0;

                std::for_each(symbols.begin() + 1, symbols.end(), [&symbols0](const auto& symb){ symbols0.emplace_back(symb); });

                if(k == 1)
                {
                    build_first1_set(symbols0, first);
                }
                else
                {
                    build_first_set(symbols0, k, first);
                }

                std::vector<sets_type> infix_op_input;

                infix_op_input.emplace_back(eff);
                infix_op_input.emplace_back(first);

                infix_operator(infix_op_input, k, result);
            }
        }
    }

    string_type alpha;

    std::for_each(symbols.begin(), symbols.end(), [&alpha](const auto& symb){ alpha += (*symb).name(); });

    log_info(L"EFF(%s) = %s", alpha.c_str(), grammar_visualization::decorate_sets(result).c_str());

    log_info(L"Built first epsilon free, EFF, set of alpha for k = %d.", k);
}

void grammar_algorithm::build_follow_set(grammar& gr, uint8_t k)
{
    // Sudkump 2nd ed., p.501 and Ronald C. Backhouse, p.110
    //  1. FL(S) = {λ}
    //  2. for each A ∈ N-{S} do FL(A) = ∅
    //  3. repeat
    //      3.1 for each A ∈ N do FL'(A) = FL(A)
    //      3.2 for each rule A -> w = u1 u2 ... uN with w ∉ T* do  (w ∉ T* means at least one nonterminal must be in RHS, if not - skip)
    //          3.2.1 L = FL'(A)
    //          3.2.2 if uN ∈ N then FL(uN) = FL(uN) ∪ L
    //          3.2.3 for i = n-1 to 1 do
    //                  3.2.3.1 L = TRUNCk(FIRSTk(ui+1) L)
    //                  3.2.3.2 if ui ∈ N then FL(ui) = FL(ui) ∪ L
    //                end for
    //          end for
    //     until FL(A) = FL'(A) for every A ∈ N
    //  4. FOLLOWk(A) = FL(A)
    //
    // FOLLOW set is list of lists for every non-terminal in grammar, size of "lists" is up to K
    //    ( )
    //     | \
    //     |   --- 
    //     |      |
    // (0,1..K) (0,1..K)
    //
    // ATTENTION:
    //  this algorithm assumes FIRSTk() are not empty, for some infinite grammars algorithm does NOT work correctly!!!, one of them is
    //      S : E + E
    //      E : E * E
    // in this case, FIRSTk(S) and FIRSTk(E) are empty sets and the statement 3.2.3.1 L = TRUNCk(FIRSTk(ui+1) L) always calculates L as an empty set ∅.
    // For k = 1 consider to use alternative implementation build_first1_set and build_follow1_set.
    log_info(L"Building follow set for k = %d ..., Sudkamp", k);

    // preliminaries
    std::vector<symbol_type> nonterminals;

    collect_nonterminals(gr, nonterminals);

    std::sort(nonterminals.begin(),
              nonterminals.end(),
              [] (const symbol_type& symb1, const symbol_type& symb2)
              {
                  return (*symb1).id() < (*symb2).id();
              });

    using follow_set_type = std::map<symbol_type, sets_type, symbol::symbol_key_comparator>;

    follow_set_type flas;       // FL(A), A ∈ N
    follow_set_type flas_prime; // FL'(A), A ∈ N

    // 1. FL(S) = {λ} or {$}
    flas.emplace(follow_set_type::value_type(gr.start_symbol(), sets_type { set_type { symbol::epsilon } }));
    //flas.emplace(follow_set_type::value_type(gr.start_symbol(), sets_type { set_type { symbol::eof } }));

    // 2. for each A ∈ N-{S} do FL(A) = ∅
    //  follow sets are empty by default
    for(const auto& nonterminal : nonterminals)
    {
        if((*nonterminal).id() != (*gr.start_symbol()).id())
        {
            // empty set
        }
    }

    // 3. repeat
    volatile bool changing = false;

    do
    {
        // 3.1 for each A ∈ N do FL'(A) = FL(A)
        for(const auto& nonterminal : nonterminals)
        {
            const auto& fla(flas[nonterminal]); // FL(A)

            const auto flas_prime_it(flas_prime.find(nonterminal)); // F'(A)

            if(flas_prime_it == flas_prime.end())
            {
                // this happening at the begining as we do not explicitly initialize F'(A)
                flas_prime.emplace(follow_set_type::value_type(nonterminal, fla));
            }
            else
            {
                (*flas_prime_it).second = fla;
            }
        }

        // 3.2 for each rule A -> w = u1 u2 ... uN with w ∉ T* do  (w ∉ T* means at least one nonterminal must be in RHS, if not - skip)
        for(const auto& rule : gr.rules())
        {
            if((*rule).rhs_nonterminal_count() == 0) 
            {
                // w ∉ T* means at least one nonterminal must be in RHS, if not - skip
                continue;
            }

            // 3.2.1 L = FL'(A)
            const auto& lhs_nonterminal((*rule).lhs().front()); // A
            const auto& fla_prime(flas_prime[lhs_nonterminal]); // FL'(A)

            auto fl_prime_a(fla_prime); // L, copy of FL'(A)

            // 3.2.2 if uN ∈ N then FL(uN) = FL(uN) ∪ L
            const auto& un_nonterminal((*rule).rhs().back()); // uN

            if((*un_nonterminal).nonterminal())
            {
                auto& fl_un(flas[un_nonterminal]); // FL(uN)

                // FL(uN) = FL(uN) ∪ L
                std::for_each(fl_prime_a.begin(),
                              fl_prime_a.end(),
                              [&fl_un](const auto& set){ fl_un.emplace_back(set); });

                make_vector_unique(fl_un);
            }

            // 3.2.3 for i = n-1 to 1 do
            for(int i = static_cast<int>((*rule).rhs().size()) - 1 - 1; i >= 0; i--) // -1 as we start from 0 and condition was 'n-1'
            {
                // 3.2.3.1 L = TRUNCk(FIRSTk(ui+1) L)
                const auto& ui1_nonterminal((*rule).rhs()[i + 1]); // ui+1
                auto first_ui1((*ui1_nonterminal).first_sets()); // FIRSTk(ui+1)

                sets_type new_fl_prime_a; // holds TRUNCk(FIRSTk(ui+1) L)

                std::vector<sets_type> infix_op_input;

                infix_op_input.emplace_back(first_ui1);
                infix_op_input.emplace_back(fl_prime_a);

                infix_operator(infix_op_input, k, new_fl_prime_a); // TRUNCk(FIRSTk(ui+1) L)

                fl_prime_a = new_fl_prime_a; // L = TRUNCk(FIRSTk(ui+1) L)

                // 3.2.3.2 if ui ∈ N then FL(ui) = FL(ui) ∪ L
                const auto& ui_nonterminal((*rule).rhs()[i]); // ui

                if((*ui_nonterminal).nonterminal())
                {
                    auto& fl_ui(flas[ui_nonterminal]); // FL(ui)

                    std::for_each(new_fl_prime_a.begin(),
                                  new_fl_prime_a.end(),
                                  [&fl_ui](const auto& set){ fl_ui.emplace_back(set); });

                    make_vector_unique(fl_ui);
                }
            }
        }

        // until FL(A) = FL'(A) for every A ∈ N
        changing = false;

        for(const auto& nonterminal : nonterminals)
        {
            // FL(A)
            const auto& fla(flas[nonterminal]);

            //log_info(L"F(%s)  = %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_sets(fla).c_str());

            // FL'(A)
            const auto& fla_prime(flas_prime[nonterminal]);

            //log_info(L"F'(%s) = %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_sets(fla_prime).c_str());

            if(fla != fla_prime)
            {
                changing = true;
                break; // no break if need to keep going to print current sets
            }
        }
    }
    while(changing);

    // 4. FOLLOWk(A) = FL(A)
    for(const auto& fla_kvp : flas)
    {
        const auto& nonterminal(fla_kvp.first);
        const auto& fla(fla_kvp.second);

        std::for_each(fla.begin(),
                      fla.end(),
                      [&nonterminal](const auto& set){ (*nonterminal).follow_sets().emplace_back(set); });
    }

    // visualize
    log_info(L"FOLLOW SETS:");
    
    for(const auto& nonterminal : nonterminals)
    {
        log_info(L"FOLLOW(%s) = %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_sets((*nonterminal).follow_sets()).c_str());
    }

    log_info(L"Built follow set for k = %d.", k);
}

void grammar_algorithm::build_first1_set(grammar& gr)
{
    // The idea is based on this observation, from
    //  "Reinhard Wilhelm Helmut Seidl Sebastian Hack 'Compiler Design', Syntactic and Semantic Analysis", p.72
    //               L1                if L2 != ∅ and ε ∉ L1
    //  L1 (+)1 L2 = 
    //               (L2\{ε}) ∪ L2    if L2 != ∅ and ε ∈ L1
    //
    // http://marvin.cs.uidaho.edu/Teaching/CS445/firstfollow.txt
    //  computeFirst({P_1, P_2, ...P_m})
    //      foreach A ∈ TERMS do First[A] = {A}
    //      foreach A ∈ NONTERMS do First[A] = {}
    //
    //      loop until nothing new happens updating the First sets
    //      while stillchanging any First[A] do
    //          foreach production P_i = A::=X_1, X_2, ... X_n do
    //              First[A] <- First[A] union computeFirstOfList(X_1, X_2, ... X_n)
    //          end foreach
    //      end while
    log_info(L"Building first set for k = 1 ...");

    // collect terminals and nonterminals
    std::vector<symbol_type> terminals;
    std::vector<symbol_type> nonterminals;

    collect_terminals(gr, terminals);
    collect_nonterminals(gr, nonterminals);

    // foreach A ∈ TERMS do First[A] = {A}
    for(const auto& terminal : terminals)
    {
        (*terminal).first_sets().emplace_back(set_type { terminal });
    }

    // FIRSTk(λ) = { { λ } }
    if((*symbol::epsilon).first_sets().empty())
    {
        (*symbol::epsilon).first_sets().emplace_back(set_type { symbol::epsilon });
    }

    // FIRSTk($) = { { $ } }
    if((*symbol::eof).first_sets().empty())
    {
        (*symbol::eof).first_sets().emplace_back(set_type { symbol::eof });
    }

    // FIRSTk(#) = { { # } }
    if((*symbol::op_mark).first_sets().empty())
    {
        (*symbol::op_mark).first_sets().emplace_back(set_type { symbol::op_mark });
    }

    // foreach A ∈ NONTERMS do First[A] = {}
    //  do nothing

    // calculate
    volatile bool changing = false;

    // loop until nothing new happens updating the First sets
    do
    {
        changing = false; // reset repeat-guard

        // for each rule A -> X1 X2 ... Xn in grammar G loop
        for(const auto& rule : gr.rules())
        {
            // First[A] <- First[A] union computeFirstOfList(X_1, X_2, ... X_n)
            const auto& lhs_nonterminal((*rule).lhs()[0]);

            std::size_t first_sets_count = (*lhs_nonterminal).first_sets().size();

            // ... computeFirstOfList(X_1, X_2, ... X_n)
            symbols_type rhs_symbols;

            std::for_each((*rule).rhs().begin(), (*rule).rhs().end(), [&rhs_symbols](const auto symb){ rhs_symbols.emplace_back(symb); });

            sets_type rhs_first_set;

            build_first1_set(rhs_symbols, rhs_first_set);

            // ... union ...
            std::for_each(rhs_first_set.begin(),
                          rhs_first_set.end(),
                          [&lhs_nonterminal](const auto& set){ (*lhs_nonterminal).first_sets().emplace_back(set); });

            // make unique
            make_vector_unique((*lhs_nonterminal).first_sets());

            if(!changing)
            {
                changing = first_sets_count != (*lhs_nonterminal).first_sets().size();
            }
        }
    }
    while(changing); // while stillchanging any First[A] do

    // visualize
    log_info(L"FIRST SETS:");
    
    for(const auto& pool_symbol_kvp : gr.pool())
    {
        const auto& pool_symbol(pool_symbol_kvp.second);

        log_info(L"FIRST(%s) = %s", (*pool_symbol).name().c_str(), grammar_visualization::decorate_sets((*pool_symbol).first_sets()).c_str());
    }

    log_info(L"Built first set for k = 1.");
}

void grammar_algorithm::build_first1_set(const typename grammar_algorithm::symbols_type& symbols, typename grammar_algorithm::sets_type& result)
{
    // http://marvin.cs.uidaho.edu/Teaching/CS445/firstfollow.txt
    //  computeFirstOfList(X_1, X_2, ... X_n)
    //      Tmp = {}
    //      k=0
    //      do
    //          k++
    //          Tmp <- Tmp union First[X_k] - {ε}
    //      while k<n & ε isin First[X_k]
    //
    //      ε only if X_1, X_2, ... X_n -> ε
    //      Note: this test can only possibly work if k==n:
    //      if ε isin First[X_k] then
    //          Tmp <- Tmp union {ε}
    //      return Tmp
    log_info(L"Building first set of alpha for k = 1 ...");

    if(!symbols.empty())
    {
        const set_type epsilon_set { symbol::epsilon };

        sets_type tmp_result;

        std::size_t k = 0;
        std::size_t n = symbols.size();
        std::size_t e = 0;

        volatile bool has_epsilon = false;

        do
        {
            const auto& symbol(symbols[k]);

            auto symbol_first_set((*symbol).first_sets()); // copy

            // Tmp <- Tmp union First[X_k] - {ε}
            has_epsilon = std::find(symbol_first_set.begin(), symbol_first_set.end(), epsilon_set) != symbol_first_set.end();

            if(has_epsilon)
            {
                symbol_first_set.erase(std::remove_if(symbol_first_set.begin(),
                                                      symbol_first_set.end(),
                                                      [&epsilon_set](const auto& set)
                                                      {
                                                          return set == epsilon_set;
                                                      }),
                                                      symbol_first_set.end());
                e++;
            }

            std::for_each(symbol_first_set.begin(),
                          symbol_first_set.end(),
                          [&tmp_result](const auto& set){ tmp_result.emplace_back(set); });

            // make unique
            make_vector_unique(tmp_result);

            k++;
        }
        while(k < n && has_epsilon);

        // ... this test can only possibly work if k==n
        // if ε isin First[X_k] then
        if(e == n)
        {
            // Tmp <- Tmp union {ε}
            tmp_result.emplace_back(epsilon_set);
        }

        result.swap(tmp_result);
    }

    // visualize
    string_type alpha;

    std::for_each(symbols.begin(), symbols.end(), [&alpha](const auto& symb){ alpha += (*symb).name(); });

    log_info(L"FIRST(%s) = %s", alpha.c_str(), grammar_visualization::decorate_sets(result).c_str());

    log_info(L"Built first set of alpha for k = 1.");
}

void grammar_algorithm::build_follow1_set(grammar& gr)
{
    // http://marvin.cs.uidaho.edu/Teaching/CS445/firstfollow.txt
    //  foreach A ∈ NONTERMS do Follow[A] = {}
    //  Follow[S]={$}, where S is the start symbol of G
    //  loop until nothing new happens updating the Follow sets
    //  while stillchanging any Follow[A] do
    //    foreach P_i do / for each rule A -> X1 X2 ... Xn in grammar G loop
    //        foreach X_i do (over elements in right hand side)
    //            if X_i ∈ NONTERMS then
    //                 Follow[X_i] <- Follow[X_i] union computeFirstOfList(X_i+1 X_i+2 ...) - { ε }
    //                 if ε ∈ computeFirstOfList(X_i+1,X_i+2...) OR computeFirstOfList(X_i+1,X_i+2...) is EMPTY (my comment) then
    //                     Follow[X_i] <- Follow[X_i] union Follow[A]
    //                 end if
    //            end if
    //        end foreach
    //    end foreach
    //  end while
    log_info(L"Building follow set for k = 1 ...");

    const set_type epsilon_set { symbol::epsilon };

    //  foreach A ∈ NONTERMS do Follow[A] = {}
    //      do nothing

    // Follow(S) := {eof}, where S is the start symbol of G
    (*gr.start_symbol()).follow_sets().emplace_back(set_type { symbol::epsilon });

    // calculate
    volatile bool changing = false;

    do
    {
        changing = false; // reset repeat-guard

        // foreach P_i do / for each rule A -> X1 X2 ... Xn in grammar G loop
        for(const auto& rule : gr.rules())
        {
            if((*rule).rhs_nonterminal_count() == 0) // ... at least one nonterminal must be in RHS, if not - skip
            {
                continue;
            }

            // foreach X_i do (over elements in right hand side)
            for(std::size_t i = 0, n = (*rule).rhs().size(); i < n; i++)
            {
                const auto& rhs_nonterminal((*rule).rhs()[i]);

                if((*rhs_nonterminal).terminal()) // skip terminals
                {
                    continue;
                }

                std::size_t follow_sets_count = (*rhs_nonterminal).follow_sets().size();

                // Follow[X_i] <- Follow[X_i] union computeFirstOfList(X_i+1 X_i+2 ...) - { ε }

                // ... computeFirstOfList(X_i+1 X_i+2 ...)
                symbols_type tail_symbols;

                std::for_each((*rule).rhs().begin() + i + 1, // +1 as X_i+1 X_i+2 ...
                              (*rule).rhs().end(),
                              [&tail_symbols](const auto& symb) { tail_symbols.emplace_back(symb); });

                sets_type tail_first_set;

                build_first1_set(tail_symbols, tail_first_set);

                // ...  - { ε }
                bool has_epsilon = std::find(tail_first_set.begin(), tail_first_set.end(), epsilon_set) != tail_first_set.end();

                if(has_epsilon)
                {
                    tail_first_set.erase(std::remove_if(tail_first_set.begin(),
                                                        tail_first_set.end(),
                                                        [&epsilon_set](const auto& set)
                                                        {
                                                            return set == epsilon_set;
                                                        }),
                                                        tail_first_set.end());
                }

                // ...  union ...
                std::for_each(tail_first_set.begin(),
                              tail_first_set.end(),
                              [&rhs_nonterminal](const auto& set)
                              {
                                  (*rhs_nonterminal).follow_sets().emplace_back(set);
                              });

                // if ε ∈ computeFirstOfList(X_i+1,X_i+2...) OR computeFirstOfList(X_i+1,X_i+2...) is EMPTY (my comment) then
                if(has_epsilon || tail_first_set.empty())
                {
                    // Follow[X_i] <- Follow[X_i] union Follow[A]
                    const auto& lhs_nonterminal((*rule).lhs()[0]);

                    if(lhs_nonterminal != rhs_nonterminal) //  check against C -> a b C
                    {
                        std::for_each((*lhs_nonterminal).follow_sets().begin(), // A
                                      (*lhs_nonterminal).follow_sets().end(),
                                      [&rhs_nonterminal](const auto& set)
                                      {
                                          (*rhs_nonterminal).follow_sets().emplace_back(set);
                                      });
                    }
                }

                // make unique
                make_vector_unique((*rhs_nonterminal).follow_sets());

                if(!changing)
                {
                    changing = follow_sets_count != (*rhs_nonterminal).follow_sets().size();
                }
            }
        }
    }
    while(changing);

    // visualize
    log_info(L"FOLLOW SETS:");
    
    std::vector<symbol_type> nonterminals;

    collect_nonterminals(gr, nonterminals);

    std::sort(nonterminals.begin(),
              nonterminals.end(),
              [] (const symbol_type& symb1, const symbol_type& symb2)
              {
                  return (*symb1).id() < (*symb2).id();
              });

    for(const auto& nonterminal : nonterminals)
    {
        log_info(L"FOLLOW(%s) = %s", (*nonterminal).name().c_str(), grammar_visualization::decorate_sets((*nonterminal).follow_sets()).c_str());
    }

    log_info(L"Built follow set for k = 1.");
}

void grammar_algorithm::build_la_set(grammar& gr, uint8_t k)
{
    // Sudkump 2nd ed., p.495
    // LAk(A) = TRUNCk(FIRSTk(A) ∪ FOLLOWk(A))
    log_info(L"Building lookahead set for nonterminals with k = %d ...", k);

    std::vector<symbol_type> nonterminals;

    collect_nonterminals(gr, nonterminals);

    for(auto& nonterminal : nonterminals)
    {
        std::vector<sets_type> la_sets;

        la_sets.emplace_back((*nonterminal).first_sets()); // push FIRSTk(A)
        la_sets.emplace_back((*nonterminal).follow_sets()); // push FOLLOWk(A)

        sets_type la_sets_result;

        infix_operator(la_sets, k, la_sets_result);

        // result
        std::for_each(la_sets_result.begin(),
                      la_sets_result.end(),
                      [&nonterminal](const auto& set){ (*nonterminal).la_sets().emplace_back(set); });
    }

    log_info(L"LOOKAHEAD SETS:");
    
    for(const auto& pool_symbol_kvp : gr.pool())
    {
        const auto& pool_symbol(pool_symbol_kvp.second);

        log_info(L"LOOKAHEAD(%s) = %s", (*pool_symbol).name().c_str(), grammar_visualization::decorate_sets((*pool_symbol).la_sets()).c_str());
    }

    log_info(L"Built lookahead set for nonterminals with k = %d.", k);
}

void grammar_algorithm::build_la_set(const typename grammar_algorithm::rule_type& rule,
                                     uint8_t k,
                                     typename grammar_algorithm::sets_type& result)
{
    // Sudkump 2nd ed., p.495
    // LAk(A -> w) = TRUNCk(FIRSTk(w) ∪ FOLLOWk(A))
    log_info(L"Building strong-LL(k) lookahead set for %s with k = %d ...", grammar_visualization::decorate_rule(rule).c_str(), k);

    sets_type alpha_first_sets;
    
    build_first_set((*rule).rhs(), k, alpha_first_sets); // FIRSTk(w)

    std::vector<sets_type> la_sets;

    if(!alpha_first_sets.empty())
    {
        la_sets.emplace_back(alpha_first_sets); // push FIRSTk(w)
    }

    la_sets.emplace_back((*(*rule).lhs().front()).follow_sets()); // push FOLLOWk(A)

    if(alpha_first_sets.empty()) // A -> λ
    {
        std::for_each(la_sets.begin(),
                      la_sets.end(),
                      [&result](const auto& sets)
                      { 
                          std::for_each(sets.begin(), sets.end(), [&result](const auto& set) { result.emplace_back(set); });
                      });
    }
    else
    {
        sets_type la_sets_result;

        infix_operator(la_sets, k, la_sets_result);

        std::for_each(la_sets_result.begin(),
                      la_sets_result.end(),
                      [&result](const auto& set){ result.emplace_back(set); });
    }

    log_info(L"LOOKAHEAD(%s) = %s", grammar_visualization::decorate_rule(rule).c_str(), grammar_visualization::decorate_sets(result).c_str());

    log_info(L"Built strong-LL(k) lookahead set for %s with k = %d.", grammar_visualization::decorate_rule(rule).c_str(), k);
}

void grammar_algorithm::build_la_set(const typename grammar_algorithm::symbols_type& symbols,
                                     int32_t nonterminal_index,
                                     uint8_t k,
                                     typename grammar_algorithm::sets_type& result)
{
    // Sudkump 2nd ed., p.495
    // The lookahead set of the sentential form uAv is defined by LAk(uAv) = FlRSTk(Av).
    log_info(L"Building LL(k) lookahead set for k = %d.", k);

    symbols_type av_symbols; // Av

    for(std::size_t i = nonterminal_index, n = symbols.size(); i < n; i++)
    {
        av_symbols.emplace_back(symbols[i]);
    }

    build_first_set(av_symbols, k, result); // FIRSTk(Av)

    string_type symbols_decorated;

    for(const auto& symbol : symbols)
    {
        symbols_decorated += (*symbol).name() + L" ";
    }

    log_info(L"LOOKAHEAD(%s) = %s", symbols_decorated.c_str(), grammar_visualization::decorate_sets(result).c_str());

    log_info(L"Built LL(k) lookahead set for k = %d.", k);
}

void grammar_algorithm::build_la_set(const typename grammar_algorithm::symbols_type& symbols,
                                     const typename grammar_algorithm::rule_type& rule,
                                     int32_t nonterminal_index, // index of A in uAv
                                     uint8_t k,
                                     typename grammar_algorithm::sets_type& result)
{
    // Sudkump 2nd ed., p.507
    // The lookahead set for the sentential form uAv and rule A -> w is defined by LAk(uAv, A -> w) = FIRSTk(wv).
    log_info(L"Building LL(k) lookahead set for k = %d.", k);

    symbols_type wu_symbols((*rule).rhs()); // wu, copy w part

    for(std::size_t i = nonterminal_index + 1, n = symbols.size(); i < n; i++) // add v part
    {
        wu_symbols.emplace_back(symbols[i]);
    }

    build_first_set(wu_symbols, k, result); // FIRSTk(Av)

    string_type symbols_decorated;

    for(const auto& symbol : symbols)
    {
        symbols_decorated += (*symbol).name() + L" ";
    }

    log_info(L"LOOKAHEAD(%s, %s) = %s",
             symbols_decorated.c_str(),
             grammar_visualization::decorate_rule(rule).c_str(),
             grammar_visualization::decorate_sets(result).c_str());

    log_info(L"Built LL(k) lookahead set for k = %d.", k);
}

void grammar_algorithm::remove_immediate_left_recursion(grammar& gr,
                                                        const typename grammar_algorithm::symbol_type& nonterminal,
                                                        std::size_t& suffix_number,
                                                        std::size_t& symbols_count,
                                                        std::size_t& rules_count)
{
    //  A -> Aα1 | ... | AαN | β1 | ... | βN
    //  =>
    //  A -> β1 A' |... | βN A'
    //  A'-> α1 A'|... | αN A' | e
    //    where A' is a new nonterminal not used elsewhere in the grammar
    log_info(L"Removing immediate left recursion ...");

    auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
    
    if(nonterminal_it != gr.nt_rules().end())
    {
        auto nonterminal_rules((*nonterminal_it).second); // take snapshot, copy

        bool left_recursive = false;

        symbol_type new_nonterminal;

        // check if nonterminal is left-recursive
        for(const auto& nonterminal_rule : nonterminal_rules)
        {
            if((*(*nonterminal_rule).lhs().front()).id() == (*(*nonterminal_rule).rhs().front()).id()) // A -> A ...
            {
                auto new_symbol_name((*nonterminal).name() + std::to_wstring(suffix_number++));

                new_nonterminal = (factory::create<symbol>(static_cast<uint32_t>(symbols_count++), new_symbol_name, symbol::kind::nonterminal));

                gr.pool().emplace(pool_type::value_type(new_symbol_name, new_nonterminal)); // add to symbols pool

                left_recursive = true;
                break;
            }
        }

        if(left_recursive)
        {
            // remove immediate left recursion
            for(auto it = nonterminal_rules.begin(); it != nonterminal_rules.end();)
            {
                const auto& nonterminal_rule(*it);

                if((*(*nonterminal_rule).lhs().front()).id() == (*(*nonterminal_rule).rhs().front()).id()) // A -> A b c ...
                {
                    // alpha
                    rule_type production(factory::create<rule>(static_cast<uint32_t>(rules_count++), (*new_nonterminal).name()));

                    (*production).add_lhs_symbol(new_nonterminal);

                    std::for_each((*nonterminal_rule).rhs().begin() + 1, // skipping A in A -> A b c ...
                                  (*nonterminal_rule).rhs().end(),
                                  [&production](const auto& symb){ (*production).add_rhs_symbol(symb); });

                    (*production).add_rhs_symbol(new_nonterminal);

                    gr.rules().emplace_back(production);
                    add_nonterminal_rule(gr, new_nonterminal, production); // nonterminal-rules mapping ...

                    // remove A -> A ...
                    gr.rules().erase(std::find_if(gr.rules().begin(),
                                                  gr.rules().end(),
                                                  [&nonterminal_rule](const auto& rule){ return (*rule).id() == (*nonterminal_rule).id(); }));
                    remove_nonterminal_rule(gr, nonterminal, nonterminal_rule);

                    it = nonterminal_rules.erase(it);
                }
                else
                {
                    // beta, just add A'
                    (*nonterminal_rule).add_rhs_symbol(new_nonterminal);
                    ++it;
                }
            }

            // A' -> λ
            rule_type production(factory::create<rule>(static_cast<uint32_t>(rules_count++), (*new_nonterminal).name()));

            (*production).add_lhs_symbol(new_nonterminal);
            (*production).add_rhs_symbol(symbol::epsilon);

            gr.rules().emplace_back(production);
            add_nonterminal_rule(gr, new_nonterminal, production); // nonterminal-rules mapping ...
        }
    }

    log_info(L"Removed immediate left recursion.");
}

void grammar_algorithm::remove_left_recursion(grammar& gr)
{
    // Introduction.To.Compiler.Construction_Parsons_0716782618, p.87
    // Paul's algorithm:
    //  assign an ordering A1,..., An, to the nonterminals of the grammar
    //  for i = 1 to n do begin
    //    for j = 1 to i-1 do begin
    //       for each production of the form Ai -> Aj α do begin
    //          remove Ai -> Aj α from the grammar
    //          for each production of the form Aj -> β do begin
    //              add Ai -> β α to the grammar
    //          end
    //       end
    //    end
    //    transform the Ai-productions to eliminate direct left recursion
    //  end
    log_info(L"Removing indirect left recursion ...");

    std::size_t suffix_number = 1;
    std::size_t symbols_count = 5000000;
    std::size_t rules_count = 9000000;

    // assign an ordering A1,..., An, to the nonterminals of the grammar
    std::vector<symbol_type> nonterminals;

    collect_nonterminals(gr, nonterminals);

    // remove indirect left recursion
    // for i = 1 to n do begin
    for(std::size_t i = 0; i < nonterminals.size(); i++)
    {
        const auto& ai_nonterminal = nonterminals[i];

        // for j = 1 to i-1 do begin
        for(std::size_t j = 0; j < i; j++)
        {
            const auto& aj_nonterminal = nonterminals[j];

            // for each production of the form Ai -> Aj α do begin
            std::vector<rule_type> new_productions;

            for(auto it = gr.rules().begin(); it != gr.rules().end();)
            {
                const auto gr_rule(*it); // take snapshot, copy

                // Ai == Aj
                if((*(*gr_rule).lhs().front()).id() == (*ai_nonterminal).id() && (*(*gr_rule).rhs().front()).id() == (*aj_nonterminal).id())
                {
                    // remove Ai -> Aj α from the grammar
                    it = gr.rules().erase(it);
                    remove_nonterminal_rule(gr, ai_nonterminal, gr_rule);

                    // for each production of the form Aj -> β do begin
                    auto aj_nt_it(gr.nt_rules().find((*aj_nonterminal).name()));

                    if(aj_nt_it != gr.nt_rules().end())
                    {
                        auto& aj_nt_rules((*aj_nt_it).second);

                        for(auto& aj_production : aj_nt_rules)
                        {
                            // add Ai -> β α to the grammar
                            rule_type ai_production(factory::create<rule>(static_cast<uint32_t>(rules_count++), (*ai_nonterminal).name()));

                            (*ai_production).add_lhs_symbol(ai_nonterminal);

                            // adding β from Aj -> β
                            std::for_each((*aj_production).rhs().begin(), (*aj_production).rhs().end(), [&ai_production](const auto& symb){ (*ai_production).add_rhs_symbol(symb); });

                            // adding α from Ai -> α
                            //                                     + 1 to skip Aj in Ai -> Aj α
                            std::for_each((*gr_rule).rhs().begin() + 1, (*gr_rule).rhs().end(), [&ai_production](const auto& symb){ (*ai_production).add_rhs_symbol(symb); });

                            new_productions.emplace_back(ai_production);
                        }
                    }
                }
                else
                {
                    ++it;
                }
            }

            for(const auto& ai_production : new_productions)
            {
                gr.rules().emplace_back(ai_production);
                add_nonterminal_rule(gr, ai_nonterminal, ai_production);
            }
        }

        // transform the Ai-productions to eliminate direct left recursion
        remove_immediate_left_recursion(gr, nonterminals[i], suffix_number, symbols_count, rules_count);
    }

    //remove_duplicate_productions(gr);
    renumber_grammar(gr);

    log_info(L"Removed indirect left recursion.");
}

void grammar_algorithm::apply_left_factoring(grammar& gr)
{
    // https://github.com/petee-d/cfge (also in soft/CFG folder)
    // https://cyberzhg.github.io/toolbox/left_fact
    // S -> a A | a b A | a b c A | a b c | k b c A | g c c A | 1 2 3 4 5 A | ϵ
    //
    //  S -> a S'''
    //       | k b c A
    //       | g c c A
    //       | 1 2 3 4 5 A
    //       | ϵ
    //   S' -> A
    //       | ϵ
    //  S'' -> c S'
    //       | A
    // S''' -> b S''
    //       | A
    //
    // for each non-terminal A find the longest prefix α common to two or more of its alternatives.
    // if α is not empty, i.e there is a nontrivial common prefix, replace all the A productions:
    //      A -> αβ1 | ... | αβN | γ
    //        with
    //      A -> α A' | γ
    //      A' -> β1 | ... | βN
    // where γ represents all alternatives that do not begin with α.
    // here A' is a new nonterminal. repeatedly apply this transformation until no two alternatives for a non-terminal have a common prefix.
    //
    // algorithm to find common prefx of rules:
    //  a) sort out rules in descending order
    //  b) compare the RHS of the top rule with all beneath rules for a particular prefix length, reduce prefix length by one and repeat
    log_info(L"Applying left factoring ...");

    std::size_t suffix_number = 1;
    std::size_t symbols_count = 5000000;
    std::size_t rules_count = 9000000;

    rules_type factored_rules;
    symbols_type factored_prefix;

    auto nt_rules(gr.nt_rules()); // take a snapshot, copy

    for(const auto& nonterminal_kvp : nt_rules) // loop over nonterminalst
    {
        const auto& nonterminal((*(gr.pool().find(nonterminal_kvp.first))).second);

        auto nonterminal_it(gr.nt_rules().find((*nonterminal).name()));
        auto& nonterminal_rules((*nonterminal_it).second); // take non-const address, as code below might modify nonterminal rules

        std::sort(nonterminal_rules.begin(),
                  nonterminal_rules.end(),
                  [] (const rule_type& rule1, const rule_type& rule2){ return ((*rule1).rhs().size() > (*rule2).rhs().size()); }); // descendant

        std::size_t prefix_size = (*nonterminal_rules.front()).rhs().size(); // prefix size is the logest RHS of the top rule

        while(prefix_size > 0)
        {
            calculate_longest_common_prefixes(nonterminal_rules, prefix_size--, factored_rules, factored_prefix);

            if(!factored_rules.empty())
            {
                factor_rules(gr, nonterminal, factored_rules, factored_prefix, suffix_number, symbols_count, rules_count);
                prefix_size++; // one more round
            }
        }
    }

    renumber_grammar(gr);

    log_info(L"Applied left factoring.");
}

void grammar_algorithm::create_production_combinations(const symbols_type& rhs, const symbol_type& nonterminal, std::vector<symbols_type>& new_productions)
{
    //# https://eli.thegreenplace.net/2010/02/08/removing-epsilon-productions-from-context-free-grammars
    //# https://www.geeksforgeeks.org/power-set/
    //# Input: Set[], set_size
    //# 1. Get the size of power set
    //#     powet_set_size = pow(2, set_size)
    //# 2  Loop for counter from 0 to pow_set_size
    //#      (a) Loop for i = 0 to set_size
    //#           (i) If ith bit in counter is set
    //#                Print ith element from set for this subset
    //#      (b) Print separator for subsets i.e., newline
    //#
    //# Set  = [a,b,c]
    //# power_set_size = pow(2, 3) = 8
    //# Run for binary counter = 000 to 111
    //#
    //# Value of Counter            Subset
    //#     000                    -> Empty set
    //#     001                    -> a
    //#     010                    -> b
    //#     011                    -> ab
    //#     100                    -> c
    //#     101                    -> ac
    //#     110                    -> bc
    //#     111                    -> abc
    //def create_production_combinations(production, nonterminal, nonterminal_entries_count):
    //    power_set_size = 1 << nonterminal_entries_count
    //
    //    result = []  # all constructed subsets
    //
    //    for k in range(power_set_size):  # k is counter
    //        print("Value of Counter = " + "{0:b}".format(k).zfill(3))
    //        nth_nonterminal = 0  # index of currently processing nonterminal, aka i in 2a loop
    //        new_production = []
    //
    //        for symbol in production:
    //            if symbol == nonterminal:
    //                if k & (1 << nth_nonterminal):  # check if nth bit in the counter is set
    //                    new_production.append(symbol)
    //                nth_nonterminal += 1
    //            else:
    //                new_production.append(symbol)  # noisy symbols, not considered as powerset members
    //
    //        result.append(new_production)
    //
    //    return result
    //
    //
    //result = create_production_combinations(['A', 'k', 'a', 'B', 'c', 'A'], 'A', 2)
    uint32_t nonterminal_entries_count = 0;

    std::for_each(rhs.begin(),
                  rhs.end(),
                  [&nonterminal, &nonterminal_entries_count](const auto& symbol){ if(symbol == nonterminal) nonterminal_entries_count++; });

    if(nonterminal_entries_count > 0)
    {
        uint32_t power_set_size = 1 << nonterminal_entries_count;

        for(uint32_t k = 0; k < power_set_size; k++)
        {
            uint32_t nth_nonterminal = 0;

            symbols_type new_rhs;
    
            for(const auto& symbol : rhs)
            {
                if((*symbol).id() == (*nonterminal).id())
                {
                    if(k & (1 << nth_nonterminal))
                    {
                        new_rhs.emplace_back(symbol);
                    }

                    nth_nonterminal++;
                }
                else
                {
                    new_rhs.emplace_back(symbol);
                }
            }

            if(!new_rhs.empty())
            {
                new_productions.emplace_back(new_rhs);
            }
        }
    }
}

void grammar_algorithm::renumber_grammar(grammar& gr)
{
    std::sort(gr.rules().begin(),
              gr.rules().end(),
              [] (const rule_type& rule1, const rule_type& rule2)
              {
                  return (*(*rule1).lhs().front()).id() < (*(*rule2).lhs().front()).id();
              });

    uint32_t k = 0;

    std::for_each(gr.rules().begin(), gr.rules().end(), [&k](const auto& gr_rule){ (*gr_rule).id() = k++; });
}

void grammar_algorithm::calculate_longest_common_prefixes(typename grammar_algorithm::rules_type& rules,
                                                          std::size_t prefix_size,
                                                          typename grammar_algorithm::rules_type& factored_rules,
                                                          typename grammar_algorithm::symbols_type& factored_prefix)
{
    factored_rules.clear();
    factored_prefix.clear();

    rules_type rules_to_factor(rules); // take a snapshot, copy, as this will be modified

    while(rules_to_factor.size() > 1) // must be at least 2
    {
        std::sort(rules_to_factor.begin(),
                  rules_to_factor.end(),
                  [] (const rule_type& rule1, const rule_type& rule2){ return ((*rule1).rhs().size() > (*rule2).rhs().size()); }); // descendant

        const auto current_rule(rules_to_factor.front()); // the top rule kak abrazec

        rules_to_factor.erase(rules_to_factor.begin()); // remove the top rule

        if((*current_rule).rhs().size() < prefix_size)
        {
            continue;
        }

        rules_type current_factored_rules;

        symbols_type prefix((*current_rule).rhs().begin(), (*current_rule).rhs().begin() + prefix_size); // calculate prefix

        for(auto it = rules_to_factor.begin(); it != rules_to_factor.end();)
        {
            bool erase = false;

            const auto& iteration_rule(*it);

            if((*iteration_rule).rhs().size() >= prefix_size)
            {
                auto mismatch_result(std::mismatch((*iteration_rule).rhs().begin(),
                                                    (*iteration_rule).rhs().begin() + prefix_size,
                                                    prefix.begin(),
                                                    [] (const symbol_type& symb1, const symbol_type& symb2){ return (*symb1).id() == (*symb2).id(); }));

                if(mismatch_result.first == (*iteration_rule).rhs().begin() + prefix_size) // no mismatch
                {
                    current_factored_rules.emplace_back(iteration_rule);
                    erase = true;
                }
            }

            erase ? it = rules_to_factor.erase(it) : ++it;
        }

        if(!current_factored_rules.empty())
        {
            current_factored_rules.emplace_back(current_rule); // also add abrazec

            factored_rules.swap(current_factored_rules);
            factored_prefix.swap(prefix);
        }
    }
}

void grammar_algorithm::factor_rules(grammar& gr,
                                     const typename grammar_algorithm::symbol_type& nonterminal,
                                     const typename grammar_algorithm::rules_type& factored_rules,
                                     const typename grammar_algorithm::symbols_type& factored_prefix,
                                     std::size_t& suffix_number,
                                     std::size_t& symbols_count,
                                     std::size_t& rules_count)
{
    // A'
    auto new_factored_symbol_name((*nonterminal).name() + std::to_wstring(suffix_number++));
    auto new_factored_symbol((factory::create<symbol>(static_cast<uint32_t>(symbols_count++), new_factored_symbol_name, symbol::kind::nonterminal)));

    gr.pool().emplace(pool_type::value_type(new_factored_symbol_name, new_factored_symbol)); // add to symbols pool

    // A -> α A'
    rule_type production(factory::create<rule>(static_cast<uint32_t>(rules_count++), (*nonterminal).name()));

    (*production).add_lhs_symbol(nonterminal);

    std::for_each(factored_prefix.begin(), factored_prefix.end(), [&production](const auto& symb){ (*production).add_rhs_symbol(symb); });
    (*production).add_rhs_symbol(new_factored_symbol);

    gr.rules().emplace_back(production);
    add_nonterminal_rule(gr, nonterminal, production); // nonterminal-rules mapping ...

    for(const auto& factor_rule : factored_rules) // remove processed rules from grammar and from nonterminal-rules mapping
    {
        gr.rules().erase(std::find_if(gr.rules().begin(), gr.rules().end(), [&factor_rule](const auto& rule){ return (*rule).id() == (*factor_rule).id(); }));
        remove_nonterminal_rule(gr, nonterminal, factor_rule);
    }

    // A' -> β1 | ... | βN
    for(const auto& factored_rule : factored_rules)
    {
        rule_type factored_production(factory::create<rule>(static_cast<uint32_t>(rules_count++), new_factored_symbol_name));

        (*factored_production).add_lhs_symbol(new_factored_symbol);

        // check for S -> a b c | a b c A | B | C
        if((*factored_rule).rhs().begin() + factored_prefix.size() == (*factored_rule).rhs().end())
        {
            // S -> a b c
            (*factored_production).add_rhs_symbol(symbol::epsilon);
        }
        else
        {
            //  S -> a b c A | B | C
            std::for_each((*factored_rule).rhs().begin() + factored_prefix.size(),  // S -> a b c A
                            (*factored_rule).rhs().end(),
                            [&factored_production](const auto& symb){ (*factored_production).add_rhs_symbol(symb); });
        }

        gr.rules().emplace_back(factored_production);
        add_nonterminal_rule(gr, new_factored_symbol, factored_production); // nonterminal-rules mapping ...
    }
}

void grammar_algorithm::infix_operator(const typename grammar_algorithm::sets_type& sets1,
                                       const typename grammar_algorithm::sets_type& sets2,
                                       std::size_t k,
                                       typename grammar_algorithm::sets_type& result)
{
    // L1 (+)k L2
    // L1 = { λ, abb }
    // L2 = { b, bab }
    // L1 (+)2 L2 = { b, ba, ab }
    // AU: p.348
    //
    // L1 = { ∅ }
    // L2 = { b, bab }
    // L1 (+)2 L2 = { ∅ }
    //
    // L1 = { λ }
    // L2 = { λ, λ }
    // L1 (+)2 L2 = { λ }
    //
    // Sadkump 2nd ed. p. 499, 3rd ed. p. 575
    // TRUNC3( {a, b, λ} {a} {b} {d} {λ} ) = TRUNC3( { aabd, babd, abd} ) = { aab, bab, abd }
    //
    sets_type result_sets;

    // init sources
    const sets_type& src1(sets1);
    const sets_type& src2(sets2);

    // combine
    for(std::size_t i = 0; i < src1.size(); i++)
    {
        for(std::size_t j = 0; j < src2.size(); j++)
        {
            if(src1[i].empty() || src2[j].empty()) // check { ∅ } annihilation
            {
                continue;
            }

            set_type result_set;

            result_set = src1[i];

            std::copy(src2[j].begin(), src2[j].end(), std::back_inserter(result_set));

            result_sets.emplace_back(result_set);
        }
    }

    // remove lambdas
    for(auto& result_set : result_sets)
    {
        // λ* = λ, keep { λ λ...} as { λ }
        if(std::all_of(result_set.begin(),
                        result_set.end(),
                        [](auto& symb)
                        {
                            return (*symb).id() == (*symbol::epsilon).id();
                        }))
        {
            result_set.resize(1);
        }
        else
        {
            result_set.erase(std::remove_if(result_set.begin(),
                                            result_set.end(),
                                            [](auto& symb)
                                            {
                                                return (*symb).id() == (*symbol::epsilon).id();
                                            }),
                                            result_set.end());
        }
    }

    // resize up to k
    std::for_each(result_sets.begin(), result_sets.end(), [&k](auto& set){ if(set.size() > k) set.resize(k); });

    // make unique
    make_vector_unique(result_sets);

    // copy result, removing empty sets
    std::for_each(result_sets.begin(),
                    result_sets.end(),
                    [&result](const auto& set)
                    {
                        if(!set.empty())
                        {
                            result.emplace_back(set);
                        }
                    });
}

std::vector<std::vector<std::size_t>> grammar_algorithm::cartesian_product(const std::vector<std::vector<std::size_t>>& v)
{
    // https://gist.github.com/thirdwing/953b146ba39c5f5ff562
    std::vector<std::vector<std::size_t>> s = { {} };

    for(auto& u : v)
    {
        std::vector<std::vector<std::size_t>> r;

        for(auto& x : s)
        {
            for(auto y : u)
            {
                r.emplace_back(x);
                r.back().emplace_back(y);
            }
        }

        s.swap(r);
    }

    //for(size_t i = 0; i < s.size(); i++) 
    //{
    //    for(size_t j = 0; j < s[i].size(); j++)
    //    {
    //        std::cout << s[i][j] << "\t";
    //    }
    //    std::cout << std::endl;
    //}

    return s;
}

void grammar_algorithm::infix_operator(const std::vector<typename grammar_algorithm::sets_type>& sets, std::size_t k, typename grammar_algorithm::sets_type& result)
{
    // F'1(A) (+)k F'2(A) ... (+)k ... F'N(A)
    // (+) = ⊕
    if(!sets.empty())
    {
        sets_type result_sets;

        // special case ∅* = λ
        if(std::all_of(sets.begin(), sets.end(), [](const auto& set){ return set.empty(); }))
        {
            result_sets.emplace_back(set_type { symbol::epsilon });
        }
        else
        {
            // build index vectors to calculate cartesian product for { a, ba } { aa, bbb } { cc, fd, gh }
            //                                                          [0,1]      [0,1]       [0,1,2]
            // import itertools                                           ^          ^            ^
            // sets = [ [0, 1], [0,1], [0,1,2] ]  -------------------------------------------------
            // for i, element in enumerate(itertools.product(*sets)):
            //     print(str(i) + ': ' + str(element))
            std::vector<std::vector<std::size_t>> indices;

            for(std::size_t i = 0, n = sets.size(); i < n; i++)
            {
                indices.emplace_back(std::vector<std::size_t>{});

                std::size_t j = 0;

                std::for_each(sets[i].begin(), sets[i].end(), [&indices, i, &j](const auto&){ indices[i].emplace_back(j++); });
            }

            // calculate cartesian product
            //  0: (0, 0, 0)
            //  1: (0, 0, 1)
            //  2: (0, 0, 2)
            //  3: (0, 1, 0)
            //  4: (0, 1, 1)
            //  5: (0, 1, 2)
            //  6: (1, 0, 0)
            //  7: (1, 0, 1)
            //  8: (1, 0, 2)
            //  9: (1, 1, 0)
            // 10: (1, 1, 1)
            // 11: (1, 1, 2)
            std::vector<std::vector<std::size_t>> cartesian_product_result(cartesian_product(indices));

            // combine
            for(const auto& cartesian_product : cartesian_product_result)
            {
                set_type result_set;

                for(std::size_t i = 0, n = sets.size(); i < n; i++)
                {
                    const auto& current_set(sets[i][cartesian_product[i]]);

                    if(current_set.empty())
                    {
                        result_set.clear();
                        break;
                    }

                    std::copy(current_set.begin(), current_set.end(), std::back_inserter(result_set));
                }

                if(!result_set.empty())
                {
                    result_sets.emplace_back(result_set);
                }
            }

            // remove lambdas
            for(auto& result_set : result_sets)
            {
                // λ* = λ, keep { λ λ...} as { λ }
                if(std::all_of(result_set.begin(),
                               result_set.end(),
                               [](auto& symb)
                               {
                                   return (*symb).id() == (*symbol::epsilon).id();
                               }))
                {
                    result_set.resize(1);
                }
                else
                {
                    result_set.erase(std::remove_if(result_set.begin(),
                                                    result_set.end(),
                                                    [](auto& symb)
                                                    {
                                                        return (*symb).id() == (*symbol::epsilon).id();
                                                    }),
                                                    result_set.end());
                }
            }

            // resize up to k
            std::for_each(result_sets.begin(), result_sets.end(), [&k](auto& set){ if(set.size() > k) set.resize(k); });

            // make unique
            make_vector_unique(result_sets);
        }

        // copy result
        std::for_each(result_sets.begin(),
                      result_sets.end(),
                      [&result](const auto& set)
                      {
                          if(!set.empty())
                          {
                              result.emplace_back(set);
                          }
                      });
    }
}

void grammar_algorithm::collect_nonterminals(const grammar& gr, std::vector<typename grammar_algorithm::symbol_type>& nonterminals)
{
    std::for_each(gr.pool().begin(),
                  gr.pool().end(),
                  [&nonterminals](const auto& pool_symbol_kvp)
                  {
                      const auto& pool_symbol(pool_symbol_kvp.second);

                      if((*pool_symbol).nonterminal())
                      {
                          nonterminals.emplace_back(pool_symbol);
                      }
                  });

    std::sort(nonterminals.begin(),
              nonterminals.end(),
              [](const symbol_type& symb1, const symbol_type& symb2)
              {
                  return (*symb1).id() < (*symb2).id();
              });
}

void grammar_algorithm::collect_terminals(const grammar& gr, std::vector<typename grammar_algorithm::symbol_type>& terminals)
{
    std::for_each(gr.pool().begin(),
                  gr.pool().end(),
                  [&terminals](const auto& pool_symbol_kvp)
                  {
                      const auto& pool_symbol(pool_symbol_kvp.second);

                      if((*pool_symbol).terminal())
                      {
                          terminals.emplace_back(pool_symbol);
                      }
                  });

    std::sort(terminals.begin(),
              terminals.end(),
              [](const symbol_type& symb1, const symbol_type& symb2)
              {
                  return (*symb1).id() < (*symb2).id();
              });
}

bool grammar_algorithm::is_less(const typename grammar_algorithm::set_type& seq1, const typename grammar_algorithm::set_type& seq2)
{
    auto it1(seq1.begin());
    auto it1_end(seq1.end());

    auto it2(seq2.begin());
    auto it2_end(seq2.end());

    for(; it1 != it1_end && it2 != it2_end; ++it1, ++it2)
    {
        std::int32_t id1 = static_cast<std::int32_t>((*(*it1)).id()); // cast to int to check λ
        std::int32_t id2 = static_cast<std::int32_t>((*(*it2)).id()); // cast to int to check λ

        if(id1 < id2)
        {
            return true;
        }
        else if(id1 > id2)
        {
            return false;
        }
    }

    return (it1 == it1_end && it2 != it2_end);
}

void grammar_algorithm::make_vector_unique(typename grammar_algorithm::sets_type& sequence)
{
    std::sort(sequence.begin(),
              sequence.end(),
              [](const typename grammar_algorithm::set_type& seq1, const typename grammar_algorithm::set_type& seq2)
              {
                  return is_less(seq1, seq2);
              });

    sequence.erase(std::unique(sequence.begin(), sequence.end() ), sequence.end());
}

void grammar_algorithm::build_synch_set(const grammar& gr, typename grammar_algorithm::set_type& result)
{
    //  Building SYNCH:
    //    1. a ∈ FOLLOW(A) => a ∈SYNCH(A)
    //    2. place keywords that start statements in SYNCH(A)
    //    3. add symbols in FIRST(A) to SYNCH(A)
    set_type synch_set;

    gr;//??

    result.swap(synch_set);
}

END_NAMESPACE
