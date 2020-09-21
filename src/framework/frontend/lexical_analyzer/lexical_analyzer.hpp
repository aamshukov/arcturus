//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __LEXICAL_ANALYZER_H__
#define __LEXICAL_ANALYZER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
class lexical_analyzer : private noncopyable
{
    public:
        using content_type = std::shared_ptr<lexical_content>;

        using token_type = Token;
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

template <typename Token>
inline typename lexical_analyzer<Token>::id_type lexical_analyzer<Token>::id() const
{
    return my_id;
}

template <typename Token>
inline typename lexical_analyzer<Token>::id_type& lexical_analyzer<Token>::id()
{
    return my_id;
}

template <typename Token>
inline const typename lexical_analyzer<Token>::content_type& lexical_analyzer<Token>::content() const
{
    return my_content;
}

template <typename Token>
inline typename lexical_analyzer<Token>::content_type& lexical_analyzer<Token>::content()
{
    return const_cast<content_type&>(static_cast<const lexical_analyzer&>(*this).content());
}

template <typename Token>
inline datum_type typename lexical_analyzer<Token>::current() const
{
    return *my_ptr;
}

template <typename Token>
inline const typename lexical_analyzer<Token>::token_type& lexical_analyzer<Token>::token()
{
    return my_token;
}

template <typename Token>
inline bool lexical_analyzer<Token>::is_eol() const
{
    return my_token.type == token_type::traits::type::eol;
}

template <typename Token>
inline bool lexical_analyzer<Token>::is_eos() const
{
    return my_token.type == token_type::traits::type::eos;
}

template <typename Token>
inline typename codepoints_type lexical_analyzer<Token>::lexeme_to_codepoints() const
{
    return my_token.codepoints(my_start_content);
}

template <typename Token>
inline typename codepoints_type lexical_analyzer<Token>::lexeme_to_codepoints(const typename lexical_analyzer<Token>::token_type& token) const
{
    return token.codepoints(my_start_content);
}

template <typename Token>
inline string_type lexical_analyzer<Token>::lexeme_to_string() const
{
    return my_token.to_string(my_start_content);
}

template <typename Token>
inline string_type lexical_analyzer<Token>::lexeme_to_string(const typename lexical_analyzer<Token>::token_type& token) const
{
    return token.to_string(my_start_content);
}

template <typename Token>
inline void lexical_analyzer<Token>::take_snapshot()
{
    my_snapshots.push(my_ptr);
}

template <typename Token>
inline void lexical_analyzer<Token>::rewind_to_snapshot()
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
