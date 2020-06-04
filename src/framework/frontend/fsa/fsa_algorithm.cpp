//..............................
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

#include <core/counter.hpp>

#include <frontend/fsa/fsa_transition.hpp>
#include <frontend/fsa/fsa_state.hpp>
#include <frontend/fsa/fsa.hpp>
#include <frontend/fsa/fsa_state_set.hpp>
#include <frontend/fsa/fsa_algorithm.hpp>

#define FSA_MAX_STATES (512)

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

typename fsa_algorithm::state_set_type fsa_algorithm::calculate_state_eclosure(const typename fsa_algorithm::fsa_type& fsa0, const fsa_algorithm::state_type& state)
{
    state_set_type state_set(factory::create<fsa_state_set>());

    (*state_set).states().emplace((*state).id(), state);

    state_set_type result(calculate_states_eclosure(fsa0, state_set));

    return result;
}

typename fsa_algorithm::state_set_type fsa_algorithm::calculate_states_eclosure(const typename fsa_algorithm::fsa_type& fsa0, const state_set_type& state_set)
{
    // Calculate e-closure:
    //  Push all states in T onto stack;
    //  Initialize e-closure(T) to T;
    //  While stack is not empty do
    //      Pop t;
    //      For each transition (t, u) labeled e-transition do
    //          If u is not in e-closure(T) then
    //              Add u to e-closure(T);
    //              Push u;
    //      End;
    //  End;
    state_set_type result(factory::create<fsa_state_set>());

    std::stack<std::pair<uint32_t, state_type>> stack;

    for(const auto& state : (*state_set).states())
    {
        stack.push(state);
    }

    (*result).states().insert((*state_set).states().begin(), (*state_set).states().end());

    while(!stack.empty())
    {
        auto t_state(stack.top());

        stack.pop();

        for(const auto& transition_kvp : (*t_state.second).transitions())
        {
            const auto& transition(transition_kvp.second);

            if((*transition).is_epsilon_transition())
            {
                const auto& u_state((*fsa0).states()[(*transition).end_state()]);

                if((*result).states().find((*u_state).id()) == (*result).states().end())
                {
                    (*result).states().emplace((*u_state).id(), u_state);
                    stack.push(std::make_pair((*u_state).id(), u_state));
                }
            }
        }
    }

    return result;
}

typename fsa_algorithm::state_set_type fsa_algorithm::calculate_move(const typename fsa_algorithm::states_type& states,
                                                                     const typename fsa_algorithm::state_set_type& state_set,
                                                                     const typename fsa_algorithm::predicate_type& predicate)
{
    // Algorithm to compute T=Move(S,c)
    //  T = 0;
    //  for each state s in S
    //  {
    //      for each edge e from s to some s’
    //      {
    //          if(e is labelled with c)
    //          {
    //              T = T U closure({s’});
    //          }
    //      }
    //  }
    state_set_type result(factory::create<fsa_state_set>());

    for(const auto& state_kvp : (*state_set).states())
    {
        const auto& state(state_kvp.second);

        for(const auto& transition_kvp : (*state).transitions())
        {
            const auto& transition(transition_kvp.second);

            if((*transition).predicate().compare(predicate) == 0)
            {
                const auto& end_state((*states.find((*transition).end_state())).second);

                (*result).states().emplace((*end_state).id(), end_state);
            }
        }
    }

    return result;
}

