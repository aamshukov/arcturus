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

template <typename Token, typename Traits>
struct quadruple
{
    using token_type = Token;
    using traits_type = Traits;
    using operation_code = typename traits_type::operation_code;

    using symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
    using symbols_type = std::vector<symbol_type>;

    using quadruple_type = std::shared_ptr<quadruple<token_type, traits_type>>;

    using index_type = std::size_t;
    using size_type = std::size_t;

    using label_type = size_type;

    using result_type = std::variant<symbol_type,
                                     quadruple_type, // target label used with 'goto' op code
                                     label_type>;    // label number used with 'label' op code
    index_type      index;

    operation_code  operation;
    symbol_type     argument1;
    symbol_type     argument2;
    result_type     result; // result, destination address or label

    quadruple()
    {
    }

    quadruple(const operation_code& operation)
        : operation(operation)
    {
    }

    quadruple(const operation_code& operation,
              const result_type& result)
        : operation(operation), result(result)
    {
    }

    quadruple(const operation_code& operation,
              const symbol_type& argument1,
              const result_type& result)
        : operation(operation), argument1(argument1), result(result)
    {
    }

    quadruple(const operation_code& operation,
              const symbol_type& argument1,
              const symbol_type& argument2,
              const result_type& result)
        : operation(operation), argument1(argument1), argument2(argument2), result(result)
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
