//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LEXICAL_CONTENT_H__
#define __LEXICAL_CONTENT_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class lexical_content : public content
{
    public:
        using data_type = std::shared_ptr<datum_type[]>;

        using line_map_type = std::unique_ptr<loc_type[]>;
        using tab_map_type = std::vector<bool>;

    private:
        line_map_type   my_line_map;        // start position of each line
        uint32_t        my_line_map_size;
        loc_type        my_cached_line;
        loc_type        my_cached_line_position;

        tab_map_type    my_tab_map;         // tab positions
        uint8_t         my_tab_size;        // tab size, default is 4

    private:
        loc_type        find_line_number(loc_type position);
        void            build_line_map();

    public:
                        lexical_content(const id_type& id, const source_type& source, uint8_t tab_size = 4);
                       ~lexical_content();

        loc_type        get_line_number(loc_type position);
        loc_type        get_column_number(loc_type position);

        bool            load(data_provider& provider) override;
};

END_NAMESPACE

#endif // __LEXICAL_CONTENT_H__
