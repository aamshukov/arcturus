//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __QUADRUPLE_H__
#define __QUADRUPLE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

struct operation_code_traits
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
        goto_statemenet     = 301,



        phi                 = 500
    )
};

template <typename Token, typename Traits = operation_code_traits>
struct quadruple
{
    using token_type = Token;
    using traits = Traits;
    using operation_code = typename traits::operation_code;

    using symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
    using symbols_type = std::vector<symbol_type>;

    using quadruple_type = std::shared_ptr<quadruple<token_type, traits>>;

    using index_type = std::size_t;
    using size_type = std::size_t;

    using label_type = size_type;

    union result
    {
        symbol_type     symbol;
        quadruple_type  target; // target label used with 'goto' op code
        label_type      label;  // label number used with 'label' op code
    };

    using result_type = quadruple::result;

    index_type      index;

    operation_code  operation;
    symbol_type     argument1;
    symbol_type     argument2;
    result_type     result; // result, destination address or label

    quadruple()
    {
    }

    quadruple(const operation_code& operation,
              const symbol_type& argument1,
              const symbol_type& argument2,
              const result_type& result)
        : operation(operation), argument1(argument1), argument2(argument2), result(result)
    {
    }

    quadruple(const operation_code& operation,
              const symbol_type& argument1,
              const result_type& result)
        : quadruple(operation, argument1, nullptr, result)
    {
    }

    quadruple(const operation_code& operation,
              const result_type& result)
        : quadruple(operation, nullptr, nullptr, result)
    {
    }

    quadruple(const operation_code& operation)
        : quadruple(operation, nullptr, nullptr, nullptr)
    {
    }

    quadruple(const quadruple& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }
    }

    quadruple(quadruple&& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }
    }

    quadruple& operator = (const quadruple& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }

        return *this;
    }

    quadruple& operator = (quadruple&& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }

        return *this;
    }
};

END_NAMESPACE

#endif // __QUADRUPLE_H__
