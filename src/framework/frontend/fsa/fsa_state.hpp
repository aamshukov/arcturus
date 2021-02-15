//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __FSA_STATE_H__
#define __FSA_STATE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

#define DUMMY_START_STATE_NAME  (L"START")
#define DUMMY_END_STATE_NAME    (L"END")

class fsa_state
{
    public:
        using token_type = std::size_t;

        using state_type = std::shared_ptr<fsa_state>;
        using states_type = std::map<std::size_t, state_type>;

        using transition_type = std::shared_ptr<fsa_transition>;
        using transitions_type = std::map<std::size_t, transition_type>;

    private:
        std::size_t             my_id;
        string_type             my_label;

        token_type              my_token;       // final state token type

        transitions_type        my_transitions; // outcomming transitions

        bool                    my_marked;

    private:
        transitions_type        clone_transitions() const;

    public:
                                fsa_state();
                                fsa_state(const string_type& label, token_type token);

                                fsa_state(const fsa_state& other);
                                fsa_state(fsa_state&& other);

        fsa_state&              operator = (const fsa_state& other);
        fsa_state&              operator = (fsa_state&& other);

        std::size_t             id() const;
        std::size_t&            id();

        const string_type&      label() const;
        string_type&            label();

        token_type              token() const;
        token_type&             token();

        const transitions_type& transitions() const;
        transitions_type&       transitions();

        bool                    marked() const;
        bool&                   marked();

        transition_type         match_transition(const transition_type& transition) const;

        state_type              clone() const;
};

inline std::size_t fsa_state::id() const
{
    return my_id;
}

inline std::size_t& fsa_state::id()
{
    return my_id;
}

inline const string_type& fsa_state::label() const
{
    return my_label;
}

inline string_type& fsa_state::label()
{
    return const_cast<string_type&>(static_cast<const fsa_state&>(*this).label());
}

inline typename fsa_state::token_type fsa_state::token() const
{
    return my_token;
}

inline typename fsa_state::token_type& fsa_state::token()
{
    return my_token;
}

inline const typename fsa_state::transitions_type& fsa_state::transitions() const
{
    return my_transitions;
}

inline typename fsa_state::transitions_type& fsa_state::transitions()
{
    return const_cast<transitions_type&>(static_cast<const fsa_state&>(*this).transitions());
}

inline bool fsa_state::marked() const
{
    return my_marked;
}

inline bool& fsa_state::marked()
{
    return my_marked;
}

END_NAMESPACE

#endif // __FSA_STATE_H__
