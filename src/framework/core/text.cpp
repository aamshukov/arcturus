//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\domain_helper.hpp>
#include <core\status.hpp>
#include <core\logger.hpp>
#include <core\unicode.hpp>
#include <core\text.hpp>

BEGIN_NAMESPACE(core)

bool text::string_to_codepoints0(const string_type& text, std::shared_ptr<typename text::datum_type[]>& codepoints, size_type& count, operation_status& status)
{
    //log_info(L"Converting string (text) to codepoints ...");

    count = 0;

    bool result = false;

    try
    {
        std::size_t text_count = text.size();

        const uint16_t*  source_start_aux(reinterpret_cast<const uint16_t*>(text.c_str()));
        const uint16_t** source_start(&source_start_aux);
        const uint16_t*  source_end(source_start_aux + text_count);

        std::shared_ptr<datum_type[]> buffer(new datum_type[text_count + 1]);

        const datum_type*  target_start_aux(buffer.get());
        const datum_type** target_start(&target_start_aux);
        const datum_type*  target_end(target_start_aux + text_count);

        convert_result cr = convert_utf16_to_utf32(source_start,
                                                   source_end,
                                                   (UTF32**)target_start,
                                                   (UTF32*)target_end,
                                                   conversion_flags::strict_conversion,
                                                   count,
                                                   false);
        if(cr == conversion_ok)
        {
            buffer[count] = 0;

            codepoints.swap(buffer);

            result = true;
        }
        else
        {
            if(cr == source_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error, L"Converting string (text) to codepoints: partial character in source, but hit end.")
            }
            else if(cr == target_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error, L"Converting string (text) to codepoints: insufficient room in target for conversion.")
            }
            else if(cr == source_illegal)
            {
                OPERATION_FAILED(status::custom_code::error, L"Converting string (text) to codepoints: source sequence is illegal/malformed.")
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Converting string (text) to codepoints: error occurred.")
    }

    //log_info(L"Converted string (text) to codepoints.");

    return result;
}

bool text::codepoints_to_string0(const typename text::datum_type* codepoints, size_type count, string_type& result_text, operation_status& status)
{
    //??log_info(L"Converting codepoints to string ...");

    bool result = false;

    try
    {
        const uint32_t*  source_start_aux(codepoints);
        const uint32_t** source_start(&source_start_aux);
        const uint32_t*  source_end(source_start_aux + count);

        size_type string_count = (count + 1) * 2;

        std::shared_ptr<uint16_t[]> buffer(new uint16_t[string_count]);

        const uint16_t*  target_start_aux(buffer.get());
        const uint16_t** target_start(&target_start_aux);
        const uint16_t*  target_end(target_start_aux + string_count);

        convert_result cr = convert_utf32_to_utf16(source_start,
                                                   source_end,
                                                   (UTF16**)target_start,
                                                   (UTF16*)target_end,
                                                   conversion_flags::strict_conversion);
        if(cr == conversion_ok)
        {
            buffer[count] = 0;

            result_text.assign(reinterpret_cast<char_type*>(buffer.get()));

            result = true;
        }
        else
        {
            if(cr == source_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error, L"Converting codepoints to string: partial character in source, but hit end.")
            }
            else if(cr == target_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error, L"Converting codepoints to string: insufficient room in target for conversion.")
            }
            else if(cr == source_illegal)
            {
                OPERATION_FAILED(status::custom_code::error, L"Converting codepoints to string: source sequence is illegal/malformed.")
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Converting codepoints to string: error occurred.")
    }

    //??log_info(L"Converted codepoints to string.");

    return result;
}

