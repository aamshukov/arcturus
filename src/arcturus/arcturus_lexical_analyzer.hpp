//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __ARCTURUS_LEXICAL_ANALYZER_H__
#define __ARCTURUS_LEXICAL_ANALYZER_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_lexical_analyzer : public lexical_analyzer
{
    //public:
    //    using fsa_type = fsa::fsa_type;
    //    using lexical_analyzer_type = std::shared_ptr<arcturus_lexical_analyzer>;

    //    using line_map_type = std::unique_ptr<loc_type[]>;
    //    using tab_map_type = std::vector<bool>;

    //    using indents_type = std::stack<std::size_t>;

    //private:
    //    bool                    my_unicode;         // true if the last obtained codepoint from unicode-escape
    //    uint8_t                 my_unicode_length;
    //    std::size_t             my_unicode_backslash_count;

    //    //line_map_type           my_line_map;        // start position of each line
    //    //std::size_t             my_line_map_size;
    //    //loc_type                my_cached_line;
    //    //loc_type                my_cached_line_position;

    //    //tab_map_type            my_tab_map;         // tab positions
    //    //uint8_t                 my_tab_size;        // tab size, default is 4

    //                                                // off-side rule support, Peter Landin
    //    std::int32_t            my_pending_indents; // > 0 indents, < 0 dedents, python
    //    indents_type            my_indents;         // stack of indents, theoretically unlimited
    //    bool                    my_boll;            // true if at the begining of a new logical line
    //    bool                    my_eoll;            // true if at the end of a new logical line, default implementation assumes each physical eol is logical eol

    //private:
    //    static void             build_fsa_from_re(const string_type& re,
    //                                              token_type::traits::type token,
    //                                              token_type::traits::type escape_token,
    //                                              const string_type& escape_predicate,
    //                                              fsa_type& result_fsa);

    //    datum_type              consume_unicode_escape(bool check_for_surrogates = true);

    //    datum_type              advance();
    //    datum_type              peek(uint16_t k = 1);
    //    void                    rewind();

    //    //void                    build_line_map();
    //    //loc_type                find_line_number(loc_type position);

    //    void                    calculate_indentation();

    //protected:
    //    virtual void            next_lexeme_impl() override;

    //public:
    //    static void             build_numeric_literals(const string_type& file_name, const string_type& label_prefix);
    //    static void             build_identifiers_and_keywords(const string_type& file_name, const string_type& label_prefix);

    //public:
    //                            arcturus_lexical_analyzer(const content_type& content, uint8_t tab_size);
    //                           ~arcturus_lexical_analyzer();

    //    static bool             create(const string_type& file_name,
    //                                   lexical_analyzer_type& result_lexical_analyzer,
    //                                   operation_status& status,
    //                                   uint8_t tab_size = 4);
};

END_NAMESPACE

#endif // __ARCTURUS_LEXICAL_ANALYZER_H__
