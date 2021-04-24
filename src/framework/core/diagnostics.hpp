//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DIAGNOSTICS_H__
#define __DIAGNOSTICS_H__

#pragma once

BEGIN_NAMESPACE(core)

class diagnostics : public singleton<diagnostics>
{
    public:
        using status_type = status;

        using data_type = std::vector<status_type>;

        using message_template_key_type = uint64_t;
        using message_templates_type = std::unordered_map<message_template_key_type, string_type>;

    private:
        data_type                       my_data;
        bool                            my_state; // quick state check, true - valid (continue), false - erroneous

        std::size_t                     my_spurious_errors; // how many spurious error before termination

        message_templates_type          my_message_templates;

    public:
                                        diagnostics();

        const data_type                 warnings() const;
        const data_type                 errors() const;

        const data_type&                data() const;

        const status_type               last_status() const;

        bool                            state() const;
        bool&                           state();

        std::size_t                     spurious_errors() const;
        std::size_t&                    spurious_errors();

        const message_templates_type&   message_templates() const;
        message_templates_type&         message_templates();

        void                            add(status_type&& status);

        string_type                     format(uint64_t code, ...);
};

inline diagnostics::diagnostics()
                  : my_spurious_errors(128), my_state(true)
{
}

inline const typename diagnostics::data_type diagnostics::warnings() const
{
    data_type result;

    std::for_each(my_data.begin(),
                  my_data.end(),
                  [&result](const auto& status)
                  {
                      if(status.custom_code() == status::custom_code::warning)
                      {
                          result.emplace_back(status);
                      }
                  });

    return result;
}

inline const typename diagnostics::data_type diagnostics::errors() const
{
    data_type result;

    std::for_each(my_data.begin(),
                  my_data.end(),
                  [&result](const auto& status)
                  {
                      if(status.custom_code() == status::custom_code::error)
                      {
                          result.emplace_back(status);
                      }
                  });

    return result;
}

inline const typename diagnostics::data_type& diagnostics::data() const
{
    return my_data;
}

inline const typename diagnostics::status_type diagnostics::last_status() const
{
    if(!my_data.empty())
    {
        return my_data[my_data.size() - 1];
    }
    else
    {
        return status();
    }
}

inline bool diagnostics::state() const
{
    return my_state;
}

inline bool& diagnostics::state()
{
    return my_state;
}

inline std::size_t diagnostics::spurious_errors() const
{
    return my_spurious_errors;
}

inline std::size_t& diagnostics::spurious_errors()
{
    return my_spurious_errors;
}

inline const typename diagnostics::message_templates_type& diagnostics::message_templates() const
{
    return my_message_templates;
}

inline typename diagnostics::message_templates_type& diagnostics::message_templates()
{
    return my_message_templates;
}

inline void typename diagnostics::add(typename diagnostics::status_type&& status)
{
    if(my_data.size() < my_spurious_errors)
    {
        my_data.emplace_back(std::move(status));
    }
}

inline string_type diagnostics::format(uint64_t code, ...)
{
    string_type result;

    auto it(my_message_templates.find(code));

    if(it != my_message_templates.end())
    {
        const auto& message_template((*it).second);

        char_type buffer[256 * 1024];

        va_list arguments;

        va_start(arguments, message_template.c_str());

        vsnwprintf(buffer, array_size(buffer), TRUNCATE, message_template.c_str(), arguments);

        va_end(arguments);
    }

    return result;
}

#define OPERATION_FAILED(__custom_code, __library_code, __contributer, __template, ...)     \
{                                                                                           \
    result = false;                                                                         \
                                                                                            \
    operation_status status;                                                                \
                                                                                            \
    status.custom_code() = __custom_code;                                                   \
    status.system_code() = ::GetLastError();                                                \
    status.library_code() = __library_code;                                                 \
    status.contributor() = __contributer;                                                   \
                                                                                            \
    status.text().assign(format(__template, ##__VA_ARGS__));                                \
    status.text().append(L"\n");                                                            \
    status.text().append(status.get_system_error_message());                                \
                                                                                            \
    diagnostics::instance().state() = false;                                                \
    diagnostics::instance().add(std::move(status));                                         \
}

#define OPERATION_FAILED_EX(__ex, __custom_code, __contributer, __template, ...)            \
    OPERATION_FAILED(__custom_code, 0, __contributer, __template, ##__VA_ARGS__)

END_NAMESPACE

#endif // __DIAGNOSTICS_H__
