//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_FUNC_TYPE_H__
#define __ARCTURUS_FUNC_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_func_type : public arcturus_scalar_type
{
    public:
        using param_type = std::shared_ptr<arcturus_type>;
        using params_type = std::vector<param_type>;
        using return_type = std::shared_ptr<arcturus_type>;

    private:
        params_type         my_type_params;   // type params <T, R, P, ...>
        params_type         my_formal_params; // value params integer, real, ...
        return_type         my_result;

        bool                my_function; // has return value, otherwise procedure
        bool                my_variadic;

    public:
                            arcturus_func_type(kind_type kind); // function_type or procedure_type
                           ~arcturus_func_type();

        friend bool         operator == (const arcturus_func_type& lhs, const arcturus_func_type& rhs);
        friend bool         operator != (const arcturus_func_type& lhs, const arcturus_func_type& rhs);

        const params_type&  type_params() const;
        params_type&        type_params();

        const params_type&  formal_params() const;
        params_type&        formal_params();

        const return_type&  result() const;
        return_type&        result();

        bool                function() const;
        bool&               function();

        bool                variadic() const;
        bool&               variadic();
};

inline const typename arcturus_func_type::params_type& arcturus_func_type::type_params() const
{
    return my_type_params;
}

inline typename arcturus_func_type::params_type& arcturus_func_type::type_params()
{
    return my_type_params;
}

inline const typename arcturus_func_type::params_type& arcturus_func_type::formal_params() const
{
    return my_formal_params;
}

inline typename arcturus_func_type::params_type& arcturus_func_type::formal_params()
{
    return my_formal_params;
}

inline const typename arcturus_func_type::return_type& arcturus_func_type::result() const
{
    return my_result;
}

inline typename arcturus_func_type::return_type& arcturus_func_type::result()
{
    return my_result;
}

inline bool arcturus_func_type::function() const
{
    return my_function;
}

inline bool& arcturus_func_type::function()
{
    return my_function;
}

inline bool arcturus_func_type::variadic() const
{
    return my_variadic;
}

inline bool& arcturus_func_type::variadic()
{
    return my_variadic;
}

END_NAMESPACE

#endif // __ARCTURUS_FUNC_TYPE_H__
