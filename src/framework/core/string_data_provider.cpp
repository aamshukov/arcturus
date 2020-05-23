//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\unicode.hpp>
#include <core\text.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\data_provider.hpp>
#include <core\string_data_provider.hpp>

BEGIN_NAMESPACE(core)

string_data_provider::string_data_provider(const string_type& data_content) : my_data_content(data_content)
{
}

string_data_provider::~string_data_provider()
{
}

bool string_data_provider::load(std::shared_ptr<typename string_data_provider::datum_type[]>& data, size_type& count, operation_status& status)
{
    log_info(L"Loading string content ...");

    count = 0;

    bool result = text::string_to_codepoints(my_data_content, data, count, status);

    log_info(L"Loaded string content.");

    return result;
}

END_NAMESPACE