bool text::string_to_codepoints(const string_type& text, std::shared_ptr<typename text::datum_type[]>& codepoints, size_type& count, operation_status& status)
{
    //log_info(L"Converting string (text) to codepoints ...");

    count = 0;

    bool result = false;

    try
    {
        UnicodeString ustr(text.c_str());

        if(!ustr.isBogus())
        {
            int32_t size = ustr.length() + 1;

            std::shared_ptr<datum_type[]> buffer(new datum_type[size]);

            UErrorCode error = U_ZERO_ERROR;

            int32_t length = ustr.toUTF32(reinterpret_cast<UChar32*>(buffer.get()), static_cast<int32_t>(size), error);

            if(error == U_ZERO_ERROR)
            {
                buffer[length] = 0;

                count = length;

                codepoints.swap(buffer);

                result = true;
            }
            else
            {
                OPERATION_FAILED(status::custom_code::error, format(L"Converting string (text) to codepoints: ICU error code is '%d'.", error).c_str())
            }
        }
        else
        {
            OPERATION_FAILED(status::custom_code::error, L"Converting string (text) to codepoints: invalid input.")
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Converting string (text) to codepoints: error occurred.")
    }

    //log_info(L"Converted string (text) to codepoints.");

    return result;
}

bool text::codepoints_to_string(const typename text::datum_type* codepoints, size_type count, string_type& result_text, operation_status& status)
{
    //log_info(L"Converting codepoints to string ...");

    bool result = false;

    try
    {
        UnicodeString ustr(UnicodeString::fromUTF32(reinterpret_cast<const UChar32*>(codepoints), static_cast<int32_t>(count)));

        if(!ustr.isBogus())
        {
            int32_t size = ustr.length();

            std::unique_ptr<char16_t[]> buffer(new char16_t[size + 1]);

            ustr.extractBetween(0, static_cast<int32_t>(size), buffer.get(), 0);

            buffer.get()[size] = 0;

            result_text.assign(reinterpret_cast<const char_type*>(buffer.get()));

            result = true;
        }
        else
        {
            OPERATION_FAILED(status::custom_code::error, L"Converting codepoints to string: invalid input.")
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Converting codepoints to string: error occurred.")
    }

    //log_info(L"Converted codepoints to string.");

    return result;
}

string_type text::codepoint_to_string(typename text::datum_type codepoint)
{
    string_type result;

    if(codepoint != text::invalid_codepoint)
    {
        std::unique_ptr<datum_type[]> codepoints(new datum_type[1 + 1]);

        codepoints[0] = codepoint;

        operation_status status; //??
        text::codepoints_to_string(codepoints.get(), 1, result, status);
    }
    else
    {
        result = L"INVALID_CODEPOINT";
    }

    return result;
}

bool text::codepoint_to_string(typename text::datum_type codepoint, string_type& result_text, operation_status& status)
{
    bool result = false;

    if(codepoint != text::invalid_codepoint)
    {
        std::unique_ptr<datum_type[]> codepoints(new datum_type[1 + 1]);

        codepoints[0] = codepoint;

        text::codepoints_to_string(codepoints.get(), 1, result_text, status);
    }
    else
    {
        result_text = L"INVALID_CODEPOINT";
    }

    return result;
}

string_type text::trim(const string_type& text, const string_type& delimiters)
{
    string_type result;

    text::trim(text, delimiters, result);

    return result;
}

void text::trim(const string_type& text, const string_type& delimiters, string_type& result_text)
{
    result_text = text;

    const char_type* data(text.c_str());

    // find left offset
    int left_offset = 0;

    while(left_offset < text.size() && (delimiters.find(data[left_offset]) != string_type::npos))
    {
        left_offset++;
    }

    // find right offset
    size_type right_offset = text.size() - 1;

    while(right_offset >= 0 && (delimiters.find(data[right_offset]) != string_type::npos))
    {
        right_offset--;
    }

    // build result
    if(left_offset > 0 || right_offset < text.size() - 1)
    {
        result_text.assign(data, left_offset, right_offset + 1);
    }
}

void text::split(const string_type& text, const string_type& delimiters, std::vector<string_type>& parts)
{
    const char_type* data(text.c_str());

    size_type offset = 0;

    while(data[offset] != 0)
    {
        while(data[offset] != 0 && (delimiters.find(data[offset]) != string_type::npos)) // skip delimiters
        {
            offset++;
        }

        size_type entry_start = offset;
        size_type entry_length = 0;
        
        while(data[offset] != 0 && (delimiters.find(data[offset]) == string_type::npos)) // populate entry
        {
            entry_length++;
            offset++;
        }

        if(entry_length > 0)
        {
            string_type entry(data + entry_start, entry_length);
            parts.emplace_back(entry);
        }
    }
}

bool text::starts_with(const string_type& text, const char_type* prefix)
{
    return text.substr(0, wcslen(prefix)).compare(prefix) == 0;
}

bool text::starts_with(const string_type& text, const string_type& prefix)
{
    return text.substr(0, prefix.size()).compare(prefix) == 0;
}

END_NAMESPACE
