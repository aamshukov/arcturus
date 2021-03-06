//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

BEGIN_NAMESPACE(core)

bool text::chars_to_codepoints(const char* chars, const std::size_t& chars_count, std::shared_ptr<datum_type[]>& codepoints, std::size_t& codepoints_count)
{
    codepoints_count = 0;

    bool result = false;

    try
    {
        const UTF8*  source_start_aux(reinterpret_cast<const UTF8*>(chars));
        const UTF8** source_start(&source_start_aux);
        const UTF8*  source_end(source_start_aux + chars_count);

        std::shared_ptr<datum_type[]> buffer(new datum_type[chars_count + 1]);

        const datum_type*  target_start_org(buffer.get());
        const datum_type*  target_start_aux(buffer.get());
        const datum_type** target_start(&target_start_aux);
        const datum_type*  target_end(target_start_aux + chars_count);

        convert_result cr = convert_utf8_to_utf32(source_start,
                                                  source_end,
                                                  (UTF32**)target_start,
                                                  (UTF32*)target_end,
                                                  conversion_flags::strict_conversion);
        if(cr == conversion_ok)
        {
            codepoints_count = std::ptrdiff_t(*target_start - target_start_org);

            buffer[codepoints_count] = 0;

            codepoints.swap(buffer);

            result = true;
        }
        else
        {
            if(cr == source_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_exhausted,
                                 status::contributor::core,
                                 L"Converting chars (utf8) to codepoints: partial character in source, but hit end.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == target_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 target_exhausted,
                                 status::contributor::core,
                                 L"Converting chars (utf8) to codepoints: insufficient room in target for conversion.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == source_illegal)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_illegal,
                                 status::contributor::core,
                                 L"Converting chars (utf8) to codepoints: source sequence is illegal/malformed.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::core,
                            L"Converting chars (utf8) to codepoints: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

codepoints_type text::chars_to_codepoints(const char* chars, const std::size_t& chars_count)
{
    codepoints_type codepoints;

    try
    {
        const UTF8*  source_start_aux(reinterpret_cast<const UTF8*>(chars));
        const UTF8** source_start(&source_start_aux);
        const UTF8*  source_end(source_start_aux + chars_count);

        std::shared_ptr<datum_type[]> buffer(new datum_type[chars_count + 1]);

        const datum_type*  target_start_org(buffer.get());
        const datum_type*  target_start_aux(buffer.get());
        const datum_type** target_start(&target_start_aux);
        const datum_type*  target_end(target_start_aux + chars_count);

        convert_result cr = convert_utf8_to_utf32(source_start,
                                                  source_end,
                                                  (UTF32**)target_start,
                                                  (UTF32*)target_end,
                                                  conversion_flags::strict_conversion);
        if(cr == conversion_ok)
        {
            auto codepoints_count = std::ptrdiff_t(*target_start - target_start_org);

            buffer[codepoints_count] = 0;

            codepoints = buffer.get();
        }
        else
        {
            bool result = true;

            if(cr == source_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_exhausted,
                                 status::contributor::core,
                                 L"Converting chars (utf8) to codepoints: partial character in source, but hit end.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == target_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 target_exhausted,
                                 status::contributor::core,
                                 L"Converting chars (utf8) to codepoints: insufficient room in target for conversion.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == source_illegal)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_illegal,
                                 status::contributor::core,
                                 L"Converting chars (utf8) to codepoints: source sequence is illegal/malformed.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
        }
    }
    catch(const std::exception& ex)
    {
        bool result = false;

        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::core,
                            L"Converting chars (utf8) to codepoints: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return codepoints;
}

bool text::codepoints_to_chars(const datum_type* codepoints, const std::size_t& codepoints_count, std::shared_ptr<char[]>& chars, std::size_t& chars_count)
{
    chars_count = 0;

    bool result = false;

    try
    {
        const uint32_t*  source_start_aux(codepoints);
        const uint32_t** source_start(&source_start_aux);
        const uint32_t*  source_end(source_start_aux + codepoints_count);

        std::size_t count = (codepoints_count + 1) * 6; // UTF-8 is a multibyte encoding able to encode the whole Unicode charset.
                                                        // An encoded character takes between 1 and 4 bytes.
                                                        // UTF-8 encoding supports longer byte sequences, up to 6 bytes,
                                                        // but the biggest code point of Unicode 6.0 (U+10FFFF) only takes 4 bytes.

        std::shared_ptr<char[]> buffer(new char[count]);

        const char*  target_start_org(buffer.get());
        const char*  target_start_aux(buffer.get());
        const char** target_start(&target_start_aux);
        const char*  target_end(target_start_aux + count);

        convert_result cr = convert_utf32_to_utf8(source_start,
                                                  source_end,
                                                  (UTF8**)target_start,
                                                  (UTF8*)target_end,
                                                  conversion_flags::strict_conversion);
        if(cr == conversion_ok)
        {
            chars_count = std::ptrdiff_t(*target_start - target_start_org);

            buffer[chars_count] = 0;

            chars.swap(buffer);

            result = true;
        }
        else
        {
            if(cr == source_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_exhausted,
                                 status::contributor::core,
                                 L"Converting codepoints to chars (utf8): partial character in source, but hit end.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == target_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 target_exhausted,
                                 status::contributor::core,
                                 L"Converting codepoints to chars (utf8): insufficient room in target for conversion.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == source_illegal)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_illegal,
                                 status::contributor::core,
                                 L"Converting codepoints to chars (utf8): source sequence is illegal/malformed.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::core,
                            L"Converting codepoints to chars (utf8): error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool text::string_to_codepoints0(const string_type& text, std::shared_ptr<datum_type[]>& codepoints, std::size_t& count)
{
    count = 0;

    bool result = false;

    try
    {
        std::size_t text_count = text.size();

        const uint16_t*  source_start_aux(reinterpret_cast<const uint16_t*>(text.c_str()));
        const uint16_t** source_start(&source_start_aux);
        const uint16_t*  source_end(source_start_aux + text_count);

        std::shared_ptr<datum_type[]> buffer(new datum_type[text_count + 1]);

        const datum_type*  target_start_org(buffer.get());
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
            count = std::ptrdiff_t(*target_start - target_start_org);

            buffer[count] = 0;

            codepoints.swap(buffer);

            result = true;
        }
        else
        {
            if(cr == source_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_exhausted,
                                 status::contributor::core,
                                 L"Converting string (text) to codepoints: partial character in source, but hit end.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == target_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 target_exhausted,
                                 status::contributor::core,
                                 L"Converting string (text) to codepoints: insufficient room in target for conversion.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == source_illegal)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_illegal,
                                 status::contributor::core,
                                 L"Converting string (text) to codepoints: source sequence is illegal/malformed.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::core,
                            L"Converting string (text) to codepoints: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool text::codepoints_to_string0(const datum_type* codepoints, std::size_t count, string_type& result_text)
{
    bool result = false;

    try
    {
        const uint32_t*  source_start_aux(codepoints);
        const uint32_t** source_start(&source_start_aux);
        const uint32_t*  source_end(source_start_aux + count);

        std::size_t string_count = (count + 1) * 2;

        std::shared_ptr<uint16_t[]> buffer(new uint16_t[string_count]);

        const uint16_t*  target_start_org(buffer.get());
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
            count = std::ptrdiff_t(*target_start - target_start_org);

            buffer[count] = 0;

            result_text.assign(reinterpret_cast<char_type*>(buffer.get()));

            result = true;
        }
        else
        {
            if(cr == source_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_exhausted,
                                 status::contributor::core,
                                 L"Converting codepoints to string: partial character in source, but hit end.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == target_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 target_exhausted,
                                 status::contributor::core,
                                 L"Converting codepoints to string: insufficient room in target for conversion.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
            else if(cr == source_illegal)
            {
                OPERATION_FAILED(status::custom_code::error,
                                 source_illegal,
                                 status::contributor::core,
                                 L"Converting codepoints to string: source sequence is illegal/malformed.")
                log_error(diagnostics::instance().last_status().text().c_str());
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::core,
                            L"Converting codepoints to string: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool text::string_to_codepoints(const string_type& text, std::shared_ptr<datum_type[]>& codepoints, std::size_t& count)
{
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
                OPERATION_FAILED(status::custom_code::error,
                                 0,
                                 status::contributor::core,
                                 format(L"Converting string (text) to codepoints: ICU error code is '%d'.", error).c_str())
                log_error(diagnostics::instance().last_status().text().c_str());
            }
        }
        else
        {
            OPERATION_FAILED(status::custom_code::error,
                             0,
                             status::contributor::core,
                             L"Converting string (text) to codepoints: invalid input.")
            log_error(diagnostics::instance().last_status().text().c_str());
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::core,
                            L"Converting string (text) to codepoints: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool text::codepoints_to_string(const datum_type* codepoints, std::size_t count, string_type& result_text)
{
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
            OPERATION_FAILED(status::custom_code::error,
                             0,
                             status::contributor::core,
                             L"Converting codepoints to string: invalid input.")
            log_error(diagnostics::instance().last_status().text().c_str());
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::core,
                            L"Converting codepoints to string: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

string_type text::codepoints_to_string(const datum_type* codepoints, std::size_t count)
{
    string_type result;

    codepoints_to_string(codepoints, count, result);

    return result;
}

string_type text::codepoint_to_string(datum_type codepoint)
{
    string_type result;

    if(codepoint != text::invalid_codepoint)
    {
        std::unique_ptr<datum_type[]> codepoints(new datum_type[1 + 1]);

        codepoints[0] = codepoint;

        text::codepoints_to_string(codepoints.get(), 1, result);
    }
    else
    {
        result = L"INVALID_CODEPOINT";
    }

    return result;
}

bool text::codepoint_to_string(datum_type codepoint, string_type& result_text)
{
    bool result = false;

    if(codepoint != text::invalid_codepoint)
    {
        std::unique_ptr<datum_type[]> codepoints(new datum_type[1 + 1]);

        codepoints[0] = codepoint;

        text::codepoints_to_string(codepoints.get(), 1, result_text);
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
    std::size_t right_offset = text.size() - 1;

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

    std::size_t offset = 0;

    while(data[offset] != 0)
    {
        while(data[offset] != 0 && (delimiters.find(data[offset]) != string_type::npos)) // skip delimiters
        {
            offset++;
        }

        std::size_t entry_start = offset;
        std::size_t entry_length = 0;
        
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
