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

//  operation           argument1       argument2       result
//  ..........................................................
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
//  if true             x                               label                   goto label, label is quadruple_type
//  if false            x                               label                   goto label, label is quadruple_type
//  goto                                                label                   goto label, label is quadruple_type
//  label                                               name
//
//  function            name                                                    procedure/function definition
//  param               p1
//  param               p2
//  ...                 ...
//  param               pN
//  begin                                                                       procedure/function begin
//  return              y                                                       function
//  end                                                                         procedure/function end
//
//  param               p1                                                      procedure/function call foo(p1, p2, ... pN)
//  param               p2                                   
//  ...                 ...
//  param               pN
//  call                foo             N
//
template <typename Token, typename OpCodeTraits>
struct quadruple : public list
{
    using token_type = Token;
    using traits_type = OpCodeTraits;
    using operation_code = typename traits_type::operation_code;

    using symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
    using symbols_type = std::vector<symbol_type>;

    using quadruple_type = std::shared_ptr<quadruple<token_type, traits_type>>;

    using index_type = std::size_t;

    using result_type = std::variant<symbol_type,       // temporary variable introduced during evaluation
                                     quadruple_type>;   // target label used with 'goto' or branch/jump op code
    enum class flag : uint64_t
    {
        clear = 0x0000,
        leader = 0x0001
    };

    DECLARE_ENUM_OPERATORS(flag)

    using flags_type = flag;

    index_type      index;
    flags_type      flags;

    operation_code  operation;

    symbol_type     argument1;
    symbol_type     argument2;

    result_type     result;

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
            index     = other.index;
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
            index     = other.index;
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
            index     = other.index;
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
            index     = other.index;
            flags     = other.flags;
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
