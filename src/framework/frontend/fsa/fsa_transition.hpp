//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FSA_TRANSITION_H__
#define __FSA_TRANSITION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class fsa_transition
{
    public:
        using predicate_type = string_type;
        using predicates_type = std::set<predicate_type>;

    private:
        uint32_t                        my_id;

        uint32_t                        my_start_state;
        uint32_t                        my_end_state;

        predicate_type                  my_predicate;
        datum_type                      my_switch_predicate; // short text predicate, indicates if predicate has simple form, and can be used in switched code generation

        int16_t                         my_rank;            // rank is a special attribute of a transition and it is used only during generation
                                                            // of code when "goto"-driven lexical analyzer is produced, allowing to shift
                                                            // more common predicates down in "goto" statements, default value is 0 and
                                                            // a rank with bigger value is more advanced in a "goto" statement,
                                                            // for example recognition of identifiers uses more general predicate
                                                            // is_identifier_part() than recognition of a keyword "while"

    public:
                                        fsa_transition();
                                        fsa_transition(uint32_t start_state, uint32_t end_state, const predicate_type& predicate);
                                        fsa_transition(uint32_t start_state, uint32_t end_state, datum_type switch_predicate);

                                        fsa_transition(const fsa_transition& other);
                                        fsa_transition(fsa_transition&& other);

        fsa_transition&                 operator = (const fsa_transition& other);
        fsa_transition&                 operator = (fsa_transition&& other);

        uint32_t                        id() const;
        uint32_t&                       id();

        uint32_t                        start_state() const;
        uint32_t&                       start_state();

        uint32_t                        end_state() const;
        uint32_t&                       end_state();

        const predicate_type&           predicate() const;
        predicate_type&                 predicate();

        datum_type                      switch_char() const;
        datum_type&                     switch_char();

        int16_t                         rank() const;
        int16_t&                        rank();

        bool                            is_epsilon_transition() const;

        static const predicate_type&    empty_predicate();
        static const predicate_type&    epsilon_predicate();
};

inline uint32_t fsa_transition::id() const
{
    return my_id;
}

inline uint32_t& fsa_transition::id()
{
    return my_id;
}

inline uint32_t fsa_transition::start_state() const
{
    return my_start_state;
}

inline uint32_t& fsa_transition::start_state()
{
    return my_start_state;
}

inline uint32_t fsa_transition::end_state() const
{
    return my_end_state;
}

inline uint32_t& fsa_transition::end_state()
{
    return my_end_state;
}

inline const typename fsa_transition::predicate_type& fsa_transition::predicate() const
{
    return my_predicate;
}

inline typename fsa_transition::predicate_type& fsa_transition::predicate()
{
    return const_cast<typename fsa_transition::predicate_type&>(static_cast<const fsa_transition&>(*this).predicate());
}

inline datum_type fsa_transition::switch_char() const
{
    return my_switch_predicate;
}

inline datum_type& fsa_transition::switch_char()
{
    return my_switch_predicate;
}

inline int16_t fsa_transition::rank() const
{
    return my_rank;
}

inline int16_t& fsa_transition::rank()
{
    return my_rank;
}

inline bool fsa_transition::is_epsilon_transition() const
{
    return my_predicate.compare(epsilon_predicate()) == 0 || my_switch_predicate == text::epsilon_codepoint();
}

inline typename fsa_transition::predicate_type const& fsa_transition::empty_predicate()
{
    static typename fsa_transition::predicate_type result;
    return result;
}

inline string_type const& fsa_transition::epsilon_predicate()
{
    static string_type result(L"ε");
    return result;
}

END_NAMESPACE

#endif // __FSA_TRANSITION_H__
