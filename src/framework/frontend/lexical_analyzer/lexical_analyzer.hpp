//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LEXICAL_ANALYZER_H__
#define __LEXICAL_ANALYZER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class lexical_analyzer : private noncopyable
{
    public:
        using content_type = std::shared_ptr<lexical_content>;

        using token_type = token<token_traits>;
        using tokens_type = std::queue<token_type>;

        using snapshots_type = std::stack<const datum_type*>;

        using id_type = std::size_t;

    protected:
        id_type                     my_id;              // master lexer, id = 0

        content_type                my_content;         // loaded content

        const datum_type*           my_start_content;   // begining of content
        const datum_type*           my_end_content;     // end of content

        token_type                  my_token;           // current lexeme
        tokens_type                 my_tokens;          // current and lookahead lexemes

        token_type                  my_prev_token;      // previous lexeme

        const datum_type*           my_ptr;             // current position in content
        const datum_type*           my_ptr_lexeme;      // begining position of lexeme in content

        snapshots_type              my_snapshots;       // backtracking's snapshots

    private:
        void                        prolog();
        void                        epilog();

    protected:
        virtual void                next_lexeme_impl() = 0;
                                    //{
                                    //    // !!! -- generated code -- !!!
                                    //    // !!! -- end of generated code -- !!!
                                    //}
    public:
                                    lexical_analyzer(const content_type& content);
                                   ~lexical_analyzer();

        id_type                     id() const;
        id_type&                    id();

        const content_type&         content() const;
        content_type&               content();

        datum_type                  current() const;

        const token_type&           token();

        bool                        is_eol() const;
        bool                        is_eos() const;

        void                        next_lexeme();
        const token_type&           lookahead_lexeme();

        codepoints_type             lexeme_to_codepoints() const;
        codepoints_type             lexeme_to_codepoints(const token_type& token) const;

        string_type                 lexeme_to_string() const;
        string_type                 lexeme_to_string(const token_type& token) const;

        void                        take_snapshot();
        void                        rewind_to_snapshot(); // backtrack
};

inline typename lexical_analyzer::id_type lexical_analyzer::id() const
{
    return my_id;
}

inline typename lexical_analyzer::id_type& lexical_analyzer::id()
{
    return my_id;
}

inline const typename lexical_analyzer::content_type& lexical_analyzer::content() const
{
    return my_content;
}

inline typename lexical_analyzer::content_type& lexical_analyzer::content()
{
    return const_cast<content_type&>(static_cast<const lexical_analyzer&>(*this).content());
}

inline datum_type typename lexical_analyzer::current() const
{
    return *my_ptr;
}

inline const typename lexical_analyzer::token_type& lexical_analyzer::token()
{
    return my_token;
}

inline bool lexical_analyzer::is_eol() const
{
    return my_token.type == token_type::traits::type::eol;
}

inline bool lexical_analyzer::is_eos() const
{
    return my_token.type == token_type::traits::type::eos;
}

inline typename codepoints_type lexical_analyzer::lexeme_to_codepoints() const
{
    return my_token.codepoints(my_start_content);
}

inline typename codepoints_type lexical_analyzer::lexeme_to_codepoints(const typename lexical_analyzer::token_type& token) const
{
    return token.codepoints(my_start_content);
}

inline string_type lexical_analyzer::lexeme_to_string() const
{
    return my_token.to_string(my_start_content);
}

inline string_type lexical_analyzer::lexeme_to_string(const typename lexical_analyzer::token_type& token) const
{
    return token.to_string(my_start_content);
}

inline void lexical_analyzer::take_snapshot()
{
    my_snapshots.push(my_ptr);
}

inline void lexical_analyzer::rewind_to_snapshot()
{
    if(!my_snapshots.empty())
    {
        my_ptr = my_snapshots.top();

        my_snapshots.pop();

        my_token.reset();

        tokens_type empty;
        std::swap(my_tokens, empty);
    }
}

END_NAMESPACE

#define CURRENT_LOCATION                                                                                                                \
    (*my_content).get_line_number(std::ptrdiff_t(std::ptrdiff_t(my_ptr - my_start_content) - std::ptrdiff_t(my_ptr - my_ptr_lexeme))),  \
    (*my_content).get_column_number(std::ptrdiff_t(std::ptrdiff_t(my_ptr - my_start_content) - std::ptrdiff_t(my_ptr - my_ptr_lexeme)))

#endif // __LEXICAL_ANALYZER_H__
