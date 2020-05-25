//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FILE_DATA_PROVIDER_H__
#define __FILE_DATA_PROVIDER_H__

#pragma once

#define UTF8_ENCODING       L"UTF-8"
#define UTF16_LE_ENCODING   L"UTF-16-LE"
#define UTF16_BE_ENCODING   L"UTF-16-BE"
#define UTF32_LE_ENCODING   L"UTF-32-LE"
#define UTF32_BE_ENCODING   L"UTF-32-BE"
#define UTF7_ENCODING       L"UTF-7"
#define UTF1_ENCODING       L"UTF-1"
#define UTF_EBCDIC_ENCODING L"UTF-EBCDIC"
#define SDSU_ENCODING       L"SDSU"
#define BOCU1_ENCODING      L"BOCU-1"
#define GB18030_ENCODING    L"GB-18030"
#define DEFAULT_ENCODING    L"DEFAULT"

BEGIN_NAMESPACE(core)

class file_data_provider : public data_provider, private noncopyable
{
    public:
        using datum_type = text::datum_type;

        using callback_type = std::function<bool(std::shared_ptr<datum_type[]>, void*, size_type&)>;

    private:
        string_type         my_file_name;
        callback_type       my_callback;

    private:
        static size_type    get_file_size(const string_type& file_name);
        static string_type  get_encoding(const string_type& file_name);

        static bool         read_raw_data(const string_type& file_name, std::shared_ptr<byte[]>& data, size_type& count, offset_type offset);

        static bool         read_utf8_data(std::shared_ptr<byte[]> raw_data,
                                           size_type raw_count,
                                           std::shared_ptr<datum_type[]>& data,
                                           size_type& count);

        static bool         read_utf16_data(std::shared_ptr<byte[]> raw_data,
                                            size_type raw_count,
                                            std::shared_ptr<datum_type[]>& data,
                                            size_type& count,
                                            bool big_endian);

        static bool         read_utf32_data(std::shared_ptr<byte[]> raw_data,
                                            size_type raw_count,
                                            std::shared_ptr<datum_type[]>& data,
                                            size_type& count,
                                            bool big_endian);
    public:
                            file_data_provider(const string_type& file_name, callback_type callback = nullptr);
        virtual            ~file_data_provider();

        virtual bool        load(std::shared_ptr<datum_type[]>& data, size_type& count) override;
};

END_NAMESPACE

#endif // __FILE_DATA_PROVIDER_H__
