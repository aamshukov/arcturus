//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __STATUS_H__
#define __STATUS_H__

#pragma once

#define SYSTEM_SUCCESS_CODE (NO_ERROR)
#define SYSTEM_ERROR_CODE   (-1)

BEGIN_NAMESPACE(core)

class status
{
    public:
        static const uint64_t info_mask        = 0x0001'0000'0000'0000;
        static const uint64_t warning_mask     = 0x0010'0000'0000'0000;
        static const uint64_t error_mask       = 0x0100'0000'0000'0000;
        static const uint64_t fatal_error_mask = 0x1000'0000'0000'0000;

        enum class custom_code : uint64_t
        {
            success           = 1,
            info              = 2 | info_mask,
            warning           = 3 | warning_mask,
            error             = 0 | error_mask,
            fatal_error       = 0 | fatal_error_mask,

            invalid_literal   = 128 | error_mask,
            status_deprecated = 129 | warning_mask
        };

        enum class contributor
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
        using system_code_type = std::size_t;
        using library_code_type = std::size_t;

        using contributor_type = contributor;

    private:
        custom_code_type    my_custom_code;
        system_code_type    my_system_code;
        library_code_type   my_library_code;

        contributor_type    my_contributor;

        string_type         my_text;

    public:
                            status();
                           ~status();

                            status(const status& other);
                            status(status&& other);

        status&             operator = (const status& other);
        status&             operator = (status&& other);

        custom_code_type    custom_code() const;
        custom_code_type&   custom_code();

        system_code_type    system_code() const;
        system_code_type&   system_code();

        library_code_type   library_code() const;
        library_code_type&  library_code();

        contributor_type    contributor() const;
        contributor_type&   contributor();

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

inline status::contributor_type status::contributor() const
{
    return my_contributor;
}

inline status::contributor_type& status::contributor()
{
    return my_contributor;
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
