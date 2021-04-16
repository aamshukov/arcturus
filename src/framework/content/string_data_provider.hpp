//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __STRING_DATA_PROVIDER_H__
#define __STRING_DATA_PROVIDER_H__

#pragma once

BEGIN_NAMESPACE(core)

class string_data_provider : public data_provider, private noncopyable
{
    private:
        string_type     my_data_content;

    public:
                        string_data_provider(const string_type& data_content);
        virtual        ~string_data_provider();

        virtual bool    load(std::shared_ptr<cp_type[]>& data, std::size_t& count) override;
};

END_NAMESPACE

#endif // __STRING_DATA_PROVIDER_H__
