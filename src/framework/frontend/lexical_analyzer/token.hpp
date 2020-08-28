//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TOKEN_H__
#define __TOKEN_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

struct token_traits
{
    DECLARE_ENUM
    (
        type,
        uint32_t,
        unknown = 0,
        epsilon = 5,
        ws,         //  6
        eol,        //  7
        eos,        //  8
        indent,     //  9,  literal = '    '
        dedent,     //  10, literal = '    '

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

template <typename Traits = token_traits>
struct token
{
    using id_type = std::size_t; // should be in sync with lexical_analyzer's

    enum class flag : uint64_t
    {
        clear     = 0x00,
        genuine   = 0x01,
        contextual= 0x02, // contextual, recognized in specific contexts, similar to C# get/set, async/await ...
        synthetic = 0x04  // additional (artificial) tokens which are inserted into the token stream ...
    };

    DECLARE_ENUM_OPERATORS(flag)

    using flags_type = flag;

    using traits = Traits;
    using token_type = typename traits::type;

    token_type      type;    // type of lexeme

    loc_type        offset;  // offset in context (absolute address)
    uint32_t        length;  // length of lexeme

    codepoints_type literal; // string or char literal (if unicode - always decoded), numeric value

    flags_type      flags;

    id_type         source;  // lexical analyser which recognizes this lexeme, could be from a different file

    private:
        void accept(const token& other)
        {
            if(this != &other)
            {
                type = other.type;

                offset = other.offset;
                length = other.length;

                literal = other.literal;

                flags = other.flags;

                source = other.source;
            }
        }

    public:
        token()
        {
            reset();
        }

        token(const token& other)
        {
            accept(other);
        }
        
        token(token&& other)
        {
            type = other.type;

            offset = other.offset;
            length = other.length;

            literal = std::move(other.literal);

            flags = other.flags;

            source = other.source;
        }

        token& operator = (const token& other)
        {
            accept(other);
            return *this;
        }

        token& operator = (token&& other)
        {
            type = other.type;

            offset = other.offset;
            length = other.length;

            literal = std::move(other.literal);

            flags = other.flags;

            source = other.source;

            return *this;
        }

        bool operator == (const token& other) const
        {
            return type == other.type &&
                   offset == other.offset &&
                   length == other.length &&
                   literal == other.literal &&
                   flags == other.flags &&
                   source == other.source;
        }

        void reset()
        {
            type = token_type::unknown;

            offset = INVALID_VALUE;
            length = INVALID_VALUE;

            flags = flags_type::clear;
            flags |= flags_type::genuine;

            source = 0;
        }

        codepoints_type codepoints(const datum_type* content) const
        {
            codepoints_type result;
            result.assign(content + offset, length);
            return result;
        }

        string_type to_string(const datum_type* content) const
        {
            string_type result;
            
            text::codepoints_to_string(content + offset, length, result);

            result += L" [" + name() + L"]:" + std::to_wstring(length);
            
            return result;
        }

        string_type name() const
        {
            return name(type);
        }

        static string_type name(const typename traits::type& token_type)
        {
            if(traits::mapping.empty())
            {
                traits::initialize();
            }

            return traits::name(token_type);
        }
};

END_NAMESPACE

#endif // __TOKEN_H__


