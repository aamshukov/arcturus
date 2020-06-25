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

#include <core/configurator.hpp>

#include <arcturus_configurator.hpp>

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)

void arcturus_configurator::populate_options()
{
    my_master_options[L"color"] = L"red";
    my_master_options[L"c"] = L"red";
}

void arcturus_configurator::populate_flags()
{
    my_master_flags[L"debug"] = false;
    my_master_flags[L"d"] = false;
}

END_NAMESPACE
