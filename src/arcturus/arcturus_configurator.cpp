//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#include <core/pch.hpp>
#include <arcturus_configurator.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)

void arcturus_configurator::populate_options()
{
    my_master_options.insert(std::pair(L"input-file-name", std::tuple(empty_string(), false, false)));

    my_master_options.insert(std::pair(L"output-file-name", std::tuple(empty_string(), false, false)));
    my_master_options.insert(std::pair(L"output-log-file-name", std::tuple(empty_string(), false, false)));

    my_master_options.insert(std::pair(L"output-path", std::tuple(empty_string(), false, false)));

    my_master_options.insert(std::pair(L"library-path", std::tuple(empty_string(), true, false)));

    my_master_options.insert(std::pair(L"build-type", std::tuple(L"release", false, true)));
    my_master_options.insert(std::pair(L"build-type", std::tuple(L"debug", false, true)));
}

void arcturus_configurator::populate_flags()
{
    my_master_flags.insert(std::pair(L"debug", false));
    my_master_flags.insert(std::pair(L"d", false));
}

END_NAMESPACE
