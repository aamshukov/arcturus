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
#include <content/string_data_provider.hpp>

BEGIN_NAMESPACE(core)

string_data_provider::string_data_provider(const string_type& data_content) : my_data_content(data_content)
{
}

string_data_provider::~string_data_provider()
{
}

bool string_data_provider::load(std::shared_ptr<datum_type[]>& data, size_type& count)
{
    log_info(L"Loading string content ...");

    count = 0;

    bool result = text::string_to_codepoints(my_data_content, data, count);

    log_info(L"Loaded string content.");

    return result;
}

END_NAMESPACE