void fsa_algorithm::remove_useless_states(const fsa_type& fsa0, fsa_type& result_fsa)
{
    // Remove all the DFA states which are either:
    //     unreachable from the initial state
    //       or
    //     cannot reach any accept-state
    assert((*fsa0).states().size() < FSA_MAX_STATES);

    // initialization
    bool tc[FSA_MAX_STATES][FSA_MAX_STATES]; // transitive closure

    memset(tc, 0, sizeof(tc));

    for(const auto& kvp_state : (*fsa0).states())
    {
        const auto& state(kvp_state.second);

        tc[(*state).id()][(*state).id()] = true;

        for(const auto& transition_kvp : (*state).transitions())
        {
            const auto& transition(transition_kvp.second);
            tc[(*transition).start_state()][(*transition).end_state()] = true;
        }
    }

    // calculate transitive closure, Warshall's algorithm, gde to utashil
    std::size_t n = (*fsa0).states().size();

    for(uint32_t i = 0; i < n; i++)
    {
        for(uint32_t j = 0; j < n; j++)
        {
            if(tc[j][i])
            {
                for(uint32_t k = 0; k <= n; k++)
                {
                    if(tc[i][k])
                    {
                        tc[j][k] = true;
                    }
                }
            }
        }
    }

    // cleanup
    fsa_type fsa1((*fsa0).clone());

    std::vector<uint32_t> states_to_remove;

    // remove all the states not reachable from initial state
    for(uint32_t i = 0; i < n; i++)
    {
        if(!tc[(*(*fsa1).start_state()).id()][i])
        {
            states_to_remove.push_back(i);
        }
    }

    // remove all states not reaching a final state
    for(uint32_t i = 0; i < n; i++)
    {
        bool reached = false;

        // loop over final states
        for(const auto& kvp_state : (*fsa1).final_states())
        {
            const auto& final_state(kvp_state.second);

            if(tc[i][(*final_state).id()]) // state i reaches a final state
            {
                reached = true;
                break;
            }
        }

        if(!reached)
        {
            states_to_remove.push_back(i);
        }
    }

    for(auto id : states_to_remove)
    {
        (*fsa1).remove_state(id);
    }

    result_fsa.swap(fsa1);
}

typename fsa_algorithm::state_set_type fsa_algorithm::get_unmarked_dstate(const std::vector<typename fsa_algorithm::state_set_type>& states)
{
    state_set_type result;

    for(const auto& state : states)
    {
        if(!(*state).marked())
        {
            result = state;
            break;
        }
    }

    return result;
}

typename fsa_algorithm::state_set_type fsa_algorithm::get_dstate(const std::vector<typename fsa_algorithm::state_set_type>& states,
                                                                 typename fsa_algorithm::state_set_type new_state)
{
    state_set_type result;

    for(const auto& state : states)
    {
        if(*state == *new_state)
        {
            result = state;
            break;
        }
    }

    return result;
}

bool fsa_algorithm::nfa_to_dfa(const typename fsa_algorithm::fsa_type& fsa0, typename fsa_algorithm::fsa_type& result_fsa)
{
    // Conversion of an NFA into a DFA (subset construction)
    // .....................................................
    // Input: An NFA N.
    // Output: A DFA D accepting the same language.
    // Operations:
    //  e-closure(s) is the set of NFA states reachable from s on e-transitions alone.
    //  e-closure(T) is the union of e-closure(r) for all r in T.
    //  move(T, a) is the set of NFA states to which there is a transition on input a from some NFA state in T.
    //
    //  set the start state to e-closure(s0) and unmark it.
    //  While there is an unmarked state T in Dstates do
    //      Mark T
    //      For each input symbol a do
    //          U := e-closure(move(T, a));
    //          If U is not in Dstates then
    //              Add U as an unmarked state to Dstates;
    //          Dtran(T, a) := U;
    //      End;
    //  End;
    log_info(L"Converting NFA to DFA ...");

    bool result = false;

    predicates_type predicates((*fsa0).predicates());

    try
    {
        auto fsa1(factory::create<fsa>());

        std::vector<state_set_type> dstates;

        auto start_state(calculate_state_eclosure(fsa0, (*fsa0).start_state()));

        dstates.emplace_back(start_state);

        for(;;)
        {
            auto t_state(get_unmarked_dstate(dstates));

            if(t_state == nullptr)
            {
                break;
            }

            (*t_state).marked() = true;

            std::pair<state_type, bool> dfa_t_state((*t_state).build_fsa_state());

            if(dfa_t_state.second)
            {
                (*fsa1).add_state(dfa_t_state.first);
            }

            for(const auto& predicate : predicates)
            {
                auto move(calculate_move((*fsa0).states(), t_state, predicate));
                auto eclosure(calculate_states_eclosure(fsa0, move));

                if((*eclosure).states().empty())
                {
                    continue;
                }

                auto u_state(get_dstate(dstates, eclosure));

                std::pair<state_type, bool> dfa_u_state(u_state == nullptr ? (*eclosure).build_fsa_state() : (*u_state).build_fsa_state());

                if(u_state == nullptr)
                {
                    u_state = eclosure;
                    dstates.emplace_back(u_state);

                    if(dfa_u_state.second)
                    {
                        (*fsa1).add_state(dfa_u_state.first);
                    }
                }

                (*fsa1).add_transition(dfa_t_state.first, dfa_u_state.first, predicate);
            }
        }

        (*fsa1).start_state() = (*start_state).dfa_state();

        for(const auto& dstate : dstates)
        {
            for(const auto& state_kvp : (*dstate).states())
            {
                const auto& state(state_kvp.second);

                if((*fsa0).is_final_state(state))
                {
                    (*fsa1).add_final_state((*dstate).dfa_state());
                    (*(*dstate).dfa_state()).token() = (*state).token();

                    break; // the first added state has priority, like 'byte'-kw and 'byte_'-identifier
                }
            }
        }

        result_fsa.swap(fsa1);

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::fsa,
                            L"Converting NFA to DFA: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Converted NFA to DFA.");

    return result;
}

