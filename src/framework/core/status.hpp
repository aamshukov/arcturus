//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __STATUS_H__
#define __STATUS_H__

#pragma once

#define SYSTEM_SUCCESS_CODE (NO_ERROR)
#define SYSTEM_ERROR_CODE   (-1)

BEGIN_NAMESPACE(core)

class status
{
    public:
        enum class custom_code
        {
            success = 1,
            info    = 2,
            warning = 3,
            error   = -1
        };

        enum class contributer
        {
            core      = 0,
            lexer     = 1,
            parser    = 2,
            semantics = 3,
            lir       = 4,
            mir       = 5,
            hir       = 6,
            generator = 7
        };

        using custom_code_type = custom_code;
        using system_code_type = uint32_t;
        using library_code_type = uint32_t;

        using contributer_type = contributer;

    private:
        custom_code_type    my_custom_code;
        system_code_type    my_system_code;
        library_code_type   my_library_code;

        contributer_type    my_contributer;

        string_type         my_text;

    public:
                            status();
                           ~status();

                            status(const status& other);
                            status(status&& other);

        const status&       operator = (const status& other);
        status&             operator = (status&& other);

        custom_code_type    custom_code() const;
        custom_code_type&   custom_code();

        system_code_type    system_code() const;
        system_code_type&   system_code();

        library_code_type   library_code() const;
        library_code_type&  library_code();

        contributer_type    contributer() const;
        contributer_type&   contributer();

        const string_type&  text() const;
        string_type&        text();

    public:
        static string_type  get_system_error_message();
};

inline status::custom_code_type status::custom_code() const
{
    return my_custom_code;
}

inline status::custom_code_type& status::custom_code()
{
    return my_custom_code;
}

inline status::system_code_type status::system_code() const
{
    return my_system_code;
}

inline status::system_code_type& status::system_code()
{
    return my_system_code;
}

inline status::library_code_type status::library_code() const
{
    return my_library_code;
}

inline status::library_code_type& status::library_code()
{
    return my_library_code;
}

inline status::contributer_type status::contributer() const
{
    return my_contributer;
}

inline status::contributer_type& status::contributer()
{
    return my_contributer;
}

inline const string_type& status::text() const
{
    return my_text;
}

inline string_type& status::text()
{
    return const_cast<string_type&>(static_cast<const status&>(*this).text());
}

using operation_status = status;

END_NAMESPACE

#define OPERATION_FAILED(__custom_code, __contributer, __template, ...)                     \
{                                                                                           \
    result = false;                                                                         \
                                                                                            \
    operation_status status;                                                                \
                                                                                            \
    status.custom_code() = __custom_code;                                                   \
    status.system_code() = ::GetLastError();                                                \
    status.contributer() = __contributer;                                                   \
                                                                                            \
    status.text().assign(format(__template, ##__VA_ARGS__));                                \
    status.text().append(L"\n");                                                            \
    status.text().append(status.get_system_error_message());                                \
                                                                                            \
    log_error(status.text().c_str());                                                       \
                                                                                            \
    diagnostics::instance().state() = false;                                                \
    diagnostics::instance().add(std::move(status));                                         \
}

#define OPERATION_FAILED_LIB(__custom_code, __library_code, __contributer, __template, ...) \
{                                                                                           \
    result = false;                                                                         \
                                                                                            \
    operation_status status;                                                                \
                                                                                            \
    status.custom_code() = __custom_code;                                                   \
    status.system_code() = ::GetLastError();                                                \
    status.library_code() = __library_code;                                                 \
    status.contributer() = __contributer;                                                   \
                                                                                            \
    status.text().assign(format(__template, ##__VA_ARGS__));                                \
    status.text().append(L"\n");                                                            \
    status.text().append(status.get_system_error_message());                                \
                                                                                            \
    log_error(status.text().c_str());                                                       \
                                                                                            \
    diagnostics::instance().state() = false;                                                \
    diagnostics::instance().add(std::move(status));                                         \
}

#define OPERATION_FAILED_EX(__ex, __custom_code, __contributer, __template, ...)            \
    OPERATION_FAILED(__custom_code, __contributer, __template, , ##__VA_ARGS__)             \
    log_exception(__ex, DUMMY_EXCEPTION_TEXT);

#endif // __STATUS_H__
