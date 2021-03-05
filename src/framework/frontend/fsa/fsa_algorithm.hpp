//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __FSA_ALGORITHM_H__
#define __FSA_ALGORITHM_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class fsa_algorithm : private noncopyable
{
    public:
        using token_type = typename fsa::token_type;

        using state_type = typename fsa::state_type;
        using states_type = typename fsa::states_type;

        using transition_type = typename fsa::transition_type;
        using transitions_type = typename fsa::transitions_type;

        using predicate_type = typename fsa::predicate_type;
        using predicates_type = typename fsa::predicates_type;

        using fsa_type = typename fsa::fsa_type;

        using state_set_type = std::shared_ptr<fsa_state_set>;

        using equivalence_class_type = std::map<std::size_t, state_type>;
        using equivalence_classes_type = std::vector<equivalence_class_type>;

    private:
        static state_set_type           get_unmarked_dstate(const std::vector<state_set_type>& states);
        static state_set_type           get_dstate(const std::vector<state_set_type>& states, state_set_type state);

        static state_set_type           calculate_state_eclosure(const fsa_type& fsa0, const state_type& state);
        static state_set_type           calculate_states_eclosure(const fsa_type& fsa0, const state_set_type& state_set);

        static state_set_type           calculate_move(const states_type& states, const state_set_type& state_set, const predicate_type& predicate);

        static void                     combine_transitions(const typename fsa_algorithm::state_type& state1,
                                                            const typename fsa_algorithm::state_type& state2,
                                                            std::vector<typename fsa_algorithm::transition_type>& transitions);
        
        static std::size_t              get_equivalence_class_index(const state_type& state, const equivalence_classes_type& equivalence_classes);
        static string_type              decorate_equivalence_class(const equivalence_class_type& equivalence_class);

        static bool                     states_equivalent(const typename fsa_algorithm::fsa_type& fsa0,
                                                              const equivalence_classes_type& equivalence_classes,
                                                              const state_type& state1,
                                                              const state_type& state2);

        static void                     build_fsa_from_equivalence_classes(const fsa_type& fsa0, const equivalence_classes_type& equivalence_classes, fsa_type& result_fsa);

        static void                     dump_equivalence_classes(const equivalence_classes_type& equivalence_classes);

    public:
        static void                     remove_useless_states(const fsa_type& fsa0, fsa_type& result_fsa);
        static void                     build_equivalence_class_combinations(const equivalence_class_type& equivalence_class,
                                                                             std::vector<std::pair<std::pair<std::size_t, state_type>, std::pair<std::size_t, state_type>>>& combinations);

        static void                     build_k_equivalence_classes0(const fsa_type& fsa0, equivalence_classes_type& equivalence_classes);
        static void                     build_k_equivalence_classes(const fsa_type& fsa0, equivalence_classes_type& equivalence_classes);

    public:
        static bool                     nfa_to_dfa(const fsa_type& fsa0, fsa_type& result_fsa);
        static void                     minimize_dfa(const fsa_type& fsa0, fsa_type& result_fsa, bool cleanup = true);

        static std::size_t              validate_dfa(const fsa_type& fsa0, const string_type& input);
};

END_NAMESPACE

#endif // __FSA_ALGORITHM_H__
