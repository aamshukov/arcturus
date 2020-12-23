//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
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
//  if-true             x                               label                   goto label, label is quadruple_type
//  if-false            x                               label                   goto label, label is quadruple_type
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
template <typename Token, typename OpCodeTraits>
struct quadruple : public list
{
    using token_type = Token;
    using traits_type = OpCodeTraits;
    using operation_code = typename traits_type::operation_code;

    using symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
    using symbols_type = std::vector<symbol_type>;

    using quadruple_type = std::shared_ptr<quadruple<token_type, traits_type>>;

    using id_type = std::size_t;

    using result_type = std::variant<symbol_type,       // temporary variable introduced during evaluation
                                     quadruple_type>;   // target label used with 'goto' or branch/jump op code
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

    symbol_type     argument1;
    symbol_type     argument2;

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
              const symbol_type& argument1,
              const result_type& result)
        : id(i), operation(operation), argument1(argument1), result(result)
    {
    }

    quadruple(const id_type& i,
              const operation_code& operation,
              const symbol_type& argument1,
              const symbol_type& argument2,
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
