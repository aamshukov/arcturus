//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __CONTENT_H__
#define __CONTENT_H__

#pragma once

BEGIN_NAMESPACE(core)

class content : private noncopyable
{
    public:
        using data_type = std::shared_ptr<datum_type[]>;

        using id_type = std::size_t;
        using source_type = string_type;

    protected:
        id_type             my_id;
        source_type         my_source;

        data_type           my_data;
        std::size_t         my_count;

    public:
                            content(const id_type& id, const source_type& source);
                           ~content();

        const id_type&      id() const;
        id_type&            id();

        const source_type&  source() const;
        source_type&        source();

        const datum_type*   data() const;
        std::size_t         count() const;

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

inline const datum_type* content::data() const
{
    const datum_type *result(nullptr);

    if(my_data)
    {
        result = my_data.get();
    }

    return result;
}

inline std::size_t content::count() const
{
    return my_count;
}

END_NAMESPACE

#endif // __CONTENT_H__
