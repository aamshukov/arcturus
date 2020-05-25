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

#include <content/data_provider.hpp>
#include <content/file_data_provider.hpp>

#include <content/content.hpp>

BEGIN_NAMESPACE(core)

content::content(const id_type& id, const source_type& source)
       : my_id(id), my_source(source), my_count(0)
{
}

content::~content()
{
}

bool content::load(data_provider& provider)
{
    log_info(L"Loading content ...");

    bool result = provider.load(my_data, my_count);

    log_info(L"Loaded content.");

    return result;
}

END_NAMESPACE