void fsa_algorithm::combine_transitions(const typename fsa_algorithm::state_type& state1,
                                        const typename fsa_algorithm::state_type& state2,
                                        std::vector<typename fsa_algorithm::transition_type>& transitions)
{
    for(const auto& transition_kvp : (*state1).transitions())
    {
        const auto& transition(transition_kvp.second);
        transitions.emplace_back(transition);
    }

    for(const auto& transition_kvp : (*state2).transitions())
    {
        const auto& transition(transition_kvp.second);
        transitions.emplace_back(transition);
    }
}

uint32_t fsa_algorithm::get_equivalence_class_index(const state_type& state, const equivalence_classes_type& equivalence_classes)
{
    uint32_t result = INVALID_VALUE;

    for(std::size_t k = 0, n = equivalence_classes.size(); k < n; k++)
    {
        if(equivalence_classes[k].find((*state).id()) != equivalence_classes[k].end())
        {
            result = static_cast<uint32_t>(k);
            break;
        }
    }

    return result;
}

string_type fsa_algorithm::decorate_equivalence_class(const typename fsa_algorithm::equivalence_class_type& equivalence_class)
{
    string_type result;

    std::vector<uint32_t> states;

    std::for_each(equivalence_class.begin(), equivalence_class.end(), [&states](const auto& state_kvp) { states.emplace_back(state_kvp.first); });

    std::sort(states.begin(), states.end(), [] (auto id1, auto id2) { return id1 < id2; });

    std::for_each(states.begin(), states.end(), [&states, &result](uint32_t state) { result += L" " + std::to_wstring(state); });

    return result;
}

