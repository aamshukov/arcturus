//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __QUADRUPLE_H__
#define __QUADRUPLE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)

// 𝛗

//  operation           argument1       argument2       result
//  ..........................................................
//  noop                                                                        no operation
//
//  decl                name            type            value                   x: integer = 0  --->  x = 0
//  decl                                                                        x: integer
//
//  x = y
//  =                   y                               x                       x = y
//  =                   0                               x                       x = 0
//  =[] (r-value)       y               i               t1                      x := y[i], where t1 = y[i]
//  []= (l-value)       x               i               t1                      x[i] = y, where t1 = x[i]
//                                                                              x[i][j]..[k] need to calculate offset based on Horner's rule
//
//  x = OP y
//  +                   y                               x                       x = +y
//  -                   y                               x                       x = -y
//  !                   y                               x                       x = !y
//  ~                   y                               x                       x = ~y
//
//  x = y OP z
//  +                   y               z               x                       x = y + z
//  -                   y               z               x                       x = y - z
//  *                   y               z               x                       x = y * z
//  /                   y               z               x                       x = y / z
//  ==                  y               z               x                       x = y == z
//  !=                  y               z               x                       x = y != z
//  <                   y               z               x                       x = y < z
//  >                   y               z               x                       x = y > z
//  &&                  y               z               x                       x = y && z
//  ||                  y               z               x                       x = y || z
//  &                   y               z               x                       x = y & z
//  |                   y               z               x                       x = y | z
//  ^                   y               z               x                       x = y ^ z
//
//  if-true             x                               label                   goto label, label is quadruple_type
//  if-false            x                               label                   goto label, label is quadruple_type
//  else
//  goto                                                label                   goto label, label is quadruple_type
//  label                                               name
//
//  function            name                                                    procedure/function definition
//  getparam            p1                                                      get param
//  getparam            p2                                                      get param
//  ...                 ...
//  getparam            pN                                                      get param
//  begin                                                                       procedure/function begin
//  return              y                                                       function
//  end                                                                         procedure/function end
//
//                                                                              procedure/function call foo(p1, p2, ... pN)
//  putparam            p1                                                      put param
//  putparam            p2                                                      put param
//  ...                 ...                                                     
//  putparam            pN                                                      put param
//  call                foo             N
//
//  𝛗                   y                               params                  y - symbol
//                                                                              N - how many params (predecessors)
//
template <typename TSymbol, typename OpCodeTraits>
struct quadruple : public list
{
    using token_type = token<token_traits>;
    using traits_type = OpCodeTraits;
    using operation_code = typename traits_type::operation_code;

    using id_type = std::size_t;

    using symbol_type = std::shared_ptr<TSymbol>;
    using argument_type = std::pair<symbol_type, id_type>; // <symbol, ssa version>
    using quadruple_type = std::shared_ptr<quadruple<TSymbol, traits_type>>;

    using phi_param_type = std::pair<argument_type, std::shared_ptr<vertex>>; // holds symbol:basic_block
    using phi_params_type = std::vector<phi_param_type>;

    using result_type = std::variant<argument_type,     // temporary variable introduced during evaluation
                                     quadruple_type,    // target label used with 'goto' or branch/jump op code
                                     phi_params_type>;
    enum class flag : uint64_t
    {
        clear = 0x0000,
        leader = 0x0001
    };

    DECLARE_ENUM_OPERATORS(flag)

    using flags_type = flag;

    id_type         id;
    flags_type      flags = flags_type::clear;

    operation_code  operation;

    argument_type   argument1;
    argument_type   argument2;

    result_type     result;

    quadruple(const id_type& i)
        : id(i)
    {
    }

    quadruple(const id_type& i, const operation_code& operation)
        : id(i), operation(operation)
    {
    }

    quadruple(const id_type& i,
              const operation_code& operation,
              const result_type& result)
        : id(i), operation(operation), result(result)
    {
    }

    quadruple(const id_type& i,
              const operation_code& operation,
              const argument_type& argument1,
              const result_type& result)
        : id(i), operation(operation), argument1(argument1), result(result)
    {
    }

    quadruple(const id_type& i,
              const operation_code& operation,
              const argument_type& argument1,
              const argument_type& argument2,
              const result_type& result)
        : id(i), operation(operation), argument1(argument1), argument2(argument2), result(result)
    {
    }

    quadruple(const quadruple& other)
    {
        if(this != &other)
        {
            id        = other.id;
            flags     = other.flags;
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
            id        = other.id;
            flags     = other.flags;
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
            id        = other.id;
            flags     = other.flags;
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
            id        = other.id;
            flags     = other.flags;
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }

        return *this;
    }

    virtual string_type to_string() = 0;
};

END_NAMESPACE

#endif // __QUADRUPLE_H__
