//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ABSTRACT_TYPE_H__
#define __ABSTRACT_TYPE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

template <typename Traits>
class abstract_type : public type
{
    public:
        using traits_type = Traits;

        using kind_type = typename traits_type::kind;
        using attributes_type = typename traits_type::attributes;

    protected:
        kind_type           my_kind;
        attributes_type     my_attributes;

    public:
                            abstract_type(kind_type kind = abstract_type<traits_type>::kind_type::unknown_type,
                                          attributes_type attributes = abstract_type<traits_type>::attributes_type::unknown_type);

                            abstract_type(const abstract_type& other) = default;
                            abstract_type(abstract_type&& other) = default;

        virtual            ~abstract_type() = 0;

        abstract_type&      operator = (const abstract_type& other) = default;
        abstract_type&      operator = (abstract_type&& other) = default;

        kind_type           kind() const;
        kind_type&          kind();

        attributes_type     attributes() const;
        attributes_type&    attributes();
};

template <typename Traits>
abstract_type<Traits>::abstract_type(typename abstract_type<Traits>::kind_type kind,
                                     typename abstract_type<Traits>::attributes_type attributes)
                     : my_kind(kind), my_attributes(attributes)
{
}

template <typename Traits>
abstract_type<Traits>::~abstract_type()
{
}

template <typename Traits>
inline typename abstract_type<Traits>::kind_type abstract_type<Traits>::kind() const
{
    return my_kind;
}

template <typename Traits>
inline typename abstract_type<Traits>::kind_type& abstract_type<Traits>::kind()
{
    return my_kind;
}

template <typename Traits>
inline typename abstract_type<Traits>::attributes_type abstract_type<Traits>::attributes() const
{
    return my_attributes;
}

template <typename Traits>
inline typename abstract_type<Traits>::attributes_type& abstract_type<Traits>::attributes()
{
    return my_attributes;
}

END_NAMESPACE

#endif // __ABSTRACT_TYPE_H__
