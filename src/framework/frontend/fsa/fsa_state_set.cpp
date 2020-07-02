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
#include <frontend/fsa/fsa_state_set.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

fsa_state_set::fsa_state_set() : my_marked(false)
{
}

fsa_state_set::fsa_state_set(const fsa_state_set& other)
{
    if(this != &other)
    {
        my_states = other.my_states;
        my_dfa_state = other.my_dfa_state;

        my_marked = other.my_marked;
    }
}

fsa_state_set::fsa_state_set(fsa_state_set&& other)
{
    if(this != &other)
    {
        my_states = std::move(other.my_states);
        my_dfa_state = std::move(other.my_dfa_state);

        my_marked = other.my_marked;
    }
}

fsa_state_set& fsa_state_set::operator = (const fsa_state_set& other)
{
    if(this != &other)
    {
        my_states = other.my_states;
        my_dfa_state = other.my_dfa_state;

        my_marked = other.my_marked;
    }

    return *this;
}

fsa_state_set& fsa_state_set::operator = (fsa_state_set&& other)
{
    if(this != &other)
    {
        my_states = std::move(other.my_states);
        my_dfa_state = std::move(other.my_dfa_state);

        my_marked = other.my_marked;
    }

    return *this;
}

std::pair<typename fsa_state_set::state_type, bool> fsa_state_set::build_fsa_state()
{
    std::pair<state_type, bool> result(std::make_pair(my_dfa_state, my_dfa_state == nullptr));

    if(result.second)
    {
        result.first = factory::create<fsa_state>();

        (*result.first).label() = decorate();

        my_dfa_state = result.first;
    }

    return result;
}

string_type fsa_state_set::decorate() const
{
    string_type result;

    std::vector<uint32_t> states;

    std::for_each(my_states.begin(), my_states.end(), [&states](const auto& state_kvp) { states.emplace_back(state_kvp.first); });

    std::sort(states.begin(), states.end(), [] (auto id1, auto id2) { return id1 < id2; });

    std::for_each(states.begin(), states.end(), [&states, &result](uint32_t state) { result += L" " + std::to_wstring(state); });

    return result;
}

bool operator == (const fsa_state_set& lhs, const fsa_state_set& rhs)
{
    bool result = lhs.states().size() == rhs.states().size();

    if(result)
    {
        std::vector<std::pair<uint32_t, typename fsa_state_set::state_type>> lhs_vector(lhs.states().begin(), lhs.states().end());

        std::sort(lhs_vector.begin(), lhs_vector.end(), [] (const auto& s1_kvp, const auto& s2_kvp)
                                                        {
                                                            return s1_kvp.first < s2_kvp.first;
                                                        });

        std::vector<std::pair<uint32_t, typename fsa_state_set::state_type>> rhs_vector(rhs.states().begin(), rhs.states().end());

        std::sort(rhs_vector.begin(), rhs_vector.end(), [] (const auto& s1_kvp, const auto& s2_kvp)
                                                        {
                                                            return s1_kvp.first < s2_kvp.first;
                                                        });
        result = std::equal(lhs_vector.begin(),
                            lhs_vector.end(),
                            rhs_vector.begin(),
                            [] (const auto& s1, const auto& s2)
                            {
                                return s1.first == s2.first;
                            });
    }

    return result;
}

END_NAMESPACE