void typename fsa_algorithm::build_equivalence_class_combinations(const typename fsa_algorithm::equivalence_class_type& equivalence_class,
                                                                  std::vector<std::pair<std::pair<uint32_t,
                                                                  typename fsa_algorithm::state_type>,
                                                                  std::pair<uint32_t, typename fsa_algorithm::state_type>>>& combinations)
{
    /* 24.12.2008 last modification: 26.06.2013
    Copyright (c) 2008-2013 by Siegfried Koepf

    This file is distributed under the terms of the GNU General Public License
    version 3 as published by the Free Software Foundation.
    For information on usage and redistribution and for a disclaimer of all
    warranties, see the file COPYING in this distribution.

    k-combinations without repetition in lexicographic order

    Algorithm by Siegfried Koepf, inspired by Donald Knuth and many others

    Functions:
      int gen_comb_norep_lex_init(unsigned char *vector, const unsigned char n, const unsigned char k)
        Test for special cases
        Initialization of vector
        Possible return values are: GEN_ERROR, GEN_EMPTY, GEN_NEXT

      int gen_comb_norep_lex_next(unsigned char *vector, const unsigned char n, const unsigned char k)
        Transforms current figure in vector into its successor
        Possible return values are: GEN_NEXT, GEN_TERM

    Arguments:
      unsigned char *vector; //pointer to the array where the current figure is stored
      const unsigned char n; //length of alphabet
      const unsigned char k; //length of figures

    Usage and restrictions:
      Arguments and elements in vector are restricted to the interval (0, 255)
      Memory allocation for vector must be provided by the calling process
      k must be <= n

    Cardinality:
      n! / ((n - k)! * k!) == Binomial(n, k)
    */
    // returns all possible combinations of states for further validation if they are pair-wise equivalent
    // all r-combinations of {1,2,...,n} in increasing lexicographic order (sochetaniya r iz n)
    // http://www.aconnect.de/friends/editions/computer/generate/comb_norep_lex.c
    // A:B, A:C, A:D, A:E, A:F, B:C, B:D, B:E, B:F, C:D, C:E, C:F, D:E, D:F, E:F
    std::vector<std::pair<int, int>> indices;

    const int r = 2; // are two (2) states equivalet?

    int n = static_cast<int>(equivalence_class.size());

    std::array<int, r> s;

    for(int i = 0; i < r; i++)
    {
        s[i] = i;
    }

    indices.emplace_back(std::pair<int, int>(s[0], s[1])); // first combination

    // magic ...
    int cnr = factorial(n)/(factorial(n - r) * factorial(r)); // C(n,r)

    for(int i = 1; i < cnr; i++)
    {
        int j;

        if(s[r - 1] < n - 1)
        {
            s[r - 1]++;
        }
        else
        {
            for(j = r - 2; j >= 0; j--)
            {
                if(s[j] < n - r + j)
                {
                    break;
                }
            }

            if(!(j < 0))
            {
                s[j]++;

                while(j < r - 1)
                {
                    s[j + 1] = s[j] + 1;
                    j++;
                }
            }
        }

        indices.emplace_back(std::pair<int, int>(s[0], s[1]));
    }

    // populate according indices ...
    std::vector<std::pair<uint32_t, state_type>> states(equivalence_class.begin(), equivalence_class.end());

    std::sort(states.begin(), states.end(), [] (const auto& s1_kvp, const auto& s2_kvp) { return s1_kvp.first < s2_kvp.first; });

    for(const auto& index : indices)
    {
        combinations.emplace_back(states[index.first], states[index.second]);
    }
}

bool fsa_algorithm::states_equivalent(const typename fsa_algorithm::fsa_type& fsa0,
                                      const typename fsa_algorithm::equivalence_classes_type& equivalence_classes,
                                      const typename fsa_algorithm::state_type& state1,
                                      const typename fsa_algorithm::state_type& state2)
{
    bool result = state1 == state2;

    if(!result) // states are not the same
    {
        // loop on all predicates relevant for these two states only,
        // as automaton is deterministic - every state has at most one transition with given predicate
        std::vector<transition_type> transitions;

        combine_transitions(state1, state2, transitions);

        for(const auto& transition : transitions)
        {
            const auto& transition1((*state1).match_transition(transition));
            const auto& transition2((*state2).match_transition(transition));

            result = transition1 && transition2;

            if(result) // both must have transitions on the same predicate
            {
                auto state1_to_find_id((*transition1).end_state());
                auto state2_to_find_id((*transition2).end_state());

                const auto& state1_to_find((*fsa0).states()[state1_to_find_id]);
                const auto& state2_to_find((*fsa0).states()[state2_to_find_id]);

                result = state1_to_find == state2_to_find;

                if(!result) // states are still not the same
                {
                    // attempt to find a group with these two states
                    auto index1 = get_equivalence_class_index(state1_to_find, equivalence_classes);
                    auto index2 = get_equivalence_class_index(state2_to_find, equivalence_classes);

                    result = index1 == index2;
                }
            }

            if(!result)
            {
                break;
            }
        }
    }

    return result;
}

