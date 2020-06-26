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

#include <core/unicode.hpp>
#include <core/text.hpp>

#include <core/configurator.hpp>

#include <arcturus_configurator.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)

void arcturus_configurator::populate_options()
{
    my_master_options.insert(std::pair(L"input-file-name", empty_string()));

    my_master_options.insert(std::pair(L"output-file-name", empty_string()));
    my_master_options.insert(std::pair(L"output-log-file-name", empty_string()));

    my_master_options.insert(std::pair(L"output-path", empty_string()));

    my_master_options.insert(std::pair(L"library-path", empty_string()));

    my_master_options.insert(std::pair(L"build-type", empty_string())); // release | debug
}

void arcturus_configurator::populate_flags()
{
    my_master_flags.insert(std::pair(L"flag", false));
    my_master_flags.insert(std::pair(L"f", false));
}

END_NAMESPACE
