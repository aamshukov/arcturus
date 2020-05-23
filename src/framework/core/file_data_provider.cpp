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
#include <core\file_data_provider.hpp>

BEGIN_NAMESPACE(core)

file_data_provider::file_data_provider(const string_type& file_name,
                                       typename file_data_provider::callback_type callback)
                  : my_file_name(file_name),
                    my_callback(callback)
{
}

file_data_provider::~file_data_provider()
{
}

size_type file_data_provider::get_file_size(const string_type& file_name)
{
    size_type result = 0;

    FILE* file;

    if(_wfopen_s(&file, file_name.c_str(), L"rb") == 0)
    {
        fseek(file, 0, SEEK_END);
        result = ftell(file);
        fclose(file);
    }

    return result;
}

string_type file_data_provider::get_encoding(const string_type& file_name)
{
    log_info(L"Detecting encoding ...");

    string_type result;

    FILE* file;

    if(_wfopen_s(&file, file_name.c_str(), L"rb") == 0)
    {
        byte bom[4];

        bom[0] = bom[1] = bom[2] = bom[3] = 0x05;

        std::size_t read_bytes = fread(reinterpret_cast<byte*>(&bom), 1, sizeof(bom), file);

        fclose(file);

        if(read_bytes >= 3 && bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)
        {
            result.assign(UTF8_ENCODING);
        }
        else if(read_bytes >= 2 && (bom[0] == 0xFE && bom[1] == 0xFF) ||
                                   (bom[0] == 0xFF && bom[1] == 0xFE))
        {
            if(bom[0] == 0xFE && bom[1] == 0xFF)
            {
                result.assign(UTF16_BE_ENCODING);
            }
            else
            {
                result.assign(UTF16_LE_ENCODING);
            }
        }
        else if(read_bytes >= 4 && (bom[0] == 0x00 && bom[1] == 0x00 && bom[2] == 0xFE && bom[3] == 0xFF) ||
                                   (bom[0] == 0xFF && bom[1] == 0xFE && bom[2] == 0x00 && bom[3] == 0x00))
        {
            if(bom[0] == 0x00 && bom[1] == 0x00 && bom[2] == 0xFE && bom[3] == 0xFF)
            {
                result.assign(UTF32_BE_ENCODING);
            }
            else
            {
                result.assign(UTF32_LE_ENCODING);
            }
        }
        else if(read_bytes >= 3 && (bom[0] == 0x2B && bom[1] == 0x2F && bom[2] == 0x76) ||
                                   (bom[0] == 0x76 && bom[1] == 0x2F && bom[2] == 0x2B))
        {
            result.assign(UTF7_ENCODING);
        }
        else if(read_bytes >= 3 && (bom[0] == 0xF7 && bom[1] == 0x64 && bom[2] == 0x4C) ||
                                   (bom[0] == 0x4C && bom[1] == 0x64 && bom[2] == 0xF7))
        {
            result.assign(UTF1_ENCODING);
        }
        else if(read_bytes >= 4 && (bom[0] == 0xDD && bom[1] == 0x73 && bom[2] == 0x66 && bom[3] == 0x73) ||
                                   (bom[0] == 0x73 && bom[1] == 0x66 && bom[2] == 0x73 && bom[3] == 0xDD))
        {
            result.assign(UTF_EBCDIC_ENCODING);
        }
        else if(read_bytes >= 3 && (bom[0] == 0x0E && bom[1] == 0xFE && bom[2] == 0xFF) ||
                                   (bom[0] == 0xFF && bom[1] == 0xFE && bom[2] == 0x0E))
        {
            result.assign(SDSU_ENCODING);
        }
        else if(read_bytes >= 3 && (bom[0] == 0xFB && bom[1] == 0xEE && bom[2] == 0x28) ||
                                   (bom[0] == 0x28 && bom[1] == 0xEE && bom[2] == 0xFB))
        {
            result.assign(BOCU1_ENCODING);
        }
        else if(read_bytes >= 4 && (bom[0] == 0x84 && bom[1] == 0x31 && bom[2] == 0x95 && bom[3] == 0x33) ||
                                   (bom[0] == 0x33 && bom[1] == 0x95 && bom[2] == 0x31 && bom[3] == 0x84))
        {
            result.assign(GB18030_ENCODING);
        }
        else
        {
            result.assign(DEFAULT_ENCODING);
        }
    }

    log_info(L"Detected encoding as '%s'.", result.c_str());

    return result;
}

