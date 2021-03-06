//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <frontend/fsa/fsa_transition.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

fsa_transition::fsa_transition()
              : fsa_transition(0, 0, empty_predicate())
{
}

fsa_transition::fsa_transition(std::size_t start_state, std::size_t end_state, const typename fsa_transition::predicate_type& predicate)
              : my_id(0), my_start_state(start_state), my_end_state(end_state), my_predicate(predicate), my_switch_predicate(text::bad_codepoint()), my_rank(0)
{
}

fsa_transition::fsa_transition(std::size_t start_state, std::size_t end_state, datum_type switch_predicate)
              : my_id(0), my_start_state(start_state), my_end_state(end_state), my_switch_predicate(switch_predicate), my_rank(0)
{
}

fsa_transition::fsa_transition(const fsa_transition& other)
{
    if(this != &other)
    {
        my_id = other.my_id;

        my_start_state = other.my_start_state;
        my_end_state = other.my_end_state;

        my_predicate = other.my_predicate;
        my_switch_predicate = other.my_switch_predicate;

        my_rank = other.my_rank;
    }
}

fsa_transition::fsa_transition(fsa_transition&& other)
{
    if(this != &other)
    {
        my_id = other.my_id;

        my_start_state = other.my_start_state;
        my_end_state = other.my_end_state;

        my_predicate = std::move(other.my_predicate);
        my_switch_predicate = other.my_switch_predicate;

        my_rank = other.my_rank;
    }
}

fsa_transition& fsa_transition::operator = (const fsa_transition& other)
{
    if(this != &other)
    {
        my_id = other.my_id;

        my_start_state = other.my_start_state;
        my_end_state = other.my_end_state;

        my_predicate = other.my_predicate;
        my_switch_predicate = other.my_switch_predicate;

        my_rank = other.my_rank;
    }

    return *this;
}

fsa_transition& fsa_transition::operator = (fsa_transition&& other)
{
    if(this != &other)
    {
        my_id = other.my_id;

        my_start_state = other.my_start_state;
        my_end_state = other.my_end_state;

        my_predicate = std::move(other.my_predicate);
        my_switch_predicate = other.my_switch_predicate;

        my_rank = other.my_rank;
    }

    return *this;
}

END_NAMESPACE
