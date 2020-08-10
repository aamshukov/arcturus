//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_QUADRUPLE_H__
#define __ARCTURUS_QUADRUPLE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
USINGNAMESPACE(backend)

struct arcturus_operation_code_traits
{
    DECLARE_ENUM
    (
        operation_code,
        uint32_t,
        unknown             = 0,

        address_of          = 1,

        load_integer        = 20,
        store_integer       = 21, // assignment
        add_integer         = 22,
        subtract_integer    = 23,
        multiply_integer    = 24,
        divide_integer      = 25,
        left_shift_integer  = 26,
        right_shift_integer = 27,
        bitwise_and_integer = 28,
        bitwise_or_integer  = 29,
        bitwise_xor_integer = 30,
        eq_integer          = 31,
        ne_integer          = 32,
        lt_integer          = 33,
        le_integer          = 34,
        gt_integer          = 35,
        ge_integer          = 36,
        negation_integer    = 37, // -

        load_real           = 50,
        store_real          = 51, // assignment
        add_real            = 52,
        subtract_real       = 53,
        multiply_real       = 54,
        divide_real         = 55,
        eq_real             = 56,
        ne_real             = 57,
        lt_real             = 58,
        le_real             = 59,
        gt_real             = 60,
        ge_real             = 61,
        negation_real       = 62, // - =

        logical_negation    = 80, // !
        bitwise_negation    = 81, // ~

        array_access        = 100, // []

        // subroutine, procedure, function ... definition
        procedure_begin     = 200,
        procedure_end       = 201,
        parameter           = 202,
        return_statement    = 203,

        // subroutine, procedure, function ... call (callsite)
        procedure_call      = 204,
        argument            = 205,

        label               = 300,
        goto_statement      = 301,

        // SSA
        phi                 = 500,

        sentinel            = 1000
    )
};

struct arcturus_quadruple : public quadruple<arcturus_token, arcturus_operation_code_traits>
{
    using quadruple_base = quadruple<arcturus_token, arcturus_operation_code_traits>;

    arcturus_quadruple()
    {
    }

    arcturus_quadruple(const operation_code& operation)
        : quadruple_base(operation)
    {
    }

    arcturus_quadruple(const operation_code& operation,
                       const result_type& result)
        : quadruple_base(operation, result)
    {
    }

    arcturus_quadruple(const operation_code& operation,
                       const symbol_type& argument1,
                       const result_type& result)
        : quadruple_base(operation, argument1, result)
    {
    }

    arcturus_quadruple(const operation_code& operation,
                       const symbol_type& argument1,
                       const symbol_type& argument2,
                       const result_type& result)
        : quadruple_base(operation, argument1, argument2, result)
    {
    }

    static string_type opcode_name(const typename arcturus_operation_code_traits::operation_code& opcode)
    {
        if(arcturus_operation_code_traits::mapping.empty())
        {
            arcturus_operation_code_traits::initialize();
        }

        return arcturus_operation_code_traits::name(opcode);
    }
};

END_NAMESPACE

#endif // __ARCTURUS_QUADRUPLE_H__
