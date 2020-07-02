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

#include <frontend/fsa/fsa_transition.hpp>
#include <frontend/fsa/fsa_state.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

fsa_state::fsa_state()
         : fsa_state(empty_string(), 0)
{
}

fsa_state::fsa_state(const string_type& label, typename fsa_state::token_type token)
         : my_id(0), my_label(label), my_token(token), my_marked(false)
{
}

fsa_state::fsa_state(const fsa_state& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_label = other.my_label;

        my_token = other.my_token;

        my_transitions = other.clone_transitions();

        my_marked = other.my_marked;
    }
}

fsa_state::fsa_state(fsa_state&& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_label = std::move(other.my_label);

        my_token = other.my_token;

        my_transitions = std::move(other.my_transitions);

        my_marked = other.my_marked;
    }
}

fsa_state& fsa_state::operator = (const fsa_state& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_label = other.my_label;

        my_token = other.my_token;

        my_transitions = other.clone_transitions();

        my_marked = other.my_marked;
    }

    return *this;
}

fsa_state& fsa_state::operator = (fsa_state&& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_label = std::move(other.my_label);

        my_token = other.my_token;

        my_transitions = std::move(other.my_transitions);

        my_marked = other.my_marked;
    }

    return *this;
}

typename fsa_state::transition_type fsa_state::match_transition(const typename fsa_state::transition_type& transition0) const
{
    transition_type result;

    for(const auto& transition_kvp : my_transitions)
    {
        const auto& transition(transition_kvp.second);

        if((*transition).predicate().compare((*transition0).predicate()) == 0)
        {
            result = transition;
            break;
        }
    }
 
    return result;
}

typename fsa_state::state_type fsa_state::clone() const
{
    state_type result(factory::create<fsa_state>());

    *result = *this;

    return result;
}

typename fsa_state::transitions_type fsa_state::clone_transitions() const
{
    transitions_type result;

    for(const auto& transition : my_transitions)
    {
        auto new_transition(factory::create<fsa_transition>());

        *new_transition = *transition.second;

        result.emplace(transitions_type::value_type((*new_transition).id(), new_transition));
    }

    return result;
}

END_NAMESPACE
