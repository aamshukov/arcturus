//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
//
// based on Andrew Durward example:
//      https://stackoverflow.com/questions/11796121/implementing-the-visitor-pattern-using-c-templates
// ... added return and parameter types
//
#ifndef __VISITOR_H__
#define __VISITOR_H__

#pragma once

BEGIN_NAMESPACE(core)

// template declaration
template <typename TReturn, typename TParam, typename ... TVisitables>
class visitor;

// specialization for single type    
template <typename TReturn, typename TParam, typename TVisitable>
class visitor<TReturn, TParam, TVisitable>
{
    public:
        using return_type = TReturn;
        using param_type = TParam;

    public:
        virtual TReturn visit(TVisitable& visitable, const TParam& param) = 0;
};

// specialization for multiple types
template <typename TReturn, typename TParam, typename TVisitable, typename ... TVisitables>
class visitor<TReturn, TParam, TVisitable, TVisitables ...> : public visitor<TReturn, TParam, TVisitables ...>
{
    public:
        using return_type = TReturn;
        using param_type = TParam;

    public:
        using visitor<TReturn, TParam, TVisitables ...>::visit; // promote the function(s) from the base class

    public:
        virtual TReturn visit(TVisitable& visitable, const TParam& param) = 0;
};

END_NAMESPACE

#endif // __VISITOR_H__
