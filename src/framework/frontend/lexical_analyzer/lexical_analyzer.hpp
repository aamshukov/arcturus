//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LEXICAL_ANALYZER_H__
#define __LEXICAL_ANALYZER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename TToken>
class lexical_analyzer : private noncopyable
{
    public:
        using content_type = std::shared_ptr<lexical_content>;

        using datum_type = text::datum_type;
        using codepoints_type = std::basic_string<datum_type>;

        using token_type = TToken;
        using tokens_type = std::queue<token_type>;

        using snapshots_type = std::stack<const datum_type*>;

    protected:
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

template <typename TToken>
inline const typename lexical_analyzer<TToken>::content_type& lexical_analyzer<TToken>::content() const
{
    return my_content;
}

template <typename TToken>
inline typename lexical_analyzer<TToken>::content_type& lexical_analyzer<TToken>::content()
{
    return const_cast<content_type&>(static_cast<const lexical_analyzer&>(*this).content());
}

template <typename TToken>
inline typename lexical_analyzer<TToken>::datum_type typename lexical_analyzer<TToken>::current() const
{
    return *my_ptr;
}

template <typename TToken>
inline const typename lexical_analyzer<TToken>::token_type& lexical_analyzer<TToken>::token()
{
    return my_token;
}

template <typename TToken>
inline bool lexical_analyzer<TToken>::is_eol() const
{
    return my_token.type == token_type::traits::type::eol;
}

template <typename TToken>
inline bool lexical_analyzer<TToken>::is_eos() const
{
    return my_token.type == token_type::traits::type::eos;
}

template <typename TToken>
inline typename lexical_analyzer<TToken>::codepoints_type lexical_analyzer<TToken>::lexeme_to_codepoints() const
{
    return my_token.codepoints(my_start_content);
}

template <typename TToken>
inline typename lexical_analyzer<TToken>::codepoints_type lexical_analyzer<TToken>::lexeme_to_codepoints(const typename lexical_analyzer<TToken>::token_type& token) const
{
    return token.codepoints(my_start_content);
}

template <typename TToken>
inline string_type lexical_analyzer<TToken>::lexeme_to_string() const
{
    return my_token.to_string(my_start_content);
}

template <typename TToken>
inline string_type lexical_analyzer<TToken>::lexeme_to_string(const typename lexical_analyzer<TToken>::token_type& token) const
{
    return token.to_string(my_start_content);
}

template <typename TToken>
inline void lexical_analyzer<TToken>::take_snapshot()
{
    my_snapshots.push(my_ptr);
}

template <typename TToken>
inline void lexical_analyzer<TToken>::rewind_to_snapshot()
{
    if(!my_snapshots.empty())
    {
        my_ptr = my_snapshots.top();

        my_snapshots.pop();

        my_tokens.clear();
    }
}

END_NAMESPACE

#define CURRENT_LOCATION                                                                                                                \
    (*my_content).get_line_number(std::ptrdiff_t(std::ptrdiff_t(my_ptr - my_start_content) - std::ptrdiff_t(my_ptr - my_ptr_lexeme))),  \
    (*my_content).get_column_number(std::ptrdiff_t(std::ptrdiff_t(my_ptr - my_start_content) - std::ptrdiff_t(my_ptr - my_ptr_lexeme)))

#include <frontend\lexical_analyzer\lexical_analyzer.inl>

#endif // __LEXICAL_ANALYZER_H__