void fsa_algorithm::build_k_equivalence_classes0(const typename fsa_algorithm::fsa_type& fsa0, typename fsa_algorithm::equivalence_classes_type& equivalence_classes)
{
    using set_type = equivalence_class_type;    // {A, B, C, D}
    using sets_type = equivalence_classes_type; // {A, B, C, D} {E} {F, G} after k-th iteration

    sets_type sets;     // holds k equivalence classes at k-th iteration
    sets_type new_sets; // working set of equivalence classes

    // 0-equivalence, k = 0, {Q-F} and {F}
    set_type state_set;
    set_type final_state_set;

    for(const auto& state_kvp : (*fsa0).states())
    {
        auto state(state_kvp.second);

        if((*fsa0).is_final_state(state))
        {
            final_state_set.emplace((*state).id(), state);
        }
        else
        {
            state_set.emplace((*state).id(), state);
        }
    }

    sets.emplace_back(state_set);
    sets.emplace_back(final_state_set);

    // k-equivalence (k > 0)
    for(;;)
    {
        for(const auto& set : sets)
        {
            if(set.size() == 1)
            {
                new_sets.emplace_back(set);
                continue;
            }

            std::vector<std::pair<std::pair<uint32_t, state_type>, std::pair<uint32_t, state_type>>> states_combinations;

            build_equivalence_class_combinations(set, states_combinations);

            for(const auto& states_combination : states_combinations)
            {
                if(states_equivalent(fsa0, sets, states_combination.first.second, states_combination.second.second))
                {
                    uint32_t index = get_equivalence_class_index(states_combination.first.second, new_sets);

                    if(index == INVALID_VALUE)
                    {
                        index = get_equivalence_class_index(states_combination.second.second, new_sets);
                    }

                    if(index != INVALID_VALUE)
                    {
                        new_sets[index].emplace(states_combination.first);
                        new_sets[index].emplace(states_combination.second);
                    }
                    else
                    {
                        set_type set0;

                        set0.emplace(states_combination.first);
                        set0.emplace(states_combination.second);

                        new_sets.emplace_back(set0);
                    }
                }
                else
                {
                    state_type fs_states[] = { states_combination.first.second, states_combination.second.second };

                    for(const auto& fs_state : fs_states)
                    {
                        uint32_t index = get_equivalence_class_index(fs_state, new_sets);

                        if(index != INVALID_VALUE)
                        {
                            new_sets[index].emplace((*fs_state).id(), fs_state);
                        }
                        else
                        {
                            set_type set0;
                            set0.emplace((*fs_state).id(), fs_state);
                            new_sets.emplace_back(set0);
                        }
                    }
                }
            }
        }

        if(sets.size() == new_sets.size()) // while changes occurres
        {
            break;
        }

        sets.assign(new_sets.begin(), new_sets.end());
        new_sets.clear();
    }

    // clean up 'free-standing' states as we considered all k-combinations (sochetaniya)
    for(auto it = sets.begin();  it != sets.end();)
    {
        if((*it).size() == 1)
        {
            const auto& state_to_find(*(*it).begin());

            bool remove = false;

            for(const auto& set : sets)
            {
                if(set.size() > 1 && set.find(state_to_find.first) != set.end())
                {
                    remove = true;
                    break;
                }
            }

            remove ? it = sets.erase(it) : ++it;
        }
        else
        {
            ++it;
        }
    }

    // result
    equivalence_classes.assign(sets.begin(), sets.end());
}

