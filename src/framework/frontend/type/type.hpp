//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TYPE_H__
#define __TYPE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

//template <typename TReturn, typename TParam>
class type : public noncopyable
{
    public:
        using size_type = std::size_t;

        //using return_type = TReturn;
        //using param_type = TParam;


    private:
        size_type       my_size; // size in bits, width for runtime allocation


        //flags       my_flags;
    //private:
    //    hash_type   hashcode();

        bool            is_function() const; //??
        bool            is_procedure() const; //??

    public:
                        type();
                       ~type();

        bool            operator == (const type& other);
        bool            operator != (const type& other);

    size_type           size() const;


    //    return_type accept(const type_visitor_type<return_type, param_type>& visitor, param_type visitor_param);

};

//template <typename TReturn, typename TParam>
//inline typename type<TReturn, TParam>::size_type type<TReturn, TParam>::size() const
inline typename type::size_type type::size() const
{
    return my_size;
}

END_NAMESPACE

#endif // __TYPE_H__
