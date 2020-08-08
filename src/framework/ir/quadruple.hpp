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
//  decl                nothing                                                 x: integer
//
//  =                   0                               x                       x = 0
//  -                   c                               tmp1                    tmp1 = -c
//  *                   b               tmp1            tmp2                    tmp2 = b * t1
//  +                   tmp2            tmp4            tmp5                    tmp5 = tmp2 + tmp4
//  =                   tmp5                            a                       a = tmp5
//
//  =[] (r-value)       y               i               tmp1                    x := y[i]
//  =                   tmp1                            x
//
//  []= (l-value)        x              i               tmp1                    x[i] = y
//  =                    y                              tmp1
//
//                                                                              x[i][j]..[k] need to calculate offset based on Horner's rule
//
//  -                    0              x               y                       y = -x
//  *                    1              x               y                       y = -x
//
//  <                    x              y               tmp1                    b = (x <= y)
//  ==                   x              y               tmp2
//  ||                   tmp1           tmp2            b
//
//  =                    a              b               tmp1                    if x relop y goto L   --->  if a == b then x = x + 1 else y = 20
//  if==0 (ifz)          tmp1                           jump address (1)
//  +                    x              1               tmp2
//  =                    tmp2                           x
//  jump                                                jump address (2)
//  =                    20                             y
//
//                                                                              loop  --->  L1: !ifz goto L2 ... goto L1
//
//  func                 name                                                   procedure/function definition
//  param                p1
//  param                p2
//  ...                  ...
//  param                pN
//  begin                                                                       procedure/function begin
//  return               y                                                      for function
//  end                                                                         procedure/function end
//  
//  param                p1                                                     procedure/function call foo(p1, p2, ... pN)
//  param                p2
//  ...                  ...
//  param                pN
//  call                 foo             N
//  
//  label                                               name
//  
//  jump                                                label                   goto label, label is a <index, symbol> pair
//
//  if==true             a                              label
//  address              b                              a
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
    using size_type = std::size_t;

    using label_type = std::pair<index_type, symbol_type>;

    using result_type = std::variant<symbol_type,       // temporary variables introduced during evaluation
                                     quadruple_type>;   // target label used with 'goto' or branching/jump op code
    index_type      index;

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
