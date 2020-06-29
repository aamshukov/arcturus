//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_STRUCT_TYPE_H__
#define __ARCTURUS_STRUCT_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_struct_type : public arcturus_type
{
    public:
        using field_type = std::shared_ptr<arcturus_type>;
        using fields_type = std::vector<field_type>;

    private:
        fields_type         my_fields;

    public:
                            arcturus_struct_type();
                           ~arcturus_struct_type();

        bool                operator == (const arcturus_type& other) override;
        bool                operator != (const arcturus_type& other) override;

        const fields_type&  fields() const;
        fields_type&        fields();
};

inline const typename arcturus_struct_type::fields_type& arcturus_struct_type::fields() const
{
    return my_fields;
}

inline typename arcturus_struct_type::fields_type& arcturus_struct_type::fields()
{
    return my_fields;
}

END_NAMESPACE

#endif // __ARCTURUS_STRUCT_TYPE_H__
