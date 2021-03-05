//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __FSA_STATE_SET_H__
#define __FSA_STATE_SET_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class fsa_state_set
{
    public:
        using state_type = typename fsa_state::state_type;
        using states_type = typename fsa_state::states_type;

    private:
        states_type                 my_states;
        state_type                  my_dfa_state;

        bool                        my_marked;

    public:
                                    fsa_state_set();

                                    fsa_state_set(const fsa_state_set& other);
                                    fsa_state_set(fsa_state_set&& other);

        fsa_state_set&              operator = (const fsa_state_set& other);
        fsa_state_set&              operator = (fsa_state_set&& other);

        const states_type&          states() const;
        states_type&                states();

        const state_type&           dfa_state() const;
        state_type&                 dfa_state();

        bool                        marked() const;
        bool&                       marked();

        std::pair<state_type, bool> build_fsa_state();

        string_type                 decorate() const;

        friend bool                 operator == (const fsa_state_set& lhs, const fsa_state_set& rhs);
        friend bool                 operator != (const fsa_state_set& lhs, const fsa_state_set& rhs);
};

inline const typename fsa_state_set::states_type& fsa_state_set::states() const
{
    return my_states;
}

inline typename fsa_state_set::states_type& fsa_state_set::states()
{
    return const_cast<states_type&>(static_cast<const fsa_state_set&>(*this).states());
}

inline const typename fsa_state_set::state_type& fsa_state_set::dfa_state() const
{
    return my_dfa_state;
}

inline typename fsa_state_set::state_type& fsa_state_set::dfa_state()
{
    return const_cast<state_type&>(static_cast<const fsa_state_set&>(*this).dfa_state());
}

inline bool fsa_state_set::marked() const
{
    return my_marked;
}

inline bool& fsa_state_set::marked()
{
    return my_marked;
}

inline bool operator != (const fsa_state_set& lhs, const fsa_state_set& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE

#endif // __FSA_STATE_SET_H__
