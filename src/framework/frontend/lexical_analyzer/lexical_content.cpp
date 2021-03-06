//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <content/data_provider.hpp>
#include <content/content.hpp>

#include <frontend/lexical_analyzer/lexical_content.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

lexical_content::lexical_content(const id_type& id, const source_type& source, uint8_t tab_size)
               : my_line_map_size(0),
                 my_cached_line(-1),
                 my_cached_line_position(-1),
                 my_tab_size(tab_size),
                 content(id, source)
{
}

lexical_content::~lexical_content()
{
}

// OpenJDK 9+ for more details ...
void lexical_content::build_line_map()
{
    const datum_type* ptr(my_data.get());
    const datum_type* start_content(ptr);
    const datum_type* end_content(start_content + my_count);

    my_tab_map.resize(my_count);

    std::size_t k = 0;

    line_map_type line_map(std::make_unique<loc_type[]>(my_count));

    while(ptr < end_content)
    {
        line_map[k++] = std::ptrdiff_t(ptr - start_content);

        do
        {
            if(*ptr == L'\r' || *ptr == L'\n')
            {
                if((*ptr == L'\r' && *(ptr + 1) == L'\n') || (*ptr == L'\n' && *(ptr + 1) == L'\r'))
                {
                    ptr += 2;
                }
                else
                {
                    ptr++;
                }

                break;
            }
            else if(*ptr == L'\t')
            {
                my_tab_map[std::ptrdiff_t(ptr - start_content)] = true;
            }
        }
        while(++ptr < end_content);
    }

    my_line_map = std::make_unique<loc_type[]>(k);
    my_line_map_size = k;

    memcpy(my_line_map.get(), line_map.get(), k * sizeof(loc_type));
}

// OpenJDK 9+ for more details ...
loc_type lexical_content::find_line_number(loc_type position)
{
    loc_type result = 0;

    if(my_line_map_size > 0)
    {
        if(position == my_cached_line_position)
        {
            result = my_cached_line;
        }
        else
        {
            my_cached_line_position = position;

            loc_type low = 0;
            loc_type high = my_line_map_size - 1;

            while(low <= high)
            {
                loc_type mid = (low + high) >> 1;
                loc_type val = my_line_map[mid];

                if(val < position)
                {
                    low = mid + 1;
                }
                else if(val > position)
                {
                    high = mid - 1;
                }
                else
                {
                    result = my_cached_line = mid + 1;
                    goto done;
                }
            }

            result = my_cached_line = low;
done:
            ;
        }
    }

    return result;
}

// OpenJDK 9+ for more details ...
loc_type lexical_content::get_line_number(loc_type position)
{
    return find_line_number(position);
}

// OpenJDK 9+ for more details ...
loc_type lexical_content::get_column_number(loc_type position)
{
    loc_type result = 0;

    if(my_line_map_size > 0)
    {
        loc_type offset = my_line_map[find_line_number(position) - 1]; // 1 is the first line
        loc_type column = 0;

        for(loc_type k = offset; k < position; k++)
        {
            if(my_tab_map[k])
            {
                column = ((column / my_tab_size) * my_tab_size) + my_tab_size;
            }
            else
            {
                column++;
            }
        }

        result = column + 1; // 1 is the first column
    }

    return result;
}

bool lexical_content::load(data_provider& provider)
{
    bool result = content::load(provider);

    build_line_map();

    return result;
}

END_NAMESPACE