void fsa_algorithm::build_k_equivalence_classes(const typename fsa_algorithm::fsa_type& fsa0, typename fsa_algorithm::equivalence_classes_type& equivalence_classes)
{
    using set_type = equivalence_class_type;    // {A, B, C, D}
    using sets_type = equivalence_classes_type; // {A, B, C, D} {E} {F, G} after k-th iteration

    sets_type sets;     // holds k equivalence classes at k-th iteration
    sets_type new_sets; // working set of equivalence classes

    // 0-equivalence, k = 0, {Q-F} and {F}
    set_type state_set;
    sets_type final_state_sets;

    for(const auto& state_kvp : (*fsa0).states())
    {
        const auto& state(state_kvp.second);

        if((*fsa0).is_final_state(state))
        {
            // for lexical analyzer - should separate final states by tokens into different groups
            bool state_added = false;

            for(auto& final_state_set : final_state_sets)
            {
                for(const auto& final_state : final_state_set)
                {
                    if((*final_state.second).token() == (*state).token())
                    {
                        final_state_set.emplace((*state).id(), state);
                        state_added = true;
                        break;
                    }
                }
            }

            if(!state_added)
            {
                set_type set0;
                set0.emplace((*state).id(), state);
                final_state_sets.emplace_back(set0);
            }
        }
        else
        {
            state_set.emplace((*state).id(), state);
        }
    }

    if(!state_set.empty())
    {
        sets.emplace_back(state_set);
    }

    if(!final_state_sets.empty())
    {
        for_each(final_state_sets.begin(), final_state_sets.end(), [&sets](const auto& final_state_set){ sets.emplace_back(final_state_set); });
    }

    // k-equivalence (k > 0)
    for(;;)
    {
        for(const auto& set : sets)
        {
            // do not consider singletons
            if(set.size() == 1)
            {
                new_sets.emplace_back(set);
                continue;
            }

            // index to start search ortogonal sets in new-sets!!
            auto new_sets_index = new_sets.size();

            // the first state introduces a new set
            auto it(set.begin());

            set_type set0;

            set0.emplace(*it);
            new_sets.emplace_back(set0);

            ++it; // skip the added state

            // loop over all states in the current set (excluding the first one above)
            for(auto it_end(set.end()); it != it_end; ++it)
            {
                bool state_added = false;

                const auto& state(*it);

                // check if state is not ortogonal to any new-set
                for(auto k = new_sets_index; k < new_sets.size(); k++)
                {
                    auto& new_set = new_sets[k];

                    const auto& new_state(new_set.begin());

                    if(states_equivalent(fsa0, sets, state.second, (*new_state).second))
                    {
                        new_set.emplace(state);
                        state_added = true;
                        break;
                    }
                }

                if(!state_added)
                {
                    // if state is ortogonal - 'split'
                    // introduce a new set and add state to this new-sets
                    set_type set1;
                    set1.emplace(state);
                    new_sets.emplace_back(set1);
                }
            }
        }

        if(sets.size() == new_sets.size()) // while changes occurres
        {
            break;
        }

        sets.assign(new_sets.begin(), new_sets.end());
        new_sets.clear();
    }

    // result
    equivalence_classes.assign(sets.begin(), sets.end());
}

