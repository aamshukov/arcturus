//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <content/data_provider.hpp>
#include <content/content.hpp>

#include <frontend/lexical_analyzer/token.hpp>
#include <frontend/lexical_analyzer/lexical_content.hpp>
#include <frontend/lexical_analyzer/lexical_analyzer.hpp>

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

lexical_analyzer::lexical_analyzer(const typename lexical_analyzer::content_type& content)
                : my_id(0),
                  my_content(content),
                  my_start_content((*my_content).data()),
                  my_end_content(my_start_content + (*my_content).count()),
                  my_ptr(my_start_content),
                  my_ptr_lexeme(nullptr)
{
}

lexical_analyzer::~lexical_analyzer()
{
}

void lexical_analyzer::next_lexeme()
{
    if(!is_eos())
    {
        my_prev_token = std::move(my_token);

        if(!my_tokens.empty())
        {
            my_token = std::move(my_tokens.front());
            my_tokens.pop();
            my_ptr = my_start_content + my_token.offset;
        }
        else
        {
            prolog();
            next_lexeme_impl();
            epilog();
        }
    }
}

const typename lexical_analyzer::token_type& lexical_analyzer::lookahead_lexeme()
{
    token_type* result(nullptr);

    // check if anything need to be done
    if(is_eos())
    {
        result = &my_token;
    }
    else
    {
        // push state
        const cp_type* aux_ptr(my_ptr);

        auto number_of_lexems = statistics::instance().number_of_lexems();

        token_type aux_token(my_token);

        // update state
        if(!my_tokens.empty())
        {
            const token_type& token(my_tokens.back());
            my_ptr = my_start_content + token.offset + token.length;
        }

        // get lexeme
        prolog();
        next_lexeme_impl();
        epilog();

        bool push_token = true;

        if(is_eos() && !my_tokens.empty())
        {
            const token_type& token(my_tokens.back());
            push_token = token.type != token_type::traits::type::eos;
        }

        if(push_token)
        {
            my_tokens.emplace(std::move(my_token));
        }

        // pop state
        my_token = std::move(aux_token);

        my_ptr = aux_ptr;

        statistics::instance().number_of_lexems() = number_of_lexems;

        // result
        result = &my_tokens.back();
    }

    return *result;
}

inline void lexical_analyzer::prolog()
{
    my_token.reset();

    my_ptr_lexeme = my_ptr;
}

inline void lexical_analyzer::epilog()
{
    if(my_ptr > my_end_content)
    {
        my_ptr = my_end_content;
    }

    my_token.offset = std::ptrdiff_t(my_ptr_lexeme - my_start_content);
    my_token.length = static_cast<std::size_t>(std::ptrdiff_t(my_ptr - my_ptr_lexeme));

    my_token.literal.assign(my_ptr_lexeme, my_token.length);

    my_token.flags = token_type::flags_type::clear;
    my_token.source = (*my_content).id();

    // update stats
    statistics::instance().number_of_lexems()++;
}

END_NAMESPACE
