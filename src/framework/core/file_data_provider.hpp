//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FILE_DATA_PROVIDER_H__
#define __FILE_DATA_PROVIDER_H__

#pragma once

BEGIN_NAMESPACE(core)

class file_data_provider : public data_provider, private noncopyable
{
    public:
        using datum_type = data_provider::datum_type;

        using callback_type = std::function<bool(std::shared_ptr<datum_type[]>, void*, size_type&)>;

    private:
        string_type         my_file_name;
        callback_type       my_callback;

    private:
        static size_type    get_file_size(const string_type& file_name);
        static string_type  get_encoding(const string_type& file_name);

        static bool         read_raw_data(const string_type& file_name, std::shared_ptr<byte[]>& data, size_type& count, offset_type offset, operation_status& status);

        static bool         read_utf8_data(std::shared_ptr<byte[]> raw_data, size_type raw_count, std::shared_ptr<datum_type[]>& data, size_type& count, operation_status& status);
        static bool         read_utf16_data(std::shared_ptr<byte[]> raw_data, size_type raw_count, std::shared_ptr<datum_type[]>& data, size_type& count, bool big_endian, operation_status& status);
        static bool         read_utf32_data(std::shared_ptr<byte[]> raw_data, size_type raw_count, std::shared_ptr<datum_type[]>& data, size_type& count, bool big_endian, operation_status& status);

    public:
                            file_data_provider(const string_type& file_name, callback_type callback = nullptr);
        virtual            ~file_data_provider();

        virtual bool        load0(std::shared_ptr<datum_type[]>& data, size_type& count, operation_status& status);
        virtual bool        load(std::shared_ptr<datum_type[]>& data, size_type& count, operation_status& status) override;
};

END_NAMESPACE

#endif // __FILE_DATA_PROVIDER_H__