void fsa_algorithm::build_fsa_from_equivalence_classes(const typename fsa_algorithm::fsa_type& fsa0,
                                                       const typename fsa_algorithm::equivalence_classes_type& equivalence_classes,
                                                       typename fsa_algorithm::fsa_type& result_fsa)
{
    fsa_type fsa1(factory::create<fsa>());

    // build states
    for(std::size_t k = 0, n = equivalence_classes.size(); k < n; k++)
    {
        const auto& equivalence_class(equivalence_classes[k]);

        auto new_state(factory::create<fsa_state>(decorate_equivalence_class(equivalence_class), 0));

        (*fsa1).add_state(new_state);

        if((*fsa1).start_state() == nullptr)
        {
            for(const auto& state_kvp : equivalence_class)
            {
                const auto& state(state_kvp.second);

                if((*fsa0).is_start_state(state))
                {
                    (*fsa1).start_state() = state;
                    break;
                }
            }
        }

        for(const auto& state_kvp : equivalence_class)
        {
            const auto& state(state_kvp.second);

            if((*fsa0).is_final_state(state))
            {
                (*fsa1).add_final_state(new_state);
                (*new_state).token() = (*state).token();
                break;
            }
        }
    }

    // build transitions
    for(uint32_t k = 0, n = static_cast<uint32_t>(equivalence_classes.size()); k < n; k++)
    {
        const auto& equivalence_class(equivalence_classes[k]);

        // new transition start state in minimized fsa1
        const auto& min_fsa_start_state((*fsa1).states()[k]);

        // representative state in equivalence class, from original fsa0
        const auto& fsa_start_state(*equivalence_class.begin());

        for(const auto& transition_kvp : (*fsa_start_state.second).transitions())
        {
            const auto& transition(transition_kvp.second);

            // new transition end state in equivalence class, from original fsa0
            const auto& fsa_end_state((*fsa0).states()[(*transition).end_state()]);

            // find equivalence class containing new transition end state from original fsa0
            for(uint32_t j = 0, m = static_cast<uint32_t>(equivalence_classes.size()); j < m; j++)
            {
                const auto& equivalence_class0(equivalence_classes[j]);

                if(equivalence_class0.find((*fsa_end_state).id()) != equivalence_class0.end())
                {
                    // new transition end state in minimized fsa1
                    const auto& min_fsa_end_state((*fsa1).states()[j]);

                    (*fsa1).add_transition(min_fsa_start_state, min_fsa_end_state, (*transition).predicate());

                    break;
                }
            }
        }
    }

    result_fsa.swap(fsa1);
}

void fsa_algorithm::dump_equivalence_classes(const typename fsa_algorithm::equivalence_classes_type& equivalence_classes)
{
    for(const auto& equivalence_class : equivalence_classes)
    {
        std::wcout << L" { ";
        std::for_each(equivalence_class.begin(),
                      equivalence_class.end(),
                      [](const auto& state_kvp){ std::wcout << L"'" << (*state_kvp.second).label().c_str() << L"' "; });
        std::wcout << L"} ";
    }

    std::wcout << std::endl;
}

void fsa_algorithm::minimize_dfa(const typename fsa_algorithm::fsa_type& fsa0, typename fsa_algorithm::fsa_type& result_fsa, bool cleanup)
{
    log_info(L"Minimizing DFA ...");

    fsa_type fsa1;

    if(cleanup)
    {
        remove_useless_states(fsa0, fsa1);
    }

    equivalence_classes_type equivalence_classes;

    build_k_equivalence_classes(cleanup ? fsa1 : fsa0, equivalence_classes);

    // dump_equivalence_classes(equivalence_classes);

    fsa_type fsa2;

    build_fsa_from_equivalence_classes(cleanup ? fsa1 : fsa0, equivalence_classes, fsa2);

    fsa_type fsa3;

    if(cleanup)
    {
        remove_useless_states(fsa2, fsa3);
    }

    result_fsa.swap(cleanup ? fsa3 : fsa2);

    log_info(L"Minimized DFA .");
}

uint32_t fsa_algorithm::validate_dfa(const typename fsa_algorithm::fsa_type& fsa0, const string_type& input)
{
    log_info(L"Validating DFA ...");

    uint32_t result = 0; // unknown

    auto state((*fsa0).start_state());

    std::size_t k = 0;

    for(auto ch : input)
    {
        k++;

        bool moved = false;

        for(const auto& transition_kvp : (*state).transitions())
        {
            const auto& transition(transition_kvp.second);

            if((*transition).predicate().compare(string_type(1, ch)) == 0)
            {
                state = (*fsa0).states()[(*transition).end_state()];
                moved = true;
                break;
            }
        }

        if(!moved)
        {
            break;
        }
    }

    if(k == input.size() && (*fsa0).is_final_state(state))
    {
        result = (*state).token();
    }

    log_info(L"Validated DFA '%s'.", (result ? L"successfuly" : L"with error"));

    return result;
}

END_NAMESPACE