bool file_data_provider::read_raw_data(const string_type& file_name, std::shared_ptr<byte[]>& data, size_type& count, offset_type offset, operation_status& status)
{
    log_info(L"Reading raw content ...");

    count = 0;

    bool result = false;

    try
    {
        FILE* file;

        if(_wfopen_s(&file, file_name.c_str(), L"rb") == 0)
        {
            fseek(file, 0, SEEK_END);

            size_type size = ftell(file);

            std::rewind(file);

            std::shared_ptr<byte[]> buffer(new byte[size + 1]);

            fseek(file, static_cast<long>(offset), SEEK_SET);
            fread(buffer.get(), 1, size, file);

            fclose(file);

            data.swap(buffer);
            count = size - offset;

            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Read raw data: error occurred.")
    }

    log_info(L"Read raw content.");

    return result;
}

bool file_data_provider::read_utf8_data(std::shared_ptr<byte[]> raw_data, size_type raw_count, std::shared_ptr<datum_type[]>& data, size_type& count, operation_status& status)
{
    log_info(L"Reading UTF-8 content ...");

    count = 0;

    bool result = false;

    try
    {
        const byte*  source_start_aux(raw_data.get());
        const byte** source_start(&source_start_aux);
        const byte*  source_end(source_start_aux + raw_count);

        std::shared_ptr<datum_type[]> buffer(new datum_type[raw_count + 1]);

        const datum_type*  target_start_aux(buffer.get());
        const datum_type** target_start(&target_start_aux);
        const datum_type*  target_end(target_start_aux + raw_count);
            
        convert_result cr = convert_utf8_to_utf32(source_start, source_end, (UTF32**)target_start, (UTF32*)target_end, conversion_flags::strict_conversion, count);

        if(cr == conversion_ok)
        {
            buffer[count] = 0;

            data.swap(buffer);

            result = true;
        }
        else
        {
            if(cr == source_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error, L"Read UTF16 data: partial character in source, but hit end.")
            }
            else if(cr == target_exhausted)
            {
                OPERATION_FAILED(status::custom_code::error, L"Read UTF16 data: insufficient room in target for conversion.")
            }
            else if(cr == source_illegal)
            {
                OPERATION_FAILED(status::custom_code::error, L"Read UTF16 data: source sequence is illegal/malformed.")
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Read UTF8 data: error occurred.")
    }

    log_info(L"Read UTF-8 content.");

    return result;
}

bool file_data_provider::read_utf16_data(std::shared_ptr<byte[]> raw_data, size_type raw_count, std::shared_ptr<datum_type[]>& data, size_type& count, bool big_endian, operation_status& status)
{
    log_info(L"Reading UTF-16 content ...");

    count = 0;

    bool result = false;

    try
    {
        if(raw_count % 2 != 0)
        {
            OPERATION_FAILED(status::custom_code::error, L"Read UTF16 data: invalid input, must be divisible by 2.")
        }
        else
        {
            raw_count /= 2;

            const uint16_t*  source_start_aux(reinterpret_cast<uint16_t*>(raw_data.get()));
            const uint16_t** source_start(&source_start_aux);
            const uint16_t*  source_end(source_start_aux + raw_count);

            std::shared_ptr<datum_type[]> buffer(new datum_type[raw_count + 1]);

            const datum_type*  target_start_aux(buffer.get());
            const datum_type** target_start(&target_start_aux);
            const datum_type*  target_end(target_start_aux + raw_count);

            convert_result cr = convert_utf16_to_utf32(source_start,
                                                       source_end,
                                                       (UTF32**)target_start,
                                                       (UTF32*)target_end,
                                                       conversion_flags::strict_conversion, count,
                                                       (is_little_endian() && big_endian) || (is_big_endian() && !big_endian));
            if(cr == conversion_ok)
            {
                buffer[count] = 0;

                data.swap(buffer);

                result = true;
            }
            else
            {
                if(cr == source_exhausted)
                {
                    OPERATION_FAILED(status::custom_code::error, L"Read UTF16 data: partial character in source, but hit end.")
                }
                else if(cr == target_exhausted)
                {
                    OPERATION_FAILED(status::custom_code::error, L"Read UTF16 data: insufficient room in target for conversion.")
                }
                else if(cr == source_illegal)
                {
                    OPERATION_FAILED(status::custom_code::error, L"Read UTF16 data: source sequence is illegal/malformed.")
                }
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Read UTF16 data: error occurred.")
    }

    log_info(L"Read UTF-16 content.");

    return result;
}

bool file_data_provider::read_utf32_data(std::shared_ptr<byte[]> raw_data, size_type raw_count, std::shared_ptr<datum_type[]>& data, size_type& count, bool big_endian, operation_status& status)
{
    log_info(L"Reading UTF-32 content ...");

    count = 0;

    bool result = false;

    try
    {
        if(raw_count % 4 != 0)
        {
            OPERATION_FAILED(status::custom_code::error, L"Read UTF32 data: invalid input, must be divisible by 4.")
        }
        else
        {
            count = raw_count / 4;

            std::shared_ptr<datum_type[]> buffer(new datum_type[count + 1]);

            for(size_type i = 0, k = 0; k < raw_count; k += sizeof(uint32_t))
            {
                uint32_t v =  (raw_data[k + 0] << 24) & 0xFF000000;
                         v |= (raw_data[k + 1] << 16) & 0x00FF0000;
                         v |= (raw_data[k + 2] <<  8) & 0x0000FF00;
                         v |= (raw_data[k + 3] <<  0) & 0x000000FF;

                if((is_little_endian() && big_endian) || (is_big_endian() && !big_endian))
                {
                    v = byte_swap_uint32(v);
                }

                buffer[i++] = v;
            }

            buffer[count] = 0;

            data.swap(buffer);

            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Read UTF32 data: error occurred.")
    }

    log_info(L"Read UTF-32 content.");

    return result;
}

bool file_data_provider::load0(std::shared_ptr<file_data_provider::datum_type[]>& data, size_type& count, operation_status& status)
{
    log_info(L"Loading content ...");

    count = 0;

    bool result = false;

    try
    {
        string_type encoding(get_encoding(my_file_name));

        std::shared_ptr<byte[]> raw_data;

        size_type raw_count;

        if(encoding == UTF8_ENCODING)
        {
            if(read_raw_data(my_file_name, raw_data, raw_count, 3, status))
            {
                read_utf8_data(raw_data, raw_count, data, count, status);
            }
        }
        else if(encoding == UTF16_BE_ENCODING || encoding == UTF16_LE_ENCODING)
        {
            if(read_raw_data(my_file_name, raw_data, raw_count, 2, status))
            {
                read_utf16_data(raw_data, raw_count, data, count, encoding == UTF16_BE_ENCODING, status);
            }
        }
        else if(encoding == UTF32_BE_ENCODING || encoding == UTF32_LE_ENCODING)
        {
            if(read_raw_data(my_file_name, raw_data, raw_count, 4, status))
            {
                read_utf32_data(raw_data, raw_count, data, count, encoding == UTF32_BE_ENCODING, status);
            }
        }
        else
        {
            if(read_raw_data(my_file_name, raw_data, raw_count, 0, status))
            {
                read_utf8_data(raw_data, raw_count, data, count, status);
            }
        }

        result = true;

        log_info(L"Loaded content.");
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Load data: error occurred.")
    }

    return result;
}

bool file_data_provider::load(std::shared_ptr<typename file_data_provider::datum_type[]>& data, size_type& count, operation_status& status)
{
    log_info(L"Loading content ...");

    count = 0;

    bool result = false;

    try
    {
        size_type file_size = get_file_size(my_file_name);

        string_type encoding(get_encoding(my_file_name));

        std::string icu_encoding;

        if(encoding == UTF8_ENCODING)
        {
            icu_encoding = "UTF-8";
        }
        else if(encoding == UTF16_BE_ENCODING || encoding == UTF16_LE_ENCODING)
        {
            if(file_size % 2 != 0)
            {
                OPERATION_FAILED(status::custom_code::error, L"Read UTF16 data: invalid input, must be divisible by 2.")
            }

            icu_encoding = "UTF-16";
            file_size /= 2;
        }
        else if(encoding == UTF32_BE_ENCODING || encoding == UTF32_LE_ENCODING)
        {
            if(file_size % 4 != 0)
            {
                OPERATION_FAILED(status::custom_code::error, L"Read UTF32 data: invalid input, must be divisible by 4.")
            }

            icu_encoding = "UTF-32";
            file_size /= 4;
        }
        else
        {
            log_warning(L"Loading content: unknown encoding.");
            icu_encoding = "UTF-8";
        }

        UFILE* ufile(u_fopen_u(reinterpret_cast<const UChar*>(my_file_name.c_str()), "rb", NULL, icu_encoding.c_str()));

        if(ufile != nullptr)
        {
            std::shared_ptr<datum_type[]> buffer(new datum_type[file_size + 1]);

            memset(buffer.get(), 0, file_size + 1);

            datum_type ch = u_fgetcx(ufile);

            size_type k = 0;

            const byte* bom(reinterpret_cast<const byte*>(&ch));

            if(!((bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF) ||
                ((bom[0] == 0xFE && bom[1] == 0xFF) || (bom[0] == 0xFF && bom[1] == 0xFE)) ||
                ((bom[0] == 0x00 && bom[1] == 0x00 && bom[2] == 0xFE && bom[3] == 0xFF)) || (bom[0] == 0xFF && bom[1] == 0xFE && bom[2] == 0x00 && bom[3] == 0x00)))
            {
                u_fungetc(ch, ufile);
            }

            if(my_callback)
            {
                result = my_callback(buffer, ufile, k);
            }
            else
            {
                while(!u_feof(ufile))
                {
                    buffer[k++] = u_fgetcx(ufile);
                }

                result = true;
            }

            u_fclose(ufile);

            if(result)
            {
                count = k;
                buffer[count] = 0;
                data.swap(buffer);
            }
        }

        log_info(L"Loaded content.");
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex, status::custom_code::error, L"Load data: error occurred.")
    }

    return result;
}

END_NAMESPACE
