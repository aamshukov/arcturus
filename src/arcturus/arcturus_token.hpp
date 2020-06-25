//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_TOKEN_H__
#define __ARCTURUS_TOKEN_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

struct arcturus_token_traits : public token_traits
{
    DECLARE_ENUM
    (
        type,
        uint32_t,
        unknown = 0,
        epsilon = 5,
        ws,     //  6
        eol,    //  7
        eos,    //  8
        indent, //  9, literal = '    '
        dedent, // 10, literal = '    '

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

using arcturus_token = token<arcturus_token_traits>;

END_NAMESPACE

#endif // __ARCTURUS_TOKEN_H__
