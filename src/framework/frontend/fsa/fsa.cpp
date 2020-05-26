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

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

fsa::fsa()
{
}

fsa::~fsa()
{
}

typename fsa::fsa_type fsa::clone()
{
    auto result(factory::create<fsa>());

    for(const auto& kvp : my_states)
    {
        auto new_state((*kvp.second).clone());

        (*result).add_state(new_state);
    }

    (*result).start_state() = my_states[(*my_start_state).id()];

    for(const auto& kvp : my_final_states)
    {
        const auto& final_state(kvp.second);

        (*result).add_final_state((*result).states()[(*final_state).id()]);
    }

    (*result).my_state_counter.reset(my_state_counter.value());
    (*result).my_transition_counter.reset(my_transition_counter.value());

    return result;
}

bool fsa::add_state(const state_type& state)
{
    log_info(L"Adding FSA state ...");

    bool result = false;

    try
    {
        (*state).id() = my_state_counter.number();

        auto rc = states().emplace(states_type::value_type((*state).id(), state));

        if(rc.second)
        {
            result = true;
        }
        else
        {
            OPERATION_FAILED(status::custom_code::error,
                             0,
                             status::contributer::fsa,
                             L"Adding FSA state: state '%s' already exists.", (*state).label().c_str())
            log_error(diagnostics::instance().last_status().text().c_str());
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::fsa,
                            L"Adding FSA state: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Added FSA state '%s'.", (*state).label().c_str());

    return result;
}

bool fsa::remove_state(uint32_t id)
{
    log_info(L"Removing FSA state '%ld' ...", id);

    bool result = false;

    try
    {
        auto remove_state(my_states[id]);

        my_states.erase(id);

        for(const auto& state_kvp : my_states)
        {
            const auto& state(state_kvp.second);

            for(auto it = (*state).transitions().begin(); it != (*state).transitions().end();)
            {
                const auto& transition((*it).second);

                if((*transition).end_state() == (*remove_state).id())
                {
                    it = (*state).transitions().erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        my_final_states.erase((*remove_state).id());

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::fsa,
                            L"Removed FSA state: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Removed FSA state '%ld'.", id);

    return result;
}

bool fsa::add_final_state(const state_type& state)
{
    log_info(L"Adding final FSA state ...");

    bool result = false;

    try
    {
        auto rc = final_states().emplace(states_type::value_type((*state).id(), state));

        if(rc.second)
        {
            result = true;
        }
        else
        {
            //??OPERATION_FAILED(status::custom_code::error, L"Adding final FSA state: state '%s' already exists.", (*state).label().c_str())
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::fsa,
                            L"Adding final FSA state: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Added final FSA state '%s'.", (*state).label().c_str());

    return result;
}

bool fsa::add_transition(const typename fsa::state_type& start_state,
                         const typename fsa::state_type& end_state,
                         const typename fsa::predicate_type& predicate)
{
    log_info(L"Adding FSA transition ...");

    bool result = false;

    try
    {
        transition_type transition(factory::create<fsa_transition>((*start_state).id(), (*end_state).id(), predicate));

        (*transition).id() = my_transition_counter.number();

        (*start_state).transitions().emplace(transitions_type::value_type((*transition).id(), transition));

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::fsa,
                            L"Adding FSA transition: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Added FSA transition '%s --> %s'.", (*start_state).label().c_str(), (*end_state).label().c_str());

    return result;
}

bool fsa::add_transition(const state_type& start_state, const state_type& end_state, typename fsa::datum_type predicate)
{
    log_info(L"Adding FSA transition ...");

    bool result = false;

    try
    {
        transition_type transition(factory::create<fsa_transition>((*start_state).id(), (*end_state).id(), predicate));

        (*transition).id() = my_transition_counter.number();

        (*start_state).transitions().emplace(transitions_type::value_type((*transition).id(), transition));

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::fsa,
                            L"Adding FSA transition: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Added FSA transition '%s --> %s'.", (*start_state).label().c_str(), (*end_state).label().c_str());

    return result;
}

bool fsa::combine(const fsa_type& fsa1, const fsa_type& fsa2, fsa_type& result_fsa)
{
    //  ... 15 ----> 16 ...
    //
    //  ... 15 ----> 16 ...
    //
    //     2 ---> 3
    //   e/
    //  1
    //   e\
    //     4 ---> 5
    log_info(L"Combining FSAs ...");

    bool result = false;

    try
    {
        auto fsa0(factory::create<fsa>());

        fsa_type fsas [] = { fsa1, fsa2 };

        using map_type = std::map<uint32_t, uint32_t>;

        map_type map1;
        map_type map2;

        map_type* maps[] = { &map1, &map2 };

        using partition_type = std::vector<fsa::state_type>;

        partition_type partition1;
        partition_type partition2;

        partition_type* partitions[] = { &partition1, &partition2 };

        auto new_start_state(factory::create<fsa_state>(DUMMY_START_STATE_NAME, 0));

        (*fsa0).add_state(new_start_state);
        (*fsa0).start_state() = new_start_state;

        for(size_type k = 0; k < array_size(fsas); k++)
        {
            auto& fsa = fsas[k];
            auto& map = *maps[k];
            auto& partition = *partitions[k];

            for(const auto& kvp : (*fsa).states())
            {
                const auto& org_state(kvp.second);
                auto new_state((*org_state).clone());

                partition.emplace_back(new_state);

                (*fsa0).add_state(new_state);

                map.emplace(map_type::value_type((*org_state).id(), (*new_state).id()));

                if((*fsa).is_final_state(org_state))
                {
                    (*fsa0).add_final_state(new_state);
                }
            }

            for(const auto& state: partition)
            {
                for(const auto& kvp : (*state).transitions())
                {
                    const auto& transition(kvp.second);

                    (*transition).start_state() = map.at((*transition).start_state());
                    (*transition).end_state() = map.at((*transition).end_state());
                }
            }
        }

        uint32_t fsa1_s_id = map1.at((*(*fsa1).start_state()).id());
        uint32_t fsa2_s_id = map2.at((*(*fsa2).start_state()).id());

        (*fsa0).add_transition((*fsa0).start_state(), (*fsa0).states().at(fsa1_s_id), fsa_transition::epsilon_predicate());
        (*fsa0).add_transition((*fsa0).start_state(), (*fsa0).states().at(fsa2_s_id), fsa_transition::epsilon_predicate());

        result_fsa.swap(fsa0);

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::fsa,
                            L"Combining FSAs: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Combined FSAs.");

    return result;
}

bool fsa::combine(const std::vector<fsa_type>& fsas, fsa_type& result_fsa)
{
    log_info(L"Combining FSAs ...");

    bool result = false;

    try
    {
        auto fsa0(factory::create<fsa>());

        using map_type = std::map<uint32_t, uint32_t>;
        using partition_type = std::vector<fsa::state_type>;

        std::vector<map_type> maps(fsas.size());
        std::vector<partition_type> partitions(fsas.size());

        auto new_start_state(factory::create<fsa_state>(DUMMY_START_STATE_NAME, 0));

        (*fsa0).add_state(new_start_state);
        (*fsa0).start_state() = new_start_state;

        for(size_type k = 0; k < fsas.size(); k++)
        {
            auto& fsa = fsas[k];
            auto& map = maps[k];

            auto& partition = partitions[k];

            for(const auto& kvp : (*fsa).states())
            {
                const auto& org_state(kvp.second);
                auto new_state((*org_state).clone());

                partition.emplace_back(new_state);

                (*fsa0).add_state(new_state);

                map.emplace(map_type::value_type((*org_state).id(), (*new_state).id()));

                if((*fsa).is_final_state(org_state))
                {
                    (*fsa0).add_final_state(new_state);
                }
            }

            for(const auto& state: partition)
            {
                for(const auto& kvp : (*state).transitions())
                {
                    const auto& transition(kvp.second);

                    (*transition).start_state() = map.at((*transition).start_state());
                    (*transition).end_state() = map.at((*transition).end_state());
                }
            }

            uint32_t fsa_s_id = map.at((*(*fsa).start_state()).id());

            (*fsa0).add_transition((*fsa0).start_state(), (*fsa0).states().at(fsa_s_id), fsa_transition::epsilon_predicate());
        }

        result_fsa.swap(fsa0);

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::fsa,
                            L"Combining FSAs: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Combined FSAs.");

    return result;
}

bool fsa::concatenate(const fsa_type& fsa1, const fsa_type& fsa2, fsa_type& result_fsa)
{
    //  ... 15 ----> 16 ...
    //
    //  ... 15 ----> 16 ...
    //
    //  1 ----> 2 ---> 3 ----> 4
    //             e
    log_info(L"Concatenating FSAs ...");

    bool result = false;

    try
    {
        auto fsa0(factory::create<fsa>());

        fsa_type fsas [] = { fsa1, fsa2 };

        using map_type = std::map<uint32_t, uint32_t>;

        map_type map1;
        map_type map2;

        map_type* maps[] = { &map1, &map2 };

        using partition_type = std::vector<fsa::state_type>;

        partition_type partition1;
        partition_type partition2;

        partition_type* partitions[] = { &partition1, &partition2 };

        for(std::size_t k = 0; k < array_size(fsas); k++)
        {
            auto& fsa = fsas[k];
            auto& map = *maps[k];
            auto& partition = *partitions[k];

            for(const auto& kvp : (*fsa).states())
            {
                const auto& org_state(kvp.second);
                auto new_state((*org_state).clone());

                partition.emplace_back(new_state);

                (*fsa0).add_state(new_state);

                map.emplace(map_type::value_type((*org_state).id(), (*new_state).id()));

                if(k > 0 && (*fsa).is_final_state(org_state))
                {
                    (*fsa0).add_final_state(new_state);
                }
            }

            for(const auto& state: partition)
            {
                for(const auto& kvp : (*state).transitions())
                {
                    const auto& transition(kvp.second);

                    (*transition).start_state() = map.at((*transition).start_state());
                    (*transition).end_state() = map.at((*transition).end_state());
                }
            }
        }

        (*fsa0).start_state() = (*fsa0).states().at((*(*fsa1).start_state()).id());

        uint32_t fsa1_s_id = map1.at((*(*fsa1).start_state()).id());

        (*fsa0).add_transition((*fsa0).start_state(), (*fsa0).states().at(fsa1_s_id), fsa_transition::epsilon_predicate());

        uint32_t fsa2_s_id = map2.at((*(*fsa1).start_state()).id());

        auto fsa2_start_state((*fsa0).states().at(fsa2_s_id));

        for(const auto& kvp : (*fsa1).final_states())
        {
            uint32_t fsa1_f_id = map1.at((*kvp.second).id());

            const auto& fsa1_final_state((*fsa0).states().at(fsa1_f_id));
            
            (*fsa0).add_transition(fsa1_final_state, fsa2_start_state, fsa_transition::epsilon_predicate());
        }

        result_fsa.swap(fsa0);

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::fsa,
                            L"Concatenating FSAs: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    log_info(L"Concatenated FSAs.");

    return result;
}

typename fsa::predicates_type fsa::predicates() const
{
    predicates_type result;

    for(const auto& kvp_state : my_states)
    {
        const auto& state(kvp_state.second);

        for(const auto& kvp_transition : (*state).transitions())
        {
            const auto& transition(kvp_transition.second);

            if(!(*transition).is_epsilon_transition())
            {
                result.emplace((*transition).predicate());
            }
        }
    }

    return result;
}

END_NAMESPACE
