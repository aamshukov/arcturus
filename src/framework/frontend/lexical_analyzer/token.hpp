//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __TOKEN_H__
#define __TOKEN_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

struct token_traits
{
    DECLARE_ENUM
    (
        type,
        std::size_t,
        unknown = 0,
        epsilon = 5,
        ws,                                     //  6
        eol,                                    //  7
        eos,                                    //  8
        indent,                                 //  9,  literal = '    '
        dedent,                                 //  10, literal = '    '

        binary_integer_lit,
        octal_integer_lit,
        decimal_integer_lit,
        hexadecimal_integer_lit,
        decimal_floating_lit,
        hexadecimal_floating_lit,

        char_lit,
        string_lit,

        abstract_kw,
        assert_kw,
        boolean_kw,
        break_kw,
        byte_kw,
        case_kw,
        catch_kw,
        char_kw,
        class_kw,
        const_kw,
        continue_kw,
        default_kw,
        do_kw,
        double_kw,
        else_kw,
        enum_kw,
        extends_kw,
        final_kw,
        finally_kw,
        float_kw,
        for_kw,
        if_kw,
        goto_kw,
        implements_kw,
        import_kw,
        instanceof_kw,
        int_kw,
        interface_kw,
        long_kw,
        native_kw,
        new_kw,
        package_kw,
        private_kw,
        protected_kw,
        public_kw,
        return_kw,
        short_kw,
        static_kw,
        strictfp_kw,
        super_kw,
        switch_kw,
        synchronized_kw,
        this_kw,
        throw_kw,
        throws_kw,
        transient_kw,
        try_kw,
        void_kw,
        volatile_kw,
        while_kw,

        var_kw,

        true_lit,
        false_lit,
        null_lit,

        identifier,

        left_paren_sep,                         // (
        right_paren_sep,                        // )
        left_brace_sep,                         // {
        right_brace_sep,                        // }
        left_bracket_sep,                       // [
        righ_bracket_sep,                       // ]
        semicolon_sep,                          // ;
        comma_sep,                              // ,
        dot_sep,                                // .
        dots_sep,                               // ...
        at_sep,                                 // @
        colon_sep,                              // :
        colons_sep,                             // ::

        addition_op,                            // +
        addition_assignment_op,                 // +=
        increment_op,                           // ++

        subtraction_op,                         // -
        subtraction_assignment_op,              // -=
        decrement_op,                           // --
        lambda_op,                              // ->
        
        multiplication_op,                      // *
        multiplication_assignment_op,           // *=
        
        division_op,                            // /
        division_assignment_op,                 // /=
        
        bitwise_and_op,                         // &
        logical_and_op,                         // &&
        bitwise_assignment_op,                  // &=

        bitwise_or_op,                          // |
        logical_or_op,                          // ||
        bitwise_or_assignment_op,               // |=
        
        bitwise_xor_op,                         // ^
        bitwise_xor_assignment_op,              // ^=

        logical_not_op,                         // !
        logical_not_assignment_op,              // !=

        module_op,                              // %
        module_assignment_op,                   // %=
        
        assignment_op,                          // =
        equality_op,                            // ==

        bitwise_not_op,                         // ~

        ternary_true_op,                        // ?

        greater_than_op,                        // >
        greater_than_equal_op,                  // >=
        signed_right_shift_op,                  // >>
        signed_right_shift_assignment_op,       // >>=
        unsigned_right_shift_op,                // >>>
        unsigned_right_shift_assignment_op,     // >>>=

        less_than_op,                           // <
        less_than_equal_op,                     // <=
        left_shift_op,                          // <<
        left_shift_assignment_op,               // <<=

        singleline_comments,                    // //
        multiline_comments,                     // /**/

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

template <typename Traits>
struct token
{
    using id_type = std::size_t; // should be in sync with lexical_analyzer's

    enum class flag : uint64_t
    {
        clear     = 0x00,
        genuine   = 0x01,
        contextual= 0x02, // contextual, recognized in specific contexts, similar to C# get/set, async/await ...
        synthetic = 0x04  // additional (artificial) tokens which are inserted into the token stream ...
    };

    DECLARE_ENUM_OPERATORS(flag)

    using flags_type = flag;

    using traits = Traits;
    using token_type = typename traits::type;

    token_type      type;    // type of lexeme

    loc_type        offset;  // offset in context (absolute address)
    std::size_t     length;  // length of lexeme

    codepoints_type literal; // string or char literal (if unicode - always decoded), numeric value

    flags_type      flags;

    id_type         source;  // lexical analyser which recognizes this lexeme, could be from different files

    private:
        void accept(const token& other)
        {
            if(this != &other)
            {
                type = other.type;

                offset = other.offset;
                length = other.length;

                literal = other.literal;

                flags = other.flags;

                source = other.source;
            }
        }

    public:
        token()
        {
            reset();
        }

        token(const token& other)
        {
            accept(other);
        }
        
        token(token&& other)
        {
            type = other.type;

            offset = other.offset;
            length = other.length;

            literal = std::move(other.literal);

            flags = other.flags;

            source = other.source;
        }

        token& operator = (const token& other)
        {
            accept(other);
            return *this;
        }

        token& operator = (token&& other)
        {
            type = other.type;

            offset = other.offset;
            length = other.length;

            literal = std::move(other.literal);

            flags = other.flags;

            source = other.source;

            return *this;
        }

        bool operator == (const token& other) const
        {
            return type == other.type &&
                   offset == other.offset &&
                   length == other.length &&
                   literal == other.literal &&
                   flags == other.flags &&
                   source == other.source;
        }

        void reset()
        {
            type = token_type::unknown;

            offset = INVALID_VALUE;
            length = INVALID_VALUE;

            flags = flags_type::clear;
            flags |= flags_type::genuine;

            source = 0;
        }

        codepoints_type codepoints(const datum_type* content) const
        {
            codepoints_type result;
            result.assign(content + offset, length);
            return result;
        }

        string_type to_string(const datum_type* content) const
        {
            string_type result;
            
            text::codepoints_to_string(content + offset, length, result);

            result += L" [" + name() + L"]:" + std::to_wstring(length);
            
            return result;
        }

        string_type name() const
        {
            return name(type);
        }

        static string_type name(const typename traits::type& token_type)
        {
            if(traits::mapping.empty())
            {
                traits::initialize();
            }

            return traits::name(token_type);
        }
};

END_NAMESPACE

#endif // __TOKEN_H__
