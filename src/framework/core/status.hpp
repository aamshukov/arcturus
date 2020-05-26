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
            grammar   = 1,
            fsa       = 2,
            lexer     = 3,
            parser    = 4,
            semantics = 5,
            lir       = 6,
            mir       = 7,
            hir       = 8,
            generator = 9
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

#endif // __STATUS_H__
