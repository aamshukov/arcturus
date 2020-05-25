//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONTENT_H__
#define __CONTENT_H__

#pragma once

BEGIN_NAMESPACE(core)

class content : private noncopyable
{
    public:
        using datum_type = data_provider::datum_type;
        using data_type = std::shared_ptr<datum_type[]>;

        using id_type = int32_t;
        using source_type = string_type;

    protected:
        id_type             my_id;
        source_type         my_source;

        data_type           my_data;
        size_type           my_count;

    public:
                            content(const id_type& id, const source_type& source);
                           ~content();

        const id_type&      id() const;
        id_type&            id();

        const source_type&  source() const;
        source_type&        source();

        const datum_type*   data() const;
        size_type           count() const;

        virtual bool        load(data_provider& provider);
};

inline const content::id_type& content::id() const
{
    return my_id;
}

inline content::id_type& content::id()
{
    return my_id;
}

inline const content::source_type& content::source() const
{
    return my_source;
}

inline content::source_type& content::source()
{
    return my_source;
}

inline const content::datum_type* content::data() const
{
    const datum_type* result(nullptr);

    if(my_data)
    {
        result = my_data.get();
    }

    return result;
}

inline size_type content::count() const
{
    return my_count;
}

END_NAMESPACE

#endif // __CONTENT_H__
