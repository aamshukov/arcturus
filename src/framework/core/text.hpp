//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TEXT_H__
#define __TEXT_H__

#pragma once

BEGIN_NAMESPACE(core)

#define NUMERIC_BUFFER_PARSE_SIZE (64)

class text : private noncopyable
{
    public:
        static constexpr datum_type ascii_numbers[128] =
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
            0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };

    public:
        static const datum_type invalid_codepoint = 0x0000FFFD;

        // http://www.unicode.org/glossary/#supplementary_code_point
        static const datum_type kSupplementaryCodePointStart = 0x010000;
        static const datum_type kSupplementaryCodePointEnd = 0x10FFFF;

        // http://www.unicode.org/glossary/#high_surrogate_code_unit
        static const datum_type kHighSurrogateStart = 0x0000D800;
        static const datum_type kHighSurrogateEnd = 0x0000DBFF;

        // http://www.unicode.org/glossary/#low_surrogate_code_unit
        static const datum_type kLowSurrogateStart = 0x0000DC00;
        static const datum_type kLowSurrogateEnd = 0x0000DFFF;

        static const datum_type kSurrogateStart = kHighSurrogateStart;
        static const datum_type kSurrogateEnd = kLowSurrogateEnd;

    public:
        static bool         chars_to_codepoints(const char* chars,
                                                const std::size_t& chars_count,
                                                std::shared_ptr<datum_type[]>& codepoints,
                                                std::size_t& codepoints_count);
        static codepoints_type
                            chars_to_codepoints(const char* chars, const std::size_t& chars_count);

        static bool         codepoints_to_chars(const datum_type* codepoints,
                                                const std::size_t& codepoints_count,
                                                std::shared_ptr<char[]>& chars,
                                                std::size_t& chars_count);

        static bool         string_to_codepoints0(const string_type& text, std::shared_ptr<datum_type[]>& codepoints, std::size_t& count);
        static bool         codepoints_to_string0(const datum_type* codepoints, std::size_t count, string_type& result_text);

        static bool         string_to_codepoints(const string_type& text, std::shared_ptr<datum_type[]>& codepoints, std::size_t& count);
        static bool         codepoints_to_string(const datum_type* codepoints, std::size_t count, string_type& result_text);
        static string_type  codepoints_to_string(const datum_type* codepoints, std::size_t count);

        static string_type  codepoint_to_string(datum_type codepoint);
        static bool         codepoint_to_string(datum_type codepoint, string_type& result_text);

        static datum_type   epsilon_codepoint();
        static datum_type   bad_codepoint();

        static datum_type   make_codepoint(datum_type high_surrogate_codeunit, datum_type low_surrogate_codeunit);

        static bool         is_high_surrogate(datum_type codeunit);
        static bool         is_low_surrogate(datum_type codeunit);
        static bool         is_surrogate(datum_type codeunit);
        static bool         is_surrogate_pair(datum_type high_surrogate_codeunit, datum_type low_surrogate_codeunit);

        static bool         in_range(datum_type codepoint, datum_type r0, datum_type r1);

        static string_type  trim(const string_type& text, const string_type& delimiters);
        static void         trim(const string_type& text, const string_type& delimiters, string_type& result_text);

        static void         split(const string_type& text, const string_type& delimiters, std::vector<string_type>& parts);

        static bool         starts_with(const string_type& text, const char_type* prefix);
        static bool         starts_with(const string_type& text, const string_type& prefix);

        static bool         is_digit(datum_type codepoint);

        static bool         is_identifier_start(datum_type codepoint);
        static bool         is_identifier_part(datum_type codepoint);

        static bool         is_java_identifier_start(datum_type codepoint);
        static bool         is_java_identifier_part(datum_type codepoint);

        static bool         is_identifier_ignorable(datum_type codepoint);

        static bool         unicode_category_cf(datum_type codepoint);
        static bool         unicode_category_ll(datum_type codepoint);
        static bool         unicode_category_lm(datum_type codepoint);
        static bool         unicode_category_lo(datum_type codepoint);
        static bool         unicode_category_lt(datum_type codepoint);
        static bool         unicode_category_lu(datum_type codepoint);
        static bool         unicode_category_mc(datum_type codepoint);
        static bool         unicode_category_mn(datum_type codepoint);
        static bool         unicode_category_nd(datum_type codepoint);
        static bool         unicode_category_nl(datum_type codepoint);
        static bool         unicode_category_pc(datum_type codepoint);
        static bool         unicode_category_zs(datum_type codepoint);

    public:
        #pragma warning(push) // due to C++20
        #pragma warning(disable : 4499)
        template <typename T>
        static T parse(const datum_type* lexeme, uint32_t length, int base);

        template <>
        static int8_t parse<int8_t>(const datum_type* lexeme, uint32_t  length, int base);

        template <>
        static uint8_t parse<uint8_t>(const datum_type* lexeme, uint32_t length, int base);

        template <>
        static int16_t parse<int16_t>(const datum_type* lexeme, uint32_t length, int base);

        template <>
        static uint16_t parse<uint16_t>(const datum_type* lexeme, uint32_t length, int base);

        template <>
        static int32_t parse<int32_t>(const datum_type* lexeme, uint32_t length, int base);

        template <>
        static uint32_t parse<uint32_t>(const datum_type* lexeme, uint32_t length, int base);

        template <>
        static int64_t parse<int64_t>(const datum_type* lexeme, uint32_t length, int base);

        template <>
        static uint64_t parse<uint64_t>(const datum_type* lexeme, uint32_t length, int base);

        template <>
        static float parse<float>(const datum_type* lexeme, uint32_t length, int);

        template <>
        static double parse<double>(const datum_type* lexeme, uint32_t length, int);
        #pragma warning(pop)
};

inline typename datum_type text::epsilon_codepoint()
{
    static datum_type result(0x000003B5);
    return result;
}

inline typename datum_type text::bad_codepoint()
{
    static datum_type result(0x0F000002);
    return result;
}

inline typename datum_type text::make_codepoint(typename datum_type high_surrogate_codeunit, typename datum_type low_surrogate_codeunit)
{
    return ((high_surrogate_codeunit - kHighSurrogateStart) << 10/* divide by 0x400*/) +
            (low_surrogate_codeunit - kLowSurrogateStart) + kSupplementaryCodePointStart;
}

inline bool text::is_high_surrogate(typename datum_type codeunit)
{
    return codeunit >= kHighSurrogateStart && codeunit <= kHighSurrogateEnd;
}

inline bool text::is_low_surrogate(typename datum_type codeunit)
{
    return codeunit >= kLowSurrogateStart && codeunit <= kLowSurrogateEnd;
}

inline bool text::is_surrogate(typename datum_type codeunit)
{
    return codeunit >= kSurrogateStart && codeunit <= kSurrogateEnd;
}

inline bool text::is_surrogate_pair(typename datum_type high_surrogate_codeunit, typename datum_type low_surrogate_codeunit)
{
    return is_high_surrogate(high_surrogate_codeunit) && is_low_surrogate(low_surrogate_codeunit);
}

inline bool text::in_range(datum_type codepoint, datum_type r0, datum_type r1)
{
    return codepoint >= r0 && codepoint <= r1;
}

inline bool text::is_digit(typename datum_type codepoint)
{
    bool result = false;

    switch(codepoint)
    {
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            result = true;
        default:
            result = u_isdigit(codepoint);
    }

    return result;
}

inline bool text::is_identifier_start(typename datum_type codepoint)
{
    bool result = false;

    switch(codepoint)
    {
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case '$': case '_':
            result = true;
        default:
            result = u_isIDStart(codepoint);
    }

    return result;
}

inline bool text::is_identifier_part(typename datum_type codepoint)
{
    bool result = false;

    switch(codepoint)
    {
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case '$': case '_':
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            result = true;
        default:
            result = u_isIDPart(codepoint);
    }

    return result;
}

inline bool text::is_java_identifier_start(typename datum_type codepoint)
{
    bool result = false;

    switch(codepoint)
    {
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case '$': case '_':
            result = true;
        default:
            result = u_isJavaIDStart(codepoint);
    }

    return result;
}

inline bool text::is_java_identifier_part(typename datum_type codepoint)
{
    bool result = false;

    switch(codepoint)
    {
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case '$': case '_':
        case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            result = true;
        default:
            result = u_isJavaIDPart(codepoint);
    }

    return result;
}

inline bool text::is_identifier_ignorable(datum_type codepoint)
{
    return u_isIDIgnorable(codepoint);
}

inline bool text::unicode_category_cf(datum_type codepoint)
{
    return in_range(codepoint, 0x00AD, 0x00AD) ||
           in_range(codepoint, 0x0600, 0x0605) ||
           in_range(codepoint, 0x061C, 0x061C) ||
           in_range(codepoint, 0x06DD, 0x06DD) ||
           in_range(codepoint, 0x070F, 0x070F) ||
           in_range(codepoint, 0x08E2, 0x08E2) ||
           in_range(codepoint, 0x180E, 0x180E) ||
           in_range(codepoint, 0x200B, 0x200F) ||
           in_range(codepoint, 0x202A, 0x202E) ||
           in_range(codepoint, 0x2060, 0x2064) ||
           in_range(codepoint, 0x2066, 0x206F) ||
           in_range(codepoint, 0xFEFF, 0xFEFF) ||
           in_range(codepoint, 0xFFF9, 0xFFFB) ||
           in_range(codepoint, 0x110BD, 0x110BD) ||
           in_range(codepoint, 0x110CD, 0x110CD) ||
           in_range(codepoint, 0x13430, 0x13438) ||
           in_range(codepoint, 0x1BCA0, 0x1BCA3) ||
           in_range(codepoint, 0x1D173, 0x1D17A) ||
           in_range(codepoint, 0xE0001, 0xE0001) ||
           in_range(codepoint, 0xE0020, 0xE007F);
}

inline bool text::unicode_category_ll(datum_type codepoint)
{
    return in_range(codepoint, 0x0061, 0x007A) ||
           in_range(codepoint, 0x00B5, 0x00B5) ||
           in_range(codepoint, 0x00DF, 0x00F6) ||
           in_range(codepoint, 0x00F8, 0x00FF) ||
           in_range(codepoint, 0x0101, 0x0101) ||
           in_range(codepoint, 0x0103, 0x0103) ||
           in_range(codepoint, 0x0105, 0x0105) ||
           in_range(codepoint, 0x0107, 0x0107) ||
           in_range(codepoint, 0x0109, 0x0109) ||
           in_range(codepoint, 0x010B, 0x010B) ||
           in_range(codepoint, 0x010D, 0x010D) ||
           in_range(codepoint, 0x010F, 0x010F) ||
           in_range(codepoint, 0x0111, 0x0111) ||
           in_range(codepoint, 0x0113, 0x0113) ||
           in_range(codepoint, 0x0115, 0x0115) ||
           in_range(codepoint, 0x0117, 0x0117) ||
           in_range(codepoint, 0x0119, 0x0119) ||
           in_range(codepoint, 0x011B, 0x011B) ||
           in_range(codepoint, 0x011D, 0x011D) ||
           in_range(codepoint, 0x011F, 0x011F) ||
           in_range(codepoint, 0x0121, 0x0121) ||
           in_range(codepoint, 0x0123, 0x0123) ||
           in_range(codepoint, 0x0125, 0x0125) ||
           in_range(codepoint, 0x0127, 0x0127) ||
           in_range(codepoint, 0x0129, 0x0129) ||
           in_range(codepoint, 0x012B, 0x012B) ||
           in_range(codepoint, 0x012D, 0x012D) ||
           in_range(codepoint, 0x012F, 0x012F) ||
           in_range(codepoint, 0x0131, 0x0131) ||
           in_range(codepoint, 0x0133, 0x0133) ||
           in_range(codepoint, 0x0135, 0x0135) ||
           in_range(codepoint, 0x0137, 0x0138) ||
           in_range(codepoint, 0x013A, 0x013A) ||
           in_range(codepoint, 0x013C, 0x013C) ||
           in_range(codepoint, 0x013E, 0x013E) ||
           in_range(codepoint, 0x0140, 0x0140) ||
           in_range(codepoint, 0x0142, 0x0142) ||
           in_range(codepoint, 0x0144, 0x0144) ||
           in_range(codepoint, 0x0146, 0x0146) ||
           in_range(codepoint, 0x0148, 0x0149) ||
           in_range(codepoint, 0x014B, 0x014B) ||
           in_range(codepoint, 0x014D, 0x014D) ||
           in_range(codepoint, 0x014F, 0x014F) ||
           in_range(codepoint, 0x0151, 0x0151) ||
           in_range(codepoint, 0x0153, 0x0153) ||
           in_range(codepoint, 0x0155, 0x0155) ||
           in_range(codepoint, 0x0157, 0x0157) ||
           in_range(codepoint, 0x0159, 0x0159) ||
           in_range(codepoint, 0x015B, 0x015B) ||
           in_range(codepoint, 0x015D, 0x015D) ||
           in_range(codepoint, 0x015F, 0x015F) ||
           in_range(codepoint, 0x0161, 0x0161) ||
           in_range(codepoint, 0x0163, 0x0163) ||
           in_range(codepoint, 0x0165, 0x0165) ||
           in_range(codepoint, 0x0167, 0x0167) ||
           in_range(codepoint, 0x0169, 0x0169) ||
           in_range(codepoint, 0x016B, 0x016B) ||
           in_range(codepoint, 0x016D, 0x016D) ||
           in_range(codepoint, 0x016F, 0x016F) ||
           in_range(codepoint, 0x0171, 0x0171) ||
           in_range(codepoint, 0x0173, 0x0173) ||
           in_range(codepoint, 0x0175, 0x0175) ||
           in_range(codepoint, 0x0177, 0x0177) ||
           in_range(codepoint, 0x017A, 0x017A) ||
           in_range(codepoint, 0x017C, 0x017C) ||
           in_range(codepoint, 0x017E, 0x0180) ||
           in_range(codepoint, 0x0183, 0x0183) ||
           in_range(codepoint, 0x0185, 0x0185) ||
           in_range(codepoint, 0x0188, 0x0188) ||
           in_range(codepoint, 0x018C, 0x018D) ||
           in_range(codepoint, 0x0192, 0x0192) ||
           in_range(codepoint, 0x0195, 0x0195) ||
           in_range(codepoint, 0x0199, 0x019B) ||
           in_range(codepoint, 0x019E, 0x019E) ||
           in_range(codepoint, 0x01A1, 0x01A1) ||
           in_range(codepoint, 0x01A3, 0x01A3) ||
           in_range(codepoint, 0x01A5, 0x01A5) ||
           in_range(codepoint, 0x01A8, 0x01A8) ||
           in_range(codepoint, 0x01AA, 0x01AB) ||
           in_range(codepoint, 0x01AD, 0x01AD) ||
           in_range(codepoint, 0x01B0, 0x01B0) ||
           in_range(codepoint, 0x01B4, 0x01B4) ||
           in_range(codepoint, 0x01B6, 0x01B6) ||
           in_range(codepoint, 0x01B9, 0x01BA) ||
           in_range(codepoint, 0x01BD, 0x01BF) ||
           in_range(codepoint, 0x01C6, 0x01C6) ||
           in_range(codepoint, 0x01C9, 0x01C9) ||
           in_range(codepoint, 0x01CC, 0x01CC) ||
           in_range(codepoint, 0x01CE, 0x01CE) ||
           in_range(codepoint, 0x01D0, 0x01D0) ||
           in_range(codepoint, 0x01D2, 0x01D2) ||
           in_range(codepoint, 0x01D4, 0x01D4) ||
           in_range(codepoint, 0x01D6, 0x01D6) ||
           in_range(codepoint, 0x01D8, 0x01D8) ||
           in_range(codepoint, 0x01DA, 0x01DA) ||
           in_range(codepoint, 0x01DC, 0x01DD) ||
           in_range(codepoint, 0x01DF, 0x01DF) ||
           in_range(codepoint, 0x01E1, 0x01E1) ||
           in_range(codepoint, 0x01E3, 0x01E3) ||
           in_range(codepoint, 0x01E5, 0x01E5) ||
           in_range(codepoint, 0x01E7, 0x01E7) ||
           in_range(codepoint, 0x01E9, 0x01E9) ||
           in_range(codepoint, 0x01EB, 0x01EB) ||
           in_range(codepoint, 0x01ED, 0x01ED) ||
           in_range(codepoint, 0x01EF, 0x01F0) ||
           in_range(codepoint, 0x01F3, 0x01F3) ||
           in_range(codepoint, 0x01F5, 0x01F5) ||
           in_range(codepoint, 0x01F9, 0x01F9) ||
           in_range(codepoint, 0x01FB, 0x01FB) ||
           in_range(codepoint, 0x01FD, 0x01FD) ||
           in_range(codepoint, 0x01FF, 0x01FF) ||
           in_range(codepoint, 0x0201, 0x0201) ||
           in_range(codepoint, 0x0203, 0x0203) ||
           in_range(codepoint, 0x0205, 0x0205) ||
           in_range(codepoint, 0x0207, 0x0207) ||
           in_range(codepoint, 0x0209, 0x0209) ||
           in_range(codepoint, 0x020B, 0x020B) ||
           in_range(codepoint, 0x020D, 0x020D) ||
           in_range(codepoint, 0x020F, 0x020F) ||
           in_range(codepoint, 0x0211, 0x0211) ||
           in_range(codepoint, 0x0213, 0x0213) ||
           in_range(codepoint, 0x0215, 0x0215) ||
           in_range(codepoint, 0x0217, 0x0217) ||
           in_range(codepoint, 0x0219, 0x0219) ||
           in_range(codepoint, 0x021B, 0x021B) ||
           in_range(codepoint, 0x021D, 0x021D) ||
           in_range(codepoint, 0x021F, 0x021F) ||
           in_range(codepoint, 0x0221, 0x0221) ||
           in_range(codepoint, 0x0223, 0x0223) ||
           in_range(codepoint, 0x0225, 0x0225) ||
           in_range(codepoint, 0x0227, 0x0227) ||
           in_range(codepoint, 0x0229, 0x0229) ||
           in_range(codepoint, 0x022B, 0x022B) ||
           in_range(codepoint, 0x022D, 0x022D) ||
           in_range(codepoint, 0x022F, 0x022F) ||
           in_range(codepoint, 0x0231, 0x0231) ||
           in_range(codepoint, 0x0233, 0x0239) ||
           in_range(codepoint, 0x023C, 0x023C) ||
           in_range(codepoint, 0x023F, 0x0240) ||
           in_range(codepoint, 0x0242, 0x0242) ||
           in_range(codepoint, 0x0247, 0x0247) ||
           in_range(codepoint, 0x0249, 0x0249) ||
           in_range(codepoint, 0x024B, 0x024B) ||
           in_range(codepoint, 0x024D, 0x024D) ||
           in_range(codepoint, 0x024F, 0x0293) ||
           in_range(codepoint, 0x0295, 0x02AF) ||
           in_range(codepoint, 0x0371, 0x0371) ||
           in_range(codepoint, 0x0373, 0x0373) ||
           in_range(codepoint, 0x0377, 0x0377) ||
           in_range(codepoint, 0x037B, 0x037D) ||
           in_range(codepoint, 0x0390, 0x0390) ||
           in_range(codepoint, 0x03AC, 0x03CE) ||
           in_range(codepoint, 0x03D0, 0x03D1) ||
           in_range(codepoint, 0x03D5, 0x03D7) ||
           in_range(codepoint, 0x03D9, 0x03D9) ||
           in_range(codepoint, 0x03DB, 0x03DB) ||
           in_range(codepoint, 0x03DD, 0x03DD) ||
           in_range(codepoint, 0x03DF, 0x03DF) ||
           in_range(codepoint, 0x03E1, 0x03E1) ||
           in_range(codepoint, 0x03E3, 0x03E3) ||
           in_range(codepoint, 0x03E5, 0x03E5) ||
           in_range(codepoint, 0x03E7, 0x03E7) ||
           in_range(codepoint, 0x03E9, 0x03E9) ||
           in_range(codepoint, 0x03EB, 0x03EB) ||
           in_range(codepoint, 0x03ED, 0x03ED) ||
           in_range(codepoint, 0x03EF, 0x03F3) ||
           in_range(codepoint, 0x03F5, 0x03F5) ||
           in_range(codepoint, 0x03F8, 0x03F8) ||
           in_range(codepoint, 0x03FB, 0x03FC) ||
           in_range(codepoint, 0x0430, 0x045F) ||
           in_range(codepoint, 0x0461, 0x0461) ||
           in_range(codepoint, 0x0463, 0x0463) ||
           in_range(codepoint, 0x0465, 0x0465) ||
           in_range(codepoint, 0x0467, 0x0467) ||
           in_range(codepoint, 0x0469, 0x0469) ||
           in_range(codepoint, 0x046B, 0x046B) ||
           in_range(codepoint, 0x046D, 0x046D) ||
           in_range(codepoint, 0x046F, 0x046F) ||
           in_range(codepoint, 0x0471, 0x0471) ||
           in_range(codepoint, 0x0473, 0x0473) ||
           in_range(codepoint, 0x0475, 0x0475) ||
           in_range(codepoint, 0x0477, 0x0477) ||
           in_range(codepoint, 0x0479, 0x0479) ||
           in_range(codepoint, 0x047B, 0x047B) ||
           in_range(codepoint, 0x047D, 0x047D) ||
           in_range(codepoint, 0x047F, 0x047F) ||
           in_range(codepoint, 0x0481, 0x0481) ||
           in_range(codepoint, 0x048B, 0x048B) ||
           in_range(codepoint, 0x048D, 0x048D) ||
           in_range(codepoint, 0x048F, 0x048F) ||
           in_range(codepoint, 0x0491, 0x0491) ||
           in_range(codepoint, 0x0493, 0x0493) ||
           in_range(codepoint, 0x0495, 0x0495) ||
           in_range(codepoint, 0x0497, 0x0497) ||
           in_range(codepoint, 0x0499, 0x0499) ||
           in_range(codepoint, 0x049B, 0x049B) ||
           in_range(codepoint, 0x049D, 0x049D) ||
           in_range(codepoint, 0x049F, 0x049F) ||
           in_range(codepoint, 0x04A1, 0x04A1) ||
           in_range(codepoint, 0x04A3, 0x04A3) ||
           in_range(codepoint, 0x04A5, 0x04A5) ||
           in_range(codepoint, 0x04A7, 0x04A7) ||
           in_range(codepoint, 0x04A9, 0x04A9) ||
           in_range(codepoint, 0x04AB, 0x04AB) ||
           in_range(codepoint, 0x04AD, 0x04AD) ||
           in_range(codepoint, 0x04AF, 0x04AF) ||
           in_range(codepoint, 0x04B1, 0x04B1) ||
           in_range(codepoint, 0x04B3, 0x04B3) ||
           in_range(codepoint, 0x04B5, 0x04B5) ||
           in_range(codepoint, 0x04B7, 0x04B7) ||
           in_range(codepoint, 0x04B9, 0x04B9) ||
           in_range(codepoint, 0x04BB, 0x04BB) ||
           in_range(codepoint, 0x04BD, 0x04BD) ||
           in_range(codepoint, 0x04BF, 0x04BF) ||
           in_range(codepoint, 0x04C2, 0x04C2) ||
           in_range(codepoint, 0x04C4, 0x04C4) ||
           in_range(codepoint, 0x04C6, 0x04C6) ||
           in_range(codepoint, 0x04C8, 0x04C8) ||
           in_range(codepoint, 0x04CA, 0x04CA) ||
           in_range(codepoint, 0x04CC, 0x04CC) ||
           in_range(codepoint, 0x04CE, 0x04CF) ||
           in_range(codepoint, 0x04D1, 0x04D1) ||
           in_range(codepoint, 0x04D3, 0x04D3) ||
           in_range(codepoint, 0x04D5, 0x04D5) ||
           in_range(codepoint, 0x04D7, 0x04D7) ||
           in_range(codepoint, 0x04D9, 0x04D9) ||
           in_range(codepoint, 0x04DB, 0x04DB) ||
           in_range(codepoint, 0x04DD, 0x04DD) ||
           in_range(codepoint, 0x04DF, 0x04DF) ||
           in_range(codepoint, 0x04E1, 0x04E1) ||
           in_range(codepoint, 0x04E3, 0x04E3) ||
           in_range(codepoint, 0x04E5, 0x04E5) ||
           in_range(codepoint, 0x04E7, 0x04E7) ||
           in_range(codepoint, 0x04E9, 0x04E9) ||
           in_range(codepoint, 0x04EB, 0x04EB) ||
           in_range(codepoint, 0x04ED, 0x04ED) ||
           in_range(codepoint, 0x04EF, 0x04EF) ||
           in_range(codepoint, 0x04F1, 0x04F1) ||
           in_range(codepoint, 0x04F3, 0x04F3) ||
           in_range(codepoint, 0x04F5, 0x04F5) ||
           in_range(codepoint, 0x04F7, 0x04F7) ||
           in_range(codepoint, 0x04F9, 0x04F9) ||
           in_range(codepoint, 0x04FB, 0x04FB) ||
           in_range(codepoint, 0x04FD, 0x04FD) ||
           in_range(codepoint, 0x04FF, 0x04FF) ||
           in_range(codepoint, 0x0501, 0x0501) ||
           in_range(codepoint, 0x0503, 0x0503) ||
           in_range(codepoint, 0x0505, 0x0505) ||
           in_range(codepoint, 0x0507, 0x0507) ||
           in_range(codepoint, 0x0509, 0x0509) ||
           in_range(codepoint, 0x050B, 0x050B) ||
           in_range(codepoint, 0x050D, 0x050D) ||
           in_range(codepoint, 0x050F, 0x050F) ||
           in_range(codepoint, 0x0511, 0x0511) ||
           in_range(codepoint, 0x0513, 0x0513) ||
           in_range(codepoint, 0x0515, 0x0515) ||
           in_range(codepoint, 0x0517, 0x0517) ||
           in_range(codepoint, 0x0519, 0x0519) ||
           in_range(codepoint, 0x051B, 0x051B) ||
           in_range(codepoint, 0x051D, 0x051D) ||
           in_range(codepoint, 0x051F, 0x051F) ||
           in_range(codepoint, 0x0521, 0x0521) ||
           in_range(codepoint, 0x0523, 0x0523) ||
           in_range(codepoint, 0x0525, 0x0525) ||
           in_range(codepoint, 0x0527, 0x0527) ||
           in_range(codepoint, 0x0529, 0x0529) ||
           in_range(codepoint, 0x052B, 0x052B) ||
           in_range(codepoint, 0x052D, 0x052D) ||
           in_range(codepoint, 0x052F, 0x052F) ||
           in_range(codepoint, 0x0560, 0x0588) ||
           in_range(codepoint, 0x10D0, 0x10FA) ||
           in_range(codepoint, 0x10FD, 0x10FF) ||
           in_range(codepoint, 0x13F8, 0x13FD) ||
           in_range(codepoint, 0x1C80, 0x1C88) ||
           in_range(codepoint, 0x1D00, 0x1D2B) ||
           in_range(codepoint, 0x1D6B, 0x1D77) ||
           in_range(codepoint, 0x1D79, 0x1D9A) ||
           in_range(codepoint, 0x1E01, 0x1E01) ||
           in_range(codepoint, 0x1E03, 0x1E03) ||
           in_range(codepoint, 0x1E05, 0x1E05) ||
           in_range(codepoint, 0x1E07, 0x1E07) ||
           in_range(codepoint, 0x1E09, 0x1E09) ||
           in_range(codepoint, 0x1E0B, 0x1E0B) ||
           in_range(codepoint, 0x1E0D, 0x1E0D) ||
           in_range(codepoint, 0x1E0F, 0x1E0F) ||
           in_range(codepoint, 0x1E11, 0x1E11) ||
           in_range(codepoint, 0x1E13, 0x1E13) ||
           in_range(codepoint, 0x1E15, 0x1E15) ||
           in_range(codepoint, 0x1E17, 0x1E17) ||
           in_range(codepoint, 0x1E19, 0x1E19) ||
           in_range(codepoint, 0x1E1B, 0x1E1B) ||
           in_range(codepoint, 0x1E1D, 0x1E1D) ||
           in_range(codepoint, 0x1E1F, 0x1E1F) ||
           in_range(codepoint, 0x1E21, 0x1E21) ||
           in_range(codepoint, 0x1E23, 0x1E23) ||
           in_range(codepoint, 0x1E25, 0x1E25) ||
           in_range(codepoint, 0x1E27, 0x1E27) ||
           in_range(codepoint, 0x1E29, 0x1E29) ||
           in_range(codepoint, 0x1E2B, 0x1E2B) ||
           in_range(codepoint, 0x1E2D, 0x1E2D) ||
           in_range(codepoint, 0x1E2F, 0x1E2F) ||
           in_range(codepoint, 0x1E31, 0x1E31) ||
           in_range(codepoint, 0x1E33, 0x1E33) ||
           in_range(codepoint, 0x1E35, 0x1E35) ||
           in_range(codepoint, 0x1E37, 0x1E37) ||
           in_range(codepoint, 0x1E39, 0x1E39) ||
           in_range(codepoint, 0x1E3B, 0x1E3B) ||
           in_range(codepoint, 0x1E3D, 0x1E3D) ||
           in_range(codepoint, 0x1E3F, 0x1E3F) ||
           in_range(codepoint, 0x1E41, 0x1E41) ||
           in_range(codepoint, 0x1E43, 0x1E43) ||
           in_range(codepoint, 0x1E45, 0x1E45) ||
           in_range(codepoint, 0x1E47, 0x1E47) ||
           in_range(codepoint, 0x1E49, 0x1E49) ||
           in_range(codepoint, 0x1E4B, 0x1E4B) ||
           in_range(codepoint, 0x1E4D, 0x1E4D) ||
           in_range(codepoint, 0x1E4F, 0x1E4F) ||
           in_range(codepoint, 0x1E51, 0x1E51) ||
           in_range(codepoint, 0x1E53, 0x1E53) ||
           in_range(codepoint, 0x1E55, 0x1E55) ||
           in_range(codepoint, 0x1E57, 0x1E57) ||
           in_range(codepoint, 0x1E59, 0x1E59) ||
           in_range(codepoint, 0x1E5B, 0x1E5B) ||
           in_range(codepoint, 0x1E5D, 0x1E5D) ||
           in_range(codepoint, 0x1E5F, 0x1E5F) ||
           in_range(codepoint, 0x1E61, 0x1E61) ||
           in_range(codepoint, 0x1E63, 0x1E63) ||
           in_range(codepoint, 0x1E65, 0x1E65) ||
           in_range(codepoint, 0x1E67, 0x1E67) ||
           in_range(codepoint, 0x1E69, 0x1E69) ||
           in_range(codepoint, 0x1E6B, 0x1E6B) ||
           in_range(codepoint, 0x1E6D, 0x1E6D) ||
           in_range(codepoint, 0x1E6F, 0x1E6F) ||
           in_range(codepoint, 0x1E71, 0x1E71) ||
           in_range(codepoint, 0x1E73, 0x1E73) ||
           in_range(codepoint, 0x1E75, 0x1E75) ||
           in_range(codepoint, 0x1E77, 0x1E77) ||
           in_range(codepoint, 0x1E79, 0x1E79) ||
           in_range(codepoint, 0x1E7B, 0x1E7B) ||
           in_range(codepoint, 0x1E7D, 0x1E7D) ||
           in_range(codepoint, 0x1E7F, 0x1E7F) ||
           in_range(codepoint, 0x1E81, 0x1E81) ||
           in_range(codepoint, 0x1E83, 0x1E83) ||
           in_range(codepoint, 0x1E85, 0x1E85) ||
           in_range(codepoint, 0x1E87, 0x1E87) ||
           in_range(codepoint, 0x1E89, 0x1E89) ||
           in_range(codepoint, 0x1E8B, 0x1E8B) ||
           in_range(codepoint, 0x1E8D, 0x1E8D) ||
           in_range(codepoint, 0x1E8F, 0x1E8F) ||
           in_range(codepoint, 0x1E91, 0x1E91) ||
           in_range(codepoint, 0x1E93, 0x1E93) ||
           in_range(codepoint, 0x1E95, 0x1E9D) ||
           in_range(codepoint, 0x1E9F, 0x1E9F) ||
           in_range(codepoint, 0x1EA1, 0x1EA1) ||
           in_range(codepoint, 0x1EA3, 0x1EA3) ||
           in_range(codepoint, 0x1EA5, 0x1EA5) ||
           in_range(codepoint, 0x1EA7, 0x1EA7) ||
           in_range(codepoint, 0x1EA9, 0x1EA9) ||
           in_range(codepoint, 0x1EAB, 0x1EAB) ||
           in_range(codepoint, 0x1EAD, 0x1EAD) ||
           in_range(codepoint, 0x1EAF, 0x1EAF) ||
           in_range(codepoint, 0x1EB1, 0x1EB1) ||
           in_range(codepoint, 0x1EB3, 0x1EB3) ||
           in_range(codepoint, 0x1EB5, 0x1EB5) ||
           in_range(codepoint, 0x1EB7, 0x1EB7) ||
           in_range(codepoint, 0x1EB9, 0x1EB9) ||
           in_range(codepoint, 0x1EBB, 0x1EBB) ||
           in_range(codepoint, 0x1EBD, 0x1EBD) ||
           in_range(codepoint, 0x1EBF, 0x1EBF) ||
           in_range(codepoint, 0x1EC1, 0x1EC1) ||
           in_range(codepoint, 0x1EC3, 0x1EC3) ||
           in_range(codepoint, 0x1EC5, 0x1EC5) ||
           in_range(codepoint, 0x1EC7, 0x1EC7) ||
           in_range(codepoint, 0x1EC9, 0x1EC9) ||
           in_range(codepoint, 0x1ECB, 0x1ECB) ||
           in_range(codepoint, 0x1ECD, 0x1ECD) ||
           in_range(codepoint, 0x1ECF, 0x1ECF) ||
           in_range(codepoint, 0x1ED1, 0x1ED1) ||
           in_range(codepoint, 0x1ED3, 0x1ED3) ||
           in_range(codepoint, 0x1ED5, 0x1ED5) ||
           in_range(codepoint, 0x1ED7, 0x1ED7) ||
           in_range(codepoint, 0x1ED9, 0x1ED9) ||
           in_range(codepoint, 0x1EDB, 0x1EDB) ||
           in_range(codepoint, 0x1EDD, 0x1EDD) ||
           in_range(codepoint, 0x1EDF, 0x1EDF) ||
           in_range(codepoint, 0x1EE1, 0x1EE1) ||
           in_range(codepoint, 0x1EE3, 0x1EE3) ||
           in_range(codepoint, 0x1EE5, 0x1EE5) ||
           in_range(codepoint, 0x1EE7, 0x1EE7) ||
           in_range(codepoint, 0x1EE9, 0x1EE9) ||
           in_range(codepoint, 0x1EEB, 0x1EEB) ||
           in_range(codepoint, 0x1EED, 0x1EED) ||
           in_range(codepoint, 0x1EEF, 0x1EEF) ||
           in_range(codepoint, 0x1EF1, 0x1EF1) ||
           in_range(codepoint, 0x1EF3, 0x1EF3) ||
           in_range(codepoint, 0x1EF5, 0x1EF5) ||
           in_range(codepoint, 0x1EF7, 0x1EF7) ||
           in_range(codepoint, 0x1EF9, 0x1EF9) ||
           in_range(codepoint, 0x1EFB, 0x1EFB) ||
           in_range(codepoint, 0x1EFD, 0x1EFD) ||
           in_range(codepoint, 0x1EFF, 0x1F07) ||
           in_range(codepoint, 0x1F10, 0x1F15) ||
           in_range(codepoint, 0x1F20, 0x1F27) ||
           in_range(codepoint, 0x1F30, 0x1F37) ||
           in_range(codepoint, 0x1F40, 0x1F45) ||
           in_range(codepoint, 0x1F50, 0x1F57) ||
           in_range(codepoint, 0x1F60, 0x1F67) ||
           in_range(codepoint, 0x1F70, 0x1F7D) ||
           in_range(codepoint, 0x1F80, 0x1F87) ||
           in_range(codepoint, 0x1F90, 0x1F97) ||
           in_range(codepoint, 0x1FA0, 0x1FA7) ||
           in_range(codepoint, 0x1FB0, 0x1FB4) ||
           in_range(codepoint, 0x1FB6, 0x1FB7) ||
           in_range(codepoint, 0x1FBE, 0x1FBE) ||
           in_range(codepoint, 0x1FC2, 0x1FC4) ||
           in_range(codepoint, 0x1FC6, 0x1FC7) ||
           in_range(codepoint, 0x1FD0, 0x1FD3) ||
           in_range(codepoint, 0x1FD6, 0x1FD7) ||
           in_range(codepoint, 0x1FE0, 0x1FE7) ||
           in_range(codepoint, 0x1FF2, 0x1FF4) ||
           in_range(codepoint, 0x1FF6, 0x1FF7) ||
           in_range(codepoint, 0x210A, 0x210A) ||
           in_range(codepoint, 0x210E, 0x210F) ||
           in_range(codepoint, 0x2113, 0x2113) ||
           in_range(codepoint, 0x212F, 0x212F) ||
           in_range(codepoint, 0x2134, 0x2134) ||
           in_range(codepoint, 0x2139, 0x2139) ||
           in_range(codepoint, 0x213C, 0x213D) ||
           in_range(codepoint, 0x2146, 0x2149) ||
           in_range(codepoint, 0x214E, 0x214E) ||
           in_range(codepoint, 0x2184, 0x2184) ||
           in_range(codepoint, 0x2C30, 0x2C5E) ||
           in_range(codepoint, 0x2C61, 0x2C61) ||
           in_range(codepoint, 0x2C65, 0x2C66) ||
           in_range(codepoint, 0x2C68, 0x2C68) ||
           in_range(codepoint, 0x2C6A, 0x2C6A) ||
           in_range(codepoint, 0x2C6C, 0x2C6C) ||
           in_range(codepoint, 0x2C71, 0x2C71) ||
           in_range(codepoint, 0x2C73, 0x2C74) ||
           in_range(codepoint, 0x2C76, 0x2C7B) ||
           in_range(codepoint, 0x2C81, 0x2C81) ||
           in_range(codepoint, 0x2C83, 0x2C83) ||
           in_range(codepoint, 0x2C85, 0x2C85) ||
           in_range(codepoint, 0x2C87, 0x2C87) ||
           in_range(codepoint, 0x2C89, 0x2C89) ||
           in_range(codepoint, 0x2C8B, 0x2C8B) ||
           in_range(codepoint, 0x2C8D, 0x2C8D) ||
           in_range(codepoint, 0x2C8F, 0x2C8F) ||
           in_range(codepoint, 0x2C91, 0x2C91) ||
           in_range(codepoint, 0x2C93, 0x2C93) ||
           in_range(codepoint, 0x2C95, 0x2C95) ||
           in_range(codepoint, 0x2C97, 0x2C97) ||
           in_range(codepoint, 0x2C99, 0x2C99) ||
           in_range(codepoint, 0x2C9B, 0x2C9B) ||
           in_range(codepoint, 0x2C9D, 0x2C9D) ||
           in_range(codepoint, 0x2C9F, 0x2C9F) ||
           in_range(codepoint, 0x2CA1, 0x2CA1) ||
           in_range(codepoint, 0x2CA3, 0x2CA3) ||
           in_range(codepoint, 0x2CA5, 0x2CA5) ||
           in_range(codepoint, 0x2CA7, 0x2CA7) ||
           in_range(codepoint, 0x2CA9, 0x2CA9) ||
           in_range(codepoint, 0x2CAB, 0x2CAB) ||
           in_range(codepoint, 0x2CAD, 0x2CAD) ||
           in_range(codepoint, 0x2CAF, 0x2CAF) ||
           in_range(codepoint, 0x2CB1, 0x2CB1) ||
           in_range(codepoint, 0x2CB3, 0x2CB3) ||
           in_range(codepoint, 0x2CB5, 0x2CB5) ||
           in_range(codepoint, 0x2CB7, 0x2CB7) ||
           in_range(codepoint, 0x2CB9, 0x2CB9) ||
           in_range(codepoint, 0x2CBB, 0x2CBB) ||
           in_range(codepoint, 0x2CBD, 0x2CBD) ||
           in_range(codepoint, 0x2CBF, 0x2CBF) ||
           in_range(codepoint, 0x2CC1, 0x2CC1) ||
           in_range(codepoint, 0x2CC3, 0x2CC3) ||
           in_range(codepoint, 0x2CC5, 0x2CC5) ||
           in_range(codepoint, 0x2CC7, 0x2CC7) ||
           in_range(codepoint, 0x2CC9, 0x2CC9) ||
           in_range(codepoint, 0x2CCB, 0x2CCB) ||
           in_range(codepoint, 0x2CCD, 0x2CCD) ||
           in_range(codepoint, 0x2CCF, 0x2CCF) ||
           in_range(codepoint, 0x2CD1, 0x2CD1) ||
           in_range(codepoint, 0x2CD3, 0x2CD3) ||
           in_range(codepoint, 0x2CD5, 0x2CD5) ||
           in_range(codepoint, 0x2CD7, 0x2CD7) ||
           in_range(codepoint, 0x2CD9, 0x2CD9) ||
           in_range(codepoint, 0x2CDB, 0x2CDB) ||
           in_range(codepoint, 0x2CDD, 0x2CDD) ||
           in_range(codepoint, 0x2CDF, 0x2CDF) ||
           in_range(codepoint, 0x2CE1, 0x2CE1) ||
           in_range(codepoint, 0x2CE3, 0x2CE4) ||
           in_range(codepoint, 0x2CEC, 0x2CEC) ||
           in_range(codepoint, 0x2CEE, 0x2CEE) ||
           in_range(codepoint, 0x2CF3, 0x2CF3) ||
           in_range(codepoint, 0x2D00, 0x2D25) ||
           in_range(codepoint, 0x2D27, 0x2D27) ||
           in_range(codepoint, 0x2D2D, 0x2D2D) ||
           in_range(codepoint, 0xA641, 0xA641) ||
           in_range(codepoint, 0xA643, 0xA643) ||
           in_range(codepoint, 0xA645, 0xA645) ||
           in_range(codepoint, 0xA647, 0xA647) ||
           in_range(codepoint, 0xA649, 0xA649) ||
           in_range(codepoint, 0xA64B, 0xA64B) ||
           in_range(codepoint, 0xA64D, 0xA64D) ||
           in_range(codepoint, 0xA64F, 0xA64F) ||
           in_range(codepoint, 0xA651, 0xA651) ||
           in_range(codepoint, 0xA653, 0xA653) ||
           in_range(codepoint, 0xA655, 0xA655) ||
           in_range(codepoint, 0xA657, 0xA657) ||
           in_range(codepoint, 0xA659, 0xA659) ||
           in_range(codepoint, 0xA65B, 0xA65B) ||
           in_range(codepoint, 0xA65D, 0xA65D) ||
           in_range(codepoint, 0xA65F, 0xA65F) ||
           in_range(codepoint, 0xA661, 0xA661) ||
           in_range(codepoint, 0xA663, 0xA663) ||
           in_range(codepoint, 0xA665, 0xA665) ||
           in_range(codepoint, 0xA667, 0xA667) ||
           in_range(codepoint, 0xA669, 0xA669) ||
           in_range(codepoint, 0xA66B, 0xA66B) ||
           in_range(codepoint, 0xA66D, 0xA66D) ||
           in_range(codepoint, 0xA681, 0xA681) ||
           in_range(codepoint, 0xA683, 0xA683) ||
           in_range(codepoint, 0xA685, 0xA685) ||
           in_range(codepoint, 0xA687, 0xA687) ||
           in_range(codepoint, 0xA689, 0xA689) ||
           in_range(codepoint, 0xA68B, 0xA68B) ||
           in_range(codepoint, 0xA68D, 0xA68D) ||
           in_range(codepoint, 0xA68F, 0xA68F) ||
           in_range(codepoint, 0xA691, 0xA691) ||
           in_range(codepoint, 0xA693, 0xA693) ||
           in_range(codepoint, 0xA695, 0xA695) ||
           in_range(codepoint, 0xA697, 0xA697) ||
           in_range(codepoint, 0xA699, 0xA699) ||
           in_range(codepoint, 0xA69B, 0xA69B) ||
           in_range(codepoint, 0xA723, 0xA723) ||
           in_range(codepoint, 0xA725, 0xA725) ||
           in_range(codepoint, 0xA727, 0xA727) ||
           in_range(codepoint, 0xA729, 0xA729) ||
           in_range(codepoint, 0xA72B, 0xA72B) ||
           in_range(codepoint, 0xA72D, 0xA72D) ||
           in_range(codepoint, 0xA72F, 0xA731) ||
           in_range(codepoint, 0xA733, 0xA733) ||
           in_range(codepoint, 0xA735, 0xA735) ||
           in_range(codepoint, 0xA737, 0xA737) ||
           in_range(codepoint, 0xA739, 0xA739) ||
           in_range(codepoint, 0xA73B, 0xA73B) ||
           in_range(codepoint, 0xA73D, 0xA73D) ||
           in_range(codepoint, 0xA73F, 0xA73F) ||
           in_range(codepoint, 0xA741, 0xA741) ||
           in_range(codepoint, 0xA743, 0xA743) ||
           in_range(codepoint, 0xA745, 0xA745) ||
           in_range(codepoint, 0xA747, 0xA747) ||
           in_range(codepoint, 0xA749, 0xA749) ||
           in_range(codepoint, 0xA74B, 0xA74B) ||
           in_range(codepoint, 0xA74D, 0xA74D) ||
           in_range(codepoint, 0xA74F, 0xA74F) ||
           in_range(codepoint, 0xA751, 0xA751) ||
           in_range(codepoint, 0xA753, 0xA753) ||
           in_range(codepoint, 0xA755, 0xA755) ||
           in_range(codepoint, 0xA757, 0xA757) ||
           in_range(codepoint, 0xA759, 0xA759) ||
           in_range(codepoint, 0xA75B, 0xA75B) ||
           in_range(codepoint, 0xA75D, 0xA75D) ||
           in_range(codepoint, 0xA75F, 0xA75F) ||
           in_range(codepoint, 0xA761, 0xA761) ||
           in_range(codepoint, 0xA763, 0xA763) ||
           in_range(codepoint, 0xA765, 0xA765) ||
           in_range(codepoint, 0xA767, 0xA767) ||
           in_range(codepoint, 0xA769, 0xA769) ||
           in_range(codepoint, 0xA76B, 0xA76B) ||
           in_range(codepoint, 0xA76D, 0xA76D) ||
           in_range(codepoint, 0xA76F, 0xA76F) ||
           in_range(codepoint, 0xA771, 0xA778) ||
           in_range(codepoint, 0xA77A, 0xA77A) ||
           in_range(codepoint, 0xA77C, 0xA77C) ||
           in_range(codepoint, 0xA77F, 0xA77F) ||
           in_range(codepoint, 0xA781, 0xA781) ||
           in_range(codepoint, 0xA783, 0xA783) ||
           in_range(codepoint, 0xA785, 0xA785) ||
           in_range(codepoint, 0xA787, 0xA787) ||
           in_range(codepoint, 0xA78C, 0xA78C) ||
           in_range(codepoint, 0xA78E, 0xA78E) ||
           in_range(codepoint, 0xA791, 0xA791) ||
           in_range(codepoint, 0xA793, 0xA795) ||
           in_range(codepoint, 0xA797, 0xA797) ||
           in_range(codepoint, 0xA799, 0xA799) ||
           in_range(codepoint, 0xA79B, 0xA79B) ||
           in_range(codepoint, 0xA79D, 0xA79D) ||
           in_range(codepoint, 0xA79F, 0xA79F) ||
           in_range(codepoint, 0xA7A1, 0xA7A1) ||
           in_range(codepoint, 0xA7A3, 0xA7A3) ||
           in_range(codepoint, 0xA7A5, 0xA7A5) ||
           in_range(codepoint, 0xA7A7, 0xA7A7) ||
           in_range(codepoint, 0xA7A9, 0xA7A9) ||
           in_range(codepoint, 0xA7AF, 0xA7AF) ||
           in_range(codepoint, 0xA7B5, 0xA7B5) ||
           in_range(codepoint, 0xA7B7, 0xA7B7) ||
           in_range(codepoint, 0xA7B9, 0xA7B9) ||
           in_range(codepoint, 0xA7BB, 0xA7BB) ||
           in_range(codepoint, 0xA7BD, 0xA7BD) ||
           in_range(codepoint, 0xA7BF, 0xA7BF) ||
           in_range(codepoint, 0xA7C3, 0xA7C3) ||
           in_range(codepoint, 0xA7C8, 0xA7C8) ||
           in_range(codepoint, 0xA7CA, 0xA7CA) ||
           in_range(codepoint, 0xA7F6, 0xA7F6) ||
           in_range(codepoint, 0xA7FA, 0xA7FA) ||
           in_range(codepoint, 0xAB30, 0xAB5A) ||
           in_range(codepoint, 0xAB60, 0xAB68) ||
           in_range(codepoint, 0xAB70, 0xABBF) ||
           in_range(codepoint, 0xFB00, 0xFB06) ||
           in_range(codepoint, 0xFB13, 0xFB17) ||
           in_range(codepoint, 0xFF41, 0xFF5A) ||
           in_range(codepoint, 0x10428, 0x1044F) ||
           in_range(codepoint, 0x104D8, 0x104FB) ||
           in_range(codepoint, 0x10CC0, 0x10CF2) ||
           in_range(codepoint, 0x118C0, 0x118DF) ||
           in_range(codepoint, 0x16E60, 0x16E7F) ||
           in_range(codepoint, 0x1D41A, 0x1D433) ||
           in_range(codepoint, 0x1D44E, 0x1D454) ||
           in_range(codepoint, 0x1D456, 0x1D467) ||
           in_range(codepoint, 0x1D482, 0x1D49B) ||
           in_range(codepoint, 0x1D4B6, 0x1D4B9) ||
           in_range(codepoint, 0x1D4BB, 0x1D4BB) ||
           in_range(codepoint, 0x1D4BD, 0x1D4C3) ||
           in_range(codepoint, 0x1D4C5, 0x1D4CF) ||
           in_range(codepoint, 0x1D4EA, 0x1D503) ||
           in_range(codepoint, 0x1D51E, 0x1D537) ||
           in_range(codepoint, 0x1D552, 0x1D56B) ||
           in_range(codepoint, 0x1D586, 0x1D59F) ||
           in_range(codepoint, 0x1D5BA, 0x1D5D3) ||
           in_range(codepoint, 0x1D5EE, 0x1D607) ||
           in_range(codepoint, 0x1D622, 0x1D63B) ||
           in_range(codepoint, 0x1D656, 0x1D66F) ||
           in_range(codepoint, 0x1D68A, 0x1D6A5) ||
           in_range(codepoint, 0x1D6C2, 0x1D6DA) ||
           in_range(codepoint, 0x1D6DC, 0x1D6E1) ||
           in_range(codepoint, 0x1D6FC, 0x1D714) ||
           in_range(codepoint, 0x1D716, 0x1D71B) ||
           in_range(codepoint, 0x1D736, 0x1D74E) ||
           in_range(codepoint, 0x1D750, 0x1D755) ||
           in_range(codepoint, 0x1D770, 0x1D788) ||
           in_range(codepoint, 0x1D78A, 0x1D78F) ||
           in_range(codepoint, 0x1D7AA, 0x1D7C2) ||
           in_range(codepoint, 0x1D7C4, 0x1D7C9) ||
           in_range(codepoint, 0x1D7CB, 0x1D7CB) ||
           in_range(codepoint, 0x1E922, 0x1E943);
}

inline bool text::unicode_category_lm(datum_type codepoint)
{
    return in_range(codepoint, 0x02B0, 0x02C1) ||
           in_range(codepoint, 0x02C6, 0x02D1) ||
           in_range(codepoint, 0x02E0, 0x02E4) ||
           in_range(codepoint, 0x02EC, 0x02EC) ||
           in_range(codepoint, 0x02EE, 0x02EE) ||
           in_range(codepoint, 0x0374, 0x0374) ||
           in_range(codepoint, 0x037A, 0x037A) ||
           in_range(codepoint, 0x0559, 0x0559) ||
           in_range(codepoint, 0x0640, 0x0640) ||
           in_range(codepoint, 0x06E5, 0x06E6) ||
           in_range(codepoint, 0x07F4, 0x07F5) ||
           in_range(codepoint, 0x07FA, 0x07FA) ||
           in_range(codepoint, 0x081A, 0x081A) ||
           in_range(codepoint, 0x0824, 0x0824) ||
           in_range(codepoint, 0x0828, 0x0828) ||
           in_range(codepoint, 0x0971, 0x0971) ||
           in_range(codepoint, 0x0E46, 0x0E46) ||
           in_range(codepoint, 0x0EC6, 0x0EC6) ||
           in_range(codepoint, 0x10FC, 0x10FC) ||
           in_range(codepoint, 0x17D7, 0x17D7) ||
           in_range(codepoint, 0x1843, 0x1843) ||
           in_range(codepoint, 0x1AA7, 0x1AA7) ||
           in_range(codepoint, 0x1C78, 0x1C7D) ||
           in_range(codepoint, 0x1D2C, 0x1D6A) ||
           in_range(codepoint, 0x1D78, 0x1D78) ||
           in_range(codepoint, 0x1D9B, 0x1DBF) ||
           in_range(codepoint, 0x2071, 0x2071) ||
           in_range(codepoint, 0x207F, 0x207F) ||
           in_range(codepoint, 0x2090, 0x209C) ||
           in_range(codepoint, 0x2C7C, 0x2C7D) ||
           in_range(codepoint, 0x2D6F, 0x2D6F) ||
           in_range(codepoint, 0x2E2F, 0x2E2F) ||
           in_range(codepoint, 0x3005, 0x3005) ||
           in_range(codepoint, 0x3031, 0x3035) ||
           in_range(codepoint, 0x303B, 0x303B) ||
           in_range(codepoint, 0x309D, 0x309E) ||
           in_range(codepoint, 0x30FC, 0x30FE) ||
           in_range(codepoint, 0xA015, 0xA015) ||
           in_range(codepoint, 0xA4F8, 0xA4FD) ||
           in_range(codepoint, 0xA60C, 0xA60C) ||
           in_range(codepoint, 0xA67F, 0xA67F) ||
           in_range(codepoint, 0xA69C, 0xA69D) ||
           in_range(codepoint, 0xA717, 0xA71F) ||
           in_range(codepoint, 0xA770, 0xA770) ||
           in_range(codepoint, 0xA788, 0xA788) ||
           in_range(codepoint, 0xA7F8, 0xA7F9) ||
           in_range(codepoint, 0xA9CF, 0xA9CF) ||
           in_range(codepoint, 0xA9E6, 0xA9E6) ||
           in_range(codepoint, 0xAA70, 0xAA70) ||
           in_range(codepoint, 0xAADD, 0xAADD) ||
           in_range(codepoint, 0xAAF3, 0xAAF4) ||
           in_range(codepoint, 0xAB5C, 0xAB5F) ||
           in_range(codepoint, 0xAB69, 0xAB69) ||
           in_range(codepoint, 0xFF70, 0xFF70) ||
           in_range(codepoint, 0xFF9E, 0xFF9F) ||
           in_range(codepoint, 0x16B40, 0x16B43) ||
           in_range(codepoint, 0x16F93, 0x16F9F) ||
           in_range(codepoint, 0x16FE0, 0x16FE1) ||
           in_range(codepoint, 0x16FE3, 0x16FE3) ||
           in_range(codepoint, 0x1E137, 0x1E13D) ||
           in_range(codepoint, 0x1E94B, 0x1E94B);
}

inline bool text::unicode_category_lo(datum_type codepoint)
{
    return in_range(codepoint, 0x00AA, 0x00AA) ||
           in_range(codepoint, 0x00BA, 0x00BA) ||
           in_range(codepoint, 0x01BB, 0x01BB) ||
           in_range(codepoint, 0x01C0, 0x01C3) ||
           in_range(codepoint, 0x0294, 0x0294) ||
           in_range(codepoint, 0x05D0, 0x05EA) ||
           in_range(codepoint, 0x05EF, 0x05F2) ||
           in_range(codepoint, 0x0620, 0x063F) ||
           in_range(codepoint, 0x0641, 0x064A) ||
           in_range(codepoint, 0x066E, 0x066F) ||
           in_range(codepoint, 0x0671, 0x06D3) ||
           in_range(codepoint, 0x06D5, 0x06D5) ||
           in_range(codepoint, 0x06EE, 0x06EF) ||
           in_range(codepoint, 0x06FA, 0x06FC) ||
           in_range(codepoint, 0x06FF, 0x06FF) ||
           in_range(codepoint, 0x0710, 0x0710) ||
           in_range(codepoint, 0x0712, 0x072F) ||
           in_range(codepoint, 0x074D, 0x07A5) ||
           in_range(codepoint, 0x07B1, 0x07B1) ||
           in_range(codepoint, 0x07CA, 0x07EA) ||
           in_range(codepoint, 0x0800, 0x0815) ||
           in_range(codepoint, 0x0840, 0x0858) ||
           in_range(codepoint, 0x0860, 0x086A) ||
           in_range(codepoint, 0x08A0, 0x08B4) ||
           in_range(codepoint, 0x08B6, 0x08C7) ||
           in_range(codepoint, 0x0904, 0x0939) ||
           in_range(codepoint, 0x093D, 0x093D) ||
           in_range(codepoint, 0x0950, 0x0950) ||
           in_range(codepoint, 0x0958, 0x0961) ||
           in_range(codepoint, 0x0972, 0x0980) ||
           in_range(codepoint, 0x0985, 0x098C) ||
           in_range(codepoint, 0x098F, 0x0990) ||
           in_range(codepoint, 0x0993, 0x09A8) ||
           in_range(codepoint, 0x09AA, 0x09B0) ||
           in_range(codepoint, 0x09B2, 0x09B2) ||
           in_range(codepoint, 0x09B6, 0x09B9) ||
           in_range(codepoint, 0x09BD, 0x09BD) ||
           in_range(codepoint, 0x09CE, 0x09CE) ||
           in_range(codepoint, 0x09DC, 0x09DD) ||
           in_range(codepoint, 0x09DF, 0x09E1) ||
           in_range(codepoint, 0x09F0, 0x09F1) ||
           in_range(codepoint, 0x09FC, 0x09FC) ||
           in_range(codepoint, 0x0A05, 0x0A0A) ||
           in_range(codepoint, 0x0A0F, 0x0A10) ||
           in_range(codepoint, 0x0A13, 0x0A28) ||
           in_range(codepoint, 0x0A2A, 0x0A30) ||
           in_range(codepoint, 0x0A32, 0x0A33) ||
           in_range(codepoint, 0x0A35, 0x0A36) ||
           in_range(codepoint, 0x0A38, 0x0A39) ||
           in_range(codepoint, 0x0A59, 0x0A5C) ||
           in_range(codepoint, 0x0A5E, 0x0A5E) ||
           in_range(codepoint, 0x0A72, 0x0A74) ||
           in_range(codepoint, 0x0A85, 0x0A8D) ||
           in_range(codepoint, 0x0A8F, 0x0A91) ||
           in_range(codepoint, 0x0A93, 0x0AA8) ||
           in_range(codepoint, 0x0AAA, 0x0AB0) ||
           in_range(codepoint, 0x0AB2, 0x0AB3) ||
           in_range(codepoint, 0x0AB5, 0x0AB9) ||
           in_range(codepoint, 0x0ABD, 0x0ABD) ||
           in_range(codepoint, 0x0AD0, 0x0AD0) ||
           in_range(codepoint, 0x0AE0, 0x0AE1) ||
           in_range(codepoint, 0x0AF9, 0x0AF9) ||
           in_range(codepoint, 0x0B05, 0x0B0C) ||
           in_range(codepoint, 0x0B0F, 0x0B10) ||
           in_range(codepoint, 0x0B13, 0x0B28) ||
           in_range(codepoint, 0x0B2A, 0x0B30) ||
           in_range(codepoint, 0x0B32, 0x0B33) ||
           in_range(codepoint, 0x0B35, 0x0B39) ||
           in_range(codepoint, 0x0B3D, 0x0B3D) ||
           in_range(codepoint, 0x0B5C, 0x0B5D) ||
           in_range(codepoint, 0x0B5F, 0x0B61) ||
           in_range(codepoint, 0x0B71, 0x0B71) ||
           in_range(codepoint, 0x0B83, 0x0B83) ||
           in_range(codepoint, 0x0B85, 0x0B8A) ||
           in_range(codepoint, 0x0B8E, 0x0B90) ||
           in_range(codepoint, 0x0B92, 0x0B95) ||
           in_range(codepoint, 0x0B99, 0x0B9A) ||
           in_range(codepoint, 0x0B9C, 0x0B9C) ||
           in_range(codepoint, 0x0B9E, 0x0B9F) ||
           in_range(codepoint, 0x0BA3, 0x0BA4) ||
           in_range(codepoint, 0x0BA8, 0x0BAA) ||
           in_range(codepoint, 0x0BAE, 0x0BB9) ||
           in_range(codepoint, 0x0BD0, 0x0BD0) ||
           in_range(codepoint, 0x0C05, 0x0C0C) ||
           in_range(codepoint, 0x0C0E, 0x0C10) ||
           in_range(codepoint, 0x0C12, 0x0C28) ||
           in_range(codepoint, 0x0C2A, 0x0C39) ||
           in_range(codepoint, 0x0C3D, 0x0C3D) ||
           in_range(codepoint, 0x0C58, 0x0C5A) ||
           in_range(codepoint, 0x0C60, 0x0C61) ||
           in_range(codepoint, 0x0C80, 0x0C80) ||
           in_range(codepoint, 0x0C85, 0x0C8C) ||
           in_range(codepoint, 0x0C8E, 0x0C90) ||
           in_range(codepoint, 0x0C92, 0x0CA8) ||
           in_range(codepoint, 0x0CAA, 0x0CB3) ||
           in_range(codepoint, 0x0CB5, 0x0CB9) ||
           in_range(codepoint, 0x0CBD, 0x0CBD) ||
           in_range(codepoint, 0x0CDE, 0x0CDE) ||
           in_range(codepoint, 0x0CE0, 0x0CE1) ||
           in_range(codepoint, 0x0CF1, 0x0CF2) ||
           in_range(codepoint, 0x0D04, 0x0D0C) ||
           in_range(codepoint, 0x0D0E, 0x0D10) ||
           in_range(codepoint, 0x0D12, 0x0D3A) ||
           in_range(codepoint, 0x0D3D, 0x0D3D) ||
           in_range(codepoint, 0x0D4E, 0x0D4E) ||
           in_range(codepoint, 0x0D54, 0x0D56) ||
           in_range(codepoint, 0x0D5F, 0x0D61) ||
           in_range(codepoint, 0x0D7A, 0x0D7F) ||
           in_range(codepoint, 0x0D85, 0x0D96) ||
           in_range(codepoint, 0x0D9A, 0x0DB1) ||
           in_range(codepoint, 0x0DB3, 0x0DBB) ||
           in_range(codepoint, 0x0DBD, 0x0DBD) ||
           in_range(codepoint, 0x0DC0, 0x0DC6) ||
           in_range(codepoint, 0x0E01, 0x0E30) ||
           in_range(codepoint, 0x0E32, 0x0E33) ||
           in_range(codepoint, 0x0E40, 0x0E45) ||
           in_range(codepoint, 0x0E81, 0x0E82) ||
           in_range(codepoint, 0x0E84, 0x0E84) ||
           in_range(codepoint, 0x0E86, 0x0E8A) ||
           in_range(codepoint, 0x0E8C, 0x0EA3) ||
           in_range(codepoint, 0x0EA5, 0x0EA5) ||
           in_range(codepoint, 0x0EA7, 0x0EB0) ||
           in_range(codepoint, 0x0EB2, 0x0EB3) ||
           in_range(codepoint, 0x0EBD, 0x0EBD) ||
           in_range(codepoint, 0x0EC0, 0x0EC4) ||
           in_range(codepoint, 0x0EDC, 0x0EDF) ||
           in_range(codepoint, 0x0F00, 0x0F00) ||
           in_range(codepoint, 0x0F40, 0x0F47) ||
           in_range(codepoint, 0x0F49, 0x0F6C) ||
           in_range(codepoint, 0x0F88, 0x0F8C) ||
           in_range(codepoint, 0x1000, 0x102A) ||
           in_range(codepoint, 0x103F, 0x103F) ||
           in_range(codepoint, 0x1050, 0x1055) ||
           in_range(codepoint, 0x105A, 0x105D) ||
           in_range(codepoint, 0x1061, 0x1061) ||
           in_range(codepoint, 0x1065, 0x1066) ||
           in_range(codepoint, 0x106E, 0x1070) ||
           in_range(codepoint, 0x1075, 0x1081) ||
           in_range(codepoint, 0x108E, 0x108E) ||
           in_range(codepoint, 0x1100, 0x1248) ||
           in_range(codepoint, 0x124A, 0x124D) ||
           in_range(codepoint, 0x1250, 0x1256) ||
           in_range(codepoint, 0x1258, 0x1258) ||
           in_range(codepoint, 0x125A, 0x125D) ||
           in_range(codepoint, 0x1260, 0x1288) ||
           in_range(codepoint, 0x128A, 0x128D) ||
           in_range(codepoint, 0x1290, 0x12B0) ||
           in_range(codepoint, 0x12B2, 0x12B5) ||
           in_range(codepoint, 0x12B8, 0x12BE) ||
           in_range(codepoint, 0x12C0, 0x12C0) ||
           in_range(codepoint, 0x12C2, 0x12C5) ||
           in_range(codepoint, 0x12C8, 0x12D6) ||
           in_range(codepoint, 0x12D8, 0x1310) ||
           in_range(codepoint, 0x1312, 0x1315) ||
           in_range(codepoint, 0x1318, 0x135A) ||
           in_range(codepoint, 0x1380, 0x138F) ||
           in_range(codepoint, 0x1401, 0x166C) ||
           in_range(codepoint, 0x166F, 0x167F) ||
           in_range(codepoint, 0x1681, 0x169A) ||
           in_range(codepoint, 0x16A0, 0x16EA) ||
           in_range(codepoint, 0x16F1, 0x16F8) ||
           in_range(codepoint, 0x1700, 0x170C) ||
           in_range(codepoint, 0x170E, 0x1711) ||
           in_range(codepoint, 0x1720, 0x1731) ||
           in_range(codepoint, 0x1740, 0x1751) ||
           in_range(codepoint, 0x1760, 0x176C) ||
           in_range(codepoint, 0x176E, 0x1770) ||
           in_range(codepoint, 0x1780, 0x17B3) ||
           in_range(codepoint, 0x17DC, 0x17DC) ||
           in_range(codepoint, 0x1820, 0x1842) ||
           in_range(codepoint, 0x1844, 0x1878) ||
           in_range(codepoint, 0x1880, 0x1884) ||
           in_range(codepoint, 0x1887, 0x18A8) ||
           in_range(codepoint, 0x18AA, 0x18AA) ||
           in_range(codepoint, 0x18B0, 0x18F5) ||
           in_range(codepoint, 0x1900, 0x191E) ||
           in_range(codepoint, 0x1950, 0x196D) ||
           in_range(codepoint, 0x1970, 0x1974) ||
           in_range(codepoint, 0x1980, 0x19AB) ||
           in_range(codepoint, 0x19B0, 0x19C9) ||
           in_range(codepoint, 0x1A00, 0x1A16) ||
           in_range(codepoint, 0x1A20, 0x1A54) ||
           in_range(codepoint, 0x1B05, 0x1B33) ||
           in_range(codepoint, 0x1B45, 0x1B4B) ||
           in_range(codepoint, 0x1B83, 0x1BA0) ||
           in_range(codepoint, 0x1BAE, 0x1BAF) ||
           in_range(codepoint, 0x1BBA, 0x1BE5) ||
           in_range(codepoint, 0x1C00, 0x1C23) ||
           in_range(codepoint, 0x1C4D, 0x1C4F) ||
           in_range(codepoint, 0x1C5A, 0x1C77) ||
           in_range(codepoint, 0x1CE9, 0x1CEC) ||
           in_range(codepoint, 0x1CEE, 0x1CF3) ||
           in_range(codepoint, 0x1CF5, 0x1CF6) ||
           in_range(codepoint, 0x1CFA, 0x1CFA) ||
           in_range(codepoint, 0x2135, 0x2138) ||
           in_range(codepoint, 0x2D30, 0x2D67) ||
           in_range(codepoint, 0x2D80, 0x2D96) ||
           in_range(codepoint, 0x2DA0, 0x2DA6) ||
           in_range(codepoint, 0x2DA8, 0x2DAE) ||
           in_range(codepoint, 0x2DB0, 0x2DB6) ||
           in_range(codepoint, 0x2DB8, 0x2DBE) ||
           in_range(codepoint, 0x2DC0, 0x2DC6) ||
           in_range(codepoint, 0x2DC8, 0x2DCE) ||
           in_range(codepoint, 0x2DD0, 0x2DD6) ||
           in_range(codepoint, 0x2DD8, 0x2DDE) ||
           in_range(codepoint, 0x3006, 0x3006) ||
           in_range(codepoint, 0x303C, 0x303C) ||
           in_range(codepoint, 0x3041, 0x3096) ||
           in_range(codepoint, 0x309F, 0x309F) ||
           in_range(codepoint, 0x30A1, 0x30FA) ||
           in_range(codepoint, 0x30FF, 0x30FF) ||
           in_range(codepoint, 0x3105, 0x312F) ||
           in_range(codepoint, 0x3131, 0x318E) ||
           in_range(codepoint, 0x31A0, 0x31BF) ||
           in_range(codepoint, 0x31F0, 0x31FF) ||
           in_range(codepoint, 0x3400, 0x3400) ||
           in_range(codepoint, 0x4DBF, 0x4DBF) ||
           in_range(codepoint, 0x4E00, 0x4E00) ||
           in_range(codepoint, 0x9FFC, 0x9FFC) ||
           in_range(codepoint, 0xA000, 0xA014) ||
           in_range(codepoint, 0xA016, 0xA48C) ||
           in_range(codepoint, 0xA4D0, 0xA4F7) ||
           in_range(codepoint, 0xA500, 0xA60B) ||
           in_range(codepoint, 0xA610, 0xA61F) ||
           in_range(codepoint, 0xA62A, 0xA62B) ||
           in_range(codepoint, 0xA66E, 0xA66E) ||
           in_range(codepoint, 0xA6A0, 0xA6E5) ||
           in_range(codepoint, 0xA78F, 0xA78F) ||
           in_range(codepoint, 0xA7F7, 0xA7F7) ||
           in_range(codepoint, 0xA7FB, 0xA801) ||
           in_range(codepoint, 0xA803, 0xA805) ||
           in_range(codepoint, 0xA807, 0xA80A) ||
           in_range(codepoint, 0xA80C, 0xA822) ||
           in_range(codepoint, 0xA840, 0xA873) ||
           in_range(codepoint, 0xA882, 0xA8B3) ||
           in_range(codepoint, 0xA8F2, 0xA8F7) ||
           in_range(codepoint, 0xA8FB, 0xA8FB) ||
           in_range(codepoint, 0xA8FD, 0xA8FE) ||
           in_range(codepoint, 0xA90A, 0xA925) ||
           in_range(codepoint, 0xA930, 0xA946) ||
           in_range(codepoint, 0xA960, 0xA97C) ||
           in_range(codepoint, 0xA984, 0xA9B2) ||
           in_range(codepoint, 0xA9E0, 0xA9E4) ||
           in_range(codepoint, 0xA9E7, 0xA9EF) ||
           in_range(codepoint, 0xA9FA, 0xA9FE) ||
           in_range(codepoint, 0xAA00, 0xAA28) ||
           in_range(codepoint, 0xAA40, 0xAA42) ||
           in_range(codepoint, 0xAA44, 0xAA4B) ||
           in_range(codepoint, 0xAA60, 0xAA6F) ||
           in_range(codepoint, 0xAA71, 0xAA76) ||
           in_range(codepoint, 0xAA7A, 0xAA7A) ||
           in_range(codepoint, 0xAA7E, 0xAAAF) ||
           in_range(codepoint, 0xAAB1, 0xAAB1) ||
           in_range(codepoint, 0xAAB5, 0xAAB6) ||
           in_range(codepoint, 0xAAB9, 0xAABD) ||
           in_range(codepoint, 0xAAC0, 0xAAC0) ||
           in_range(codepoint, 0xAAC2, 0xAAC2) ||
           in_range(codepoint, 0xAADB, 0xAADC) ||
           in_range(codepoint, 0xAAE0, 0xAAEA) ||
           in_range(codepoint, 0xAAF2, 0xAAF2) ||
           in_range(codepoint, 0xAB01, 0xAB06) ||
           in_range(codepoint, 0xAB09, 0xAB0E) ||
           in_range(codepoint, 0xAB11, 0xAB16) ||
           in_range(codepoint, 0xAB20, 0xAB26) ||
           in_range(codepoint, 0xAB28, 0xAB2E) ||
           in_range(codepoint, 0xABC0, 0xABE2) ||
           in_range(codepoint, 0xAC00, 0xAC00) ||
           in_range(codepoint, 0xD7A3, 0xD7A3) ||
           in_range(codepoint, 0xD7B0, 0xD7C6) ||
           in_range(codepoint, 0xD7CB, 0xD7FB) ||
           in_range(codepoint, 0xF900, 0xFA6D) ||
           in_range(codepoint, 0xFA70, 0xFAD9) ||
           in_range(codepoint, 0xFB1D, 0xFB1D) ||
           in_range(codepoint, 0xFB1F, 0xFB28) ||
           in_range(codepoint, 0xFB2A, 0xFB36) ||
           in_range(codepoint, 0xFB38, 0xFB3C) ||
           in_range(codepoint, 0xFB3E, 0xFB3E) ||
           in_range(codepoint, 0xFB40, 0xFB41) ||
           in_range(codepoint, 0xFB43, 0xFB44) ||
           in_range(codepoint, 0xFB46, 0xFBB1) ||
           in_range(codepoint, 0xFBD3, 0xFD3D) ||
           in_range(codepoint, 0xFD50, 0xFD8F) ||
           in_range(codepoint, 0xFD92, 0xFDC7) ||
           in_range(codepoint, 0xFDF0, 0xFDFB) ||
           in_range(codepoint, 0xFE70, 0xFE74) ||
           in_range(codepoint, 0xFE76, 0xFEFC) ||
           in_range(codepoint, 0xFF66, 0xFF6F) ||
           in_range(codepoint, 0xFF71, 0xFF9D) ||
           in_range(codepoint, 0xFFA0, 0xFFBE) ||
           in_range(codepoint, 0xFFC2, 0xFFC7) ||
           in_range(codepoint, 0xFFCA, 0xFFCF) ||
           in_range(codepoint, 0xFFD2, 0xFFD7) ||
           in_range(codepoint, 0xFFDA, 0xFFDC) ||
           in_range(codepoint, 0x10000, 0x1000B) ||
           in_range(codepoint, 0x1000D, 0x10026) ||
           in_range(codepoint, 0x10028, 0x1003A) ||
           in_range(codepoint, 0x1003C, 0x1003D) ||
           in_range(codepoint, 0x1003F, 0x1004D) ||
           in_range(codepoint, 0x10050, 0x1005D) ||
           in_range(codepoint, 0x10080, 0x100FA) ||
           in_range(codepoint, 0x10280, 0x1029C) ||
           in_range(codepoint, 0x102A0, 0x102D0) ||
           in_range(codepoint, 0x10300, 0x1031F) ||
           in_range(codepoint, 0x1032D, 0x10340) ||
           in_range(codepoint, 0x10342, 0x10349) ||
           in_range(codepoint, 0x10350, 0x10375) ||
           in_range(codepoint, 0x10380, 0x1039D) ||
           in_range(codepoint, 0x103A0, 0x103C3) ||
           in_range(codepoint, 0x103C8, 0x103CF) ||
           in_range(codepoint, 0x10450, 0x1049D) ||
           in_range(codepoint, 0x10500, 0x10527) ||
           in_range(codepoint, 0x10530, 0x10563) ||
           in_range(codepoint, 0x10600, 0x10736) ||
           in_range(codepoint, 0x10740, 0x10755) ||
           in_range(codepoint, 0x10760, 0x10767) ||
           in_range(codepoint, 0x10800, 0x10805) ||
           in_range(codepoint, 0x10808, 0x10808) ||
           in_range(codepoint, 0x1080A, 0x10835) ||
           in_range(codepoint, 0x10837, 0x10838) ||
           in_range(codepoint, 0x1083C, 0x1083C) ||
           in_range(codepoint, 0x1083F, 0x10855) ||
           in_range(codepoint, 0x10860, 0x10876) ||
           in_range(codepoint, 0x10880, 0x1089E) ||
           in_range(codepoint, 0x108E0, 0x108F2) ||
           in_range(codepoint, 0x108F4, 0x108F5) ||
           in_range(codepoint, 0x10900, 0x10915) ||
           in_range(codepoint, 0x10920, 0x10939) ||
           in_range(codepoint, 0x10980, 0x109B7) ||
           in_range(codepoint, 0x109BE, 0x109BF) ||
           in_range(codepoint, 0x10A00, 0x10A00) ||
           in_range(codepoint, 0x10A10, 0x10A13) ||
           in_range(codepoint, 0x10A15, 0x10A17) ||
           in_range(codepoint, 0x10A19, 0x10A35) ||
           in_range(codepoint, 0x10A60, 0x10A7C) ||
           in_range(codepoint, 0x10A80, 0x10A9C) ||
           in_range(codepoint, 0x10AC0, 0x10AC7) ||
           in_range(codepoint, 0x10AC9, 0x10AE4) ||
           in_range(codepoint, 0x10B00, 0x10B35) ||
           in_range(codepoint, 0x10B40, 0x10B55) ||
           in_range(codepoint, 0x10B60, 0x10B72) ||
           in_range(codepoint, 0x10B80, 0x10B91) ||
           in_range(codepoint, 0x10C00, 0x10C48) ||
           in_range(codepoint, 0x10D00, 0x10D23) ||
           in_range(codepoint, 0x10E80, 0x10EA9) ||
           in_range(codepoint, 0x10EB0, 0x10EB1) ||
           in_range(codepoint, 0x10F00, 0x10F1C) ||
           in_range(codepoint, 0x10F27, 0x10F27) ||
           in_range(codepoint, 0x10F30, 0x10F45) ||
           in_range(codepoint, 0x10FB0, 0x10FC4) ||
           in_range(codepoint, 0x10FE0, 0x10FF6) ||
           in_range(codepoint, 0x11003, 0x11037) ||
           in_range(codepoint, 0x11083, 0x110AF) ||
           in_range(codepoint, 0x110D0, 0x110E8) ||
           in_range(codepoint, 0x11103, 0x11126) ||
           in_range(codepoint, 0x11144, 0x11144) ||
           in_range(codepoint, 0x11147, 0x11147) ||
           in_range(codepoint, 0x11150, 0x11172) ||
           in_range(codepoint, 0x11176, 0x11176) ||
           in_range(codepoint, 0x11183, 0x111B2) ||
           in_range(codepoint, 0x111C1, 0x111C4) ||
           in_range(codepoint, 0x111DA, 0x111DA) ||
           in_range(codepoint, 0x111DC, 0x111DC) ||
           in_range(codepoint, 0x11200, 0x11211) ||
           in_range(codepoint, 0x11213, 0x1122B) ||
           in_range(codepoint, 0x11280, 0x11286) ||
           in_range(codepoint, 0x11288, 0x11288) ||
           in_range(codepoint, 0x1128A, 0x1128D) ||
           in_range(codepoint, 0x1128F, 0x1129D) ||
           in_range(codepoint, 0x1129F, 0x112A8) ||
           in_range(codepoint, 0x112B0, 0x112DE) ||
           in_range(codepoint, 0x11305, 0x1130C) ||
           in_range(codepoint, 0x1130F, 0x11310) ||
           in_range(codepoint, 0x11313, 0x11328) ||
           in_range(codepoint, 0x1132A, 0x11330) ||
           in_range(codepoint, 0x11332, 0x11333) ||
           in_range(codepoint, 0x11335, 0x11339) ||
           in_range(codepoint, 0x1133D, 0x1133D) ||
           in_range(codepoint, 0x11350, 0x11350) ||
           in_range(codepoint, 0x1135D, 0x11361) ||
           in_range(codepoint, 0x11400, 0x11434) ||
           in_range(codepoint, 0x11447, 0x1144A) ||
           in_range(codepoint, 0x1145F, 0x11461) ||
           in_range(codepoint, 0x11480, 0x114AF) ||
           in_range(codepoint, 0x114C4, 0x114C5) ||
           in_range(codepoint, 0x114C7, 0x114C7) ||
           in_range(codepoint, 0x11580, 0x115AE) ||
           in_range(codepoint, 0x115D8, 0x115DB) ||
           in_range(codepoint, 0x11600, 0x1162F) ||
           in_range(codepoint, 0x11644, 0x11644) ||
           in_range(codepoint, 0x11680, 0x116AA) ||
           in_range(codepoint, 0x116B8, 0x116B8) ||
           in_range(codepoint, 0x11700, 0x1171A) ||
           in_range(codepoint, 0x11800, 0x1182B) ||
           in_range(codepoint, 0x118FF, 0x11906) ||
           in_range(codepoint, 0x11909, 0x11909) ||
           in_range(codepoint, 0x1190C, 0x11913) ||
           in_range(codepoint, 0x11915, 0x11916) ||
           in_range(codepoint, 0x11918, 0x1192F) ||
           in_range(codepoint, 0x1193F, 0x1193F) ||
           in_range(codepoint, 0x11941, 0x11941) ||
           in_range(codepoint, 0x119A0, 0x119A7) ||
           in_range(codepoint, 0x119AA, 0x119D0) ||
           in_range(codepoint, 0x119E1, 0x119E1) ||
           in_range(codepoint, 0x119E3, 0x119E3) ||
           in_range(codepoint, 0x11A00, 0x11A00) ||
           in_range(codepoint, 0x11A0B, 0x11A32) ||
           in_range(codepoint, 0x11A3A, 0x11A3A) ||
           in_range(codepoint, 0x11A50, 0x11A50) ||
           in_range(codepoint, 0x11A5C, 0x11A89) ||
           in_range(codepoint, 0x11A9D, 0x11A9D) ||
           in_range(codepoint, 0x11AC0, 0x11AF8) ||
           in_range(codepoint, 0x11C00, 0x11C08) ||
           in_range(codepoint, 0x11C0A, 0x11C2E) ||
           in_range(codepoint, 0x11C40, 0x11C40) ||
           in_range(codepoint, 0x11C72, 0x11C8F) ||
           in_range(codepoint, 0x11D00, 0x11D06) ||
           in_range(codepoint, 0x11D08, 0x11D09) ||
           in_range(codepoint, 0x11D0B, 0x11D30) ||
           in_range(codepoint, 0x11D46, 0x11D46) ||
           in_range(codepoint, 0x11D60, 0x11D65) ||
           in_range(codepoint, 0x11D67, 0x11D68) ||
           in_range(codepoint, 0x11D6A, 0x11D89) ||
           in_range(codepoint, 0x11D98, 0x11D98) ||
           in_range(codepoint, 0x11EE0, 0x11EF2) ||
           in_range(codepoint, 0x11FB0, 0x11FB0) ||
           in_range(codepoint, 0x12000, 0x12399) ||
           in_range(codepoint, 0x12480, 0x12543) ||
           in_range(codepoint, 0x13000, 0x1342E) ||
           in_range(codepoint, 0x14400, 0x14646) ||
           in_range(codepoint, 0x16800, 0x16A38) ||
           in_range(codepoint, 0x16A40, 0x16A5E) ||
           in_range(codepoint, 0x16AD0, 0x16AED) ||
           in_range(codepoint, 0x16B00, 0x16B2F) ||
           in_range(codepoint, 0x16B63, 0x16B77) ||
           in_range(codepoint, 0x16B7D, 0x16B8F) ||
           in_range(codepoint, 0x16F00, 0x16F4A) ||
           in_range(codepoint, 0x16F50, 0x16F50) ||
           in_range(codepoint, 0x17000, 0x17000) ||
           in_range(codepoint, 0x187F7, 0x187F7) ||
           in_range(codepoint, 0x18800, 0x18CD5) ||
           in_range(codepoint, 0x18D00, 0x18D00) ||
           in_range(codepoint, 0x18D08, 0x18D08) ||
           in_range(codepoint, 0x1B000, 0x1B11E) ||
           in_range(codepoint, 0x1B150, 0x1B152) ||
           in_range(codepoint, 0x1B164, 0x1B167) ||
           in_range(codepoint, 0x1B170, 0x1B2FB) ||
           in_range(codepoint, 0x1BC00, 0x1BC6A) ||
           in_range(codepoint, 0x1BC70, 0x1BC7C) ||
           in_range(codepoint, 0x1BC80, 0x1BC88) ||
           in_range(codepoint, 0x1BC90, 0x1BC99) ||
           in_range(codepoint, 0x1E100, 0x1E12C) ||
           in_range(codepoint, 0x1E14E, 0x1E14E) ||
           in_range(codepoint, 0x1E2C0, 0x1E2EB) ||
           in_range(codepoint, 0x1E800, 0x1E8C4) ||
           in_range(codepoint, 0x1EE00, 0x1EE03) ||
           in_range(codepoint, 0x1EE05, 0x1EE1F) ||
           in_range(codepoint, 0x1EE21, 0x1EE22) ||
           in_range(codepoint, 0x1EE24, 0x1EE24) ||
           in_range(codepoint, 0x1EE27, 0x1EE27) ||
           in_range(codepoint, 0x1EE29, 0x1EE32) ||
           in_range(codepoint, 0x1EE34, 0x1EE37) ||
           in_range(codepoint, 0x1EE39, 0x1EE39) ||
           in_range(codepoint, 0x1EE3B, 0x1EE3B) ||
           in_range(codepoint, 0x1EE42, 0x1EE42) ||
           in_range(codepoint, 0x1EE47, 0x1EE47) ||
           in_range(codepoint, 0x1EE49, 0x1EE49) ||
           in_range(codepoint, 0x1EE4B, 0x1EE4B) ||
           in_range(codepoint, 0x1EE4D, 0x1EE4F) ||
           in_range(codepoint, 0x1EE51, 0x1EE52) ||
           in_range(codepoint, 0x1EE54, 0x1EE54) ||
           in_range(codepoint, 0x1EE57, 0x1EE57) ||
           in_range(codepoint, 0x1EE59, 0x1EE59) ||
           in_range(codepoint, 0x1EE5B, 0x1EE5B) ||
           in_range(codepoint, 0x1EE5D, 0x1EE5D) ||
           in_range(codepoint, 0x1EE5F, 0x1EE5F) ||
           in_range(codepoint, 0x1EE61, 0x1EE62) ||
           in_range(codepoint, 0x1EE64, 0x1EE64) ||
           in_range(codepoint, 0x1EE67, 0x1EE6A) ||
           in_range(codepoint, 0x1EE6C, 0x1EE72) ||
           in_range(codepoint, 0x1EE74, 0x1EE77) ||
           in_range(codepoint, 0x1EE79, 0x1EE7C) ||
           in_range(codepoint, 0x1EE7E, 0x1EE7E) ||
           in_range(codepoint, 0x1EE80, 0x1EE89) ||
           in_range(codepoint, 0x1EE8B, 0x1EE9B) ||
           in_range(codepoint, 0x1EEA1, 0x1EEA3) ||
           in_range(codepoint, 0x1EEA5, 0x1EEA9) ||
           in_range(codepoint, 0x1EEAB, 0x1EEBB) ||
           in_range(codepoint, 0x20000, 0x20000) ||
           in_range(codepoint, 0x2A6DD, 0x2A6DD) ||
           in_range(codepoint, 0x2A700, 0x2A700) ||
           in_range(codepoint, 0x2B734, 0x2B734) ||
           in_range(codepoint, 0x2B740, 0x2B740) ||
           in_range(codepoint, 0x2B81D, 0x2B81D) ||
           in_range(codepoint, 0x2B820, 0x2B820) ||
           in_range(codepoint, 0x2CEA1, 0x2CEA1) ||
           in_range(codepoint, 0x2CEB0, 0x2CEB0) ||
           in_range(codepoint, 0x2EBE0, 0x2EBE0) ||
           in_range(codepoint, 0x2F800, 0x2FA1D) ||
           in_range(codepoint, 0x30000, 0x30000) ||
           in_range(codepoint, 0x3134A, 0x3134A);
}

inline bool text::unicode_category_lt(datum_type codepoint)
{
    return in_range(codepoint, 0x01C5, 0x01C5) ||
           in_range(codepoint, 0x01C8, 0x01C8) ||
           in_range(codepoint, 0x01CB, 0x01CB) ||
           in_range(codepoint, 0x01F2, 0x01F2) ||
           in_range(codepoint, 0x1F88, 0x1F8F) ||
           in_range(codepoint, 0x1F98, 0x1F9F) ||
           in_range(codepoint, 0x1FA8, 0x1FAF) ||
           in_range(codepoint, 0x1FBC, 0x1FBC) ||
           in_range(codepoint, 0x1FCC, 0x1FCC) ||
           in_range(codepoint, 0x1FFC, 0x1FFC);
}

inline bool text::unicode_category_lu(datum_type codepoint)
{
    return in_range(codepoint, 0x0041, 0x005A) ||
           in_range(codepoint, 0x00C0, 0x00D6) ||
           in_range(codepoint, 0x00D8, 0x00DE) ||
           in_range(codepoint, 0x0100, 0x0100) ||
           in_range(codepoint, 0x0102, 0x0102) ||
           in_range(codepoint, 0x0104, 0x0104) ||
           in_range(codepoint, 0x0106, 0x0106) ||
           in_range(codepoint, 0x0108, 0x0108) ||
           in_range(codepoint, 0x010A, 0x010A) ||
           in_range(codepoint, 0x010C, 0x010C) ||
           in_range(codepoint, 0x010E, 0x010E) ||
           in_range(codepoint, 0x0110, 0x0110) ||
           in_range(codepoint, 0x0112, 0x0112) ||
           in_range(codepoint, 0x0114, 0x0114) ||
           in_range(codepoint, 0x0116, 0x0116) ||
           in_range(codepoint, 0x0118, 0x0118) ||
           in_range(codepoint, 0x011A, 0x011A) ||
           in_range(codepoint, 0x011C, 0x011C) ||
           in_range(codepoint, 0x011E, 0x011E) ||
           in_range(codepoint, 0x0120, 0x0120) ||
           in_range(codepoint, 0x0122, 0x0122) ||
           in_range(codepoint, 0x0124, 0x0124) ||
           in_range(codepoint, 0x0126, 0x0126) ||
           in_range(codepoint, 0x0128, 0x0128) ||
           in_range(codepoint, 0x012A, 0x012A) ||
           in_range(codepoint, 0x012C, 0x012C) ||
           in_range(codepoint, 0x012E, 0x012E) ||
           in_range(codepoint, 0x0130, 0x0130) ||
           in_range(codepoint, 0x0132, 0x0132) ||
           in_range(codepoint, 0x0134, 0x0134) ||
           in_range(codepoint, 0x0136, 0x0136) ||
           in_range(codepoint, 0x0139, 0x0139) ||
           in_range(codepoint, 0x013B, 0x013B) ||
           in_range(codepoint, 0x013D, 0x013D) ||
           in_range(codepoint, 0x013F, 0x013F) ||
           in_range(codepoint, 0x0141, 0x0141) ||
           in_range(codepoint, 0x0143, 0x0143) ||
           in_range(codepoint, 0x0145, 0x0145) ||
           in_range(codepoint, 0x0147, 0x0147) ||
           in_range(codepoint, 0x014A, 0x014A) ||
           in_range(codepoint, 0x014C, 0x014C) ||
           in_range(codepoint, 0x014E, 0x014E) ||
           in_range(codepoint, 0x0150, 0x0150) ||
           in_range(codepoint, 0x0152, 0x0152) ||
           in_range(codepoint, 0x0154, 0x0154) ||
           in_range(codepoint, 0x0156, 0x0156) ||
           in_range(codepoint, 0x0158, 0x0158) ||
           in_range(codepoint, 0x015A, 0x015A) ||
           in_range(codepoint, 0x015C, 0x015C) ||
           in_range(codepoint, 0x015E, 0x015E) ||
           in_range(codepoint, 0x0160, 0x0160) ||
           in_range(codepoint, 0x0162, 0x0162) ||
           in_range(codepoint, 0x0164, 0x0164) ||
           in_range(codepoint, 0x0166, 0x0166) ||
           in_range(codepoint, 0x0168, 0x0168) ||
           in_range(codepoint, 0x016A, 0x016A) ||
           in_range(codepoint, 0x016C, 0x016C) ||
           in_range(codepoint, 0x016E, 0x016E) ||
           in_range(codepoint, 0x0170, 0x0170) ||
           in_range(codepoint, 0x0172, 0x0172) ||
           in_range(codepoint, 0x0174, 0x0174) ||
           in_range(codepoint, 0x0176, 0x0176) ||
           in_range(codepoint, 0x0178, 0x0179) ||
           in_range(codepoint, 0x017B, 0x017B) ||
           in_range(codepoint, 0x017D, 0x017D) ||
           in_range(codepoint, 0x0181, 0x0182) ||
           in_range(codepoint, 0x0184, 0x0184) ||
           in_range(codepoint, 0x0186, 0x0187) ||
           in_range(codepoint, 0x0189, 0x018B) ||
           in_range(codepoint, 0x018E, 0x0191) ||
           in_range(codepoint, 0x0193, 0x0194) ||
           in_range(codepoint, 0x0196, 0x0198) ||
           in_range(codepoint, 0x019C, 0x019D) ||
           in_range(codepoint, 0x019F, 0x01A0) ||
           in_range(codepoint, 0x01A2, 0x01A2) ||
           in_range(codepoint, 0x01A4, 0x01A4) ||
           in_range(codepoint, 0x01A6, 0x01A7) ||
           in_range(codepoint, 0x01A9, 0x01A9) ||
           in_range(codepoint, 0x01AC, 0x01AC) ||
           in_range(codepoint, 0x01AE, 0x01AF) ||
           in_range(codepoint, 0x01B1, 0x01B3) ||
           in_range(codepoint, 0x01B5, 0x01B5) ||
           in_range(codepoint, 0x01B7, 0x01B8) ||
           in_range(codepoint, 0x01BC, 0x01BC) ||
           in_range(codepoint, 0x01C4, 0x01C4) ||
           in_range(codepoint, 0x01C7, 0x01C7) ||
           in_range(codepoint, 0x01CA, 0x01CA) ||
           in_range(codepoint, 0x01CD, 0x01CD) ||
           in_range(codepoint, 0x01CF, 0x01CF) ||
           in_range(codepoint, 0x01D1, 0x01D1) ||
           in_range(codepoint, 0x01D3, 0x01D3) ||
           in_range(codepoint, 0x01D5, 0x01D5) ||
           in_range(codepoint, 0x01D7, 0x01D7) ||
           in_range(codepoint, 0x01D9, 0x01D9) ||
           in_range(codepoint, 0x01DB, 0x01DB) ||
           in_range(codepoint, 0x01DE, 0x01DE) ||
           in_range(codepoint, 0x01E0, 0x01E0) ||
           in_range(codepoint, 0x01E2, 0x01E2) ||
           in_range(codepoint, 0x01E4, 0x01E4) ||
           in_range(codepoint, 0x01E6, 0x01E6) ||
           in_range(codepoint, 0x01E8, 0x01E8) ||
           in_range(codepoint, 0x01EA, 0x01EA) ||
           in_range(codepoint, 0x01EC, 0x01EC) ||
           in_range(codepoint, 0x01EE, 0x01EE) ||
           in_range(codepoint, 0x01F1, 0x01F1) ||
           in_range(codepoint, 0x01F4, 0x01F4) ||
           in_range(codepoint, 0x01F6, 0x01F8) ||
           in_range(codepoint, 0x01FA, 0x01FA) ||
           in_range(codepoint, 0x01FC, 0x01FC) ||
           in_range(codepoint, 0x01FE, 0x01FE) ||
           in_range(codepoint, 0x0200, 0x0200) ||
           in_range(codepoint, 0x0202, 0x0202) ||
           in_range(codepoint, 0x0204, 0x0204) ||
           in_range(codepoint, 0x0206, 0x0206) ||
           in_range(codepoint, 0x0208, 0x0208) ||
           in_range(codepoint, 0x020A, 0x020A) ||
           in_range(codepoint, 0x020C, 0x020C) ||
           in_range(codepoint, 0x020E, 0x020E) ||
           in_range(codepoint, 0x0210, 0x0210) ||
           in_range(codepoint, 0x0212, 0x0212) ||
           in_range(codepoint, 0x0214, 0x0214) ||
           in_range(codepoint, 0x0216, 0x0216) ||
           in_range(codepoint, 0x0218, 0x0218) ||
           in_range(codepoint, 0x021A, 0x021A) ||
           in_range(codepoint, 0x021C, 0x021C) ||
           in_range(codepoint, 0x021E, 0x021E) ||
           in_range(codepoint, 0x0220, 0x0220) ||
           in_range(codepoint, 0x0222, 0x0222) ||
           in_range(codepoint, 0x0224, 0x0224) ||
           in_range(codepoint, 0x0226, 0x0226) ||
           in_range(codepoint, 0x0228, 0x0228) ||
           in_range(codepoint, 0x022A, 0x022A) ||
           in_range(codepoint, 0x022C, 0x022C) ||
           in_range(codepoint, 0x022E, 0x022E) ||
           in_range(codepoint, 0x0230, 0x0230) ||
           in_range(codepoint, 0x0232, 0x0232) ||
           in_range(codepoint, 0x023A, 0x023B) ||
           in_range(codepoint, 0x023D, 0x023E) ||
           in_range(codepoint, 0x0241, 0x0241) ||
           in_range(codepoint, 0x0243, 0x0246) ||
           in_range(codepoint, 0x0248, 0x0248) ||
           in_range(codepoint, 0x024A, 0x024A) ||
           in_range(codepoint, 0x024C, 0x024C) ||
           in_range(codepoint, 0x024E, 0x024E) ||
           in_range(codepoint, 0x0370, 0x0370) ||
           in_range(codepoint, 0x0372, 0x0372) ||
           in_range(codepoint, 0x0376, 0x0376) ||
           in_range(codepoint, 0x037F, 0x037F) ||
           in_range(codepoint, 0x0386, 0x0386) ||
           in_range(codepoint, 0x0388, 0x038A) ||
           in_range(codepoint, 0x038C, 0x038C) ||
           in_range(codepoint, 0x038E, 0x038F) ||
           in_range(codepoint, 0x0391, 0x03A1) ||
           in_range(codepoint, 0x03A3, 0x03AB) ||
           in_range(codepoint, 0x03CF, 0x03CF) ||
           in_range(codepoint, 0x03D2, 0x03D4) ||
           in_range(codepoint, 0x03D8, 0x03D8) ||
           in_range(codepoint, 0x03DA, 0x03DA) ||
           in_range(codepoint, 0x03DC, 0x03DC) ||
           in_range(codepoint, 0x03DE, 0x03DE) ||
           in_range(codepoint, 0x03E0, 0x03E0) ||
           in_range(codepoint, 0x03E2, 0x03E2) ||
           in_range(codepoint, 0x03E4, 0x03E4) ||
           in_range(codepoint, 0x03E6, 0x03E6) ||
           in_range(codepoint, 0x03E8, 0x03E8) ||
           in_range(codepoint, 0x03EA, 0x03EA) ||
           in_range(codepoint, 0x03EC, 0x03EC) ||
           in_range(codepoint, 0x03EE, 0x03EE) ||
           in_range(codepoint, 0x03F4, 0x03F4) ||
           in_range(codepoint, 0x03F7, 0x03F7) ||
           in_range(codepoint, 0x03F9, 0x03FA) ||
           in_range(codepoint, 0x03FD, 0x042F) ||
           in_range(codepoint, 0x0460, 0x0460) ||
           in_range(codepoint, 0x0462, 0x0462) ||
           in_range(codepoint, 0x0464, 0x0464) ||
           in_range(codepoint, 0x0466, 0x0466) ||
           in_range(codepoint, 0x0468, 0x0468) ||
           in_range(codepoint, 0x046A, 0x046A) ||
           in_range(codepoint, 0x046C, 0x046C) ||
           in_range(codepoint, 0x046E, 0x046E) ||
           in_range(codepoint, 0x0470, 0x0470) ||
           in_range(codepoint, 0x0472, 0x0472) ||
           in_range(codepoint, 0x0474, 0x0474) ||
           in_range(codepoint, 0x0476, 0x0476) ||
           in_range(codepoint, 0x0478, 0x0478) ||
           in_range(codepoint, 0x047A, 0x047A) ||
           in_range(codepoint, 0x047C, 0x047C) ||
           in_range(codepoint, 0x047E, 0x047E) ||
           in_range(codepoint, 0x0480, 0x0480) ||
           in_range(codepoint, 0x048A, 0x048A) ||
           in_range(codepoint, 0x048C, 0x048C) ||
           in_range(codepoint, 0x048E, 0x048E) ||
           in_range(codepoint, 0x0490, 0x0490) ||
           in_range(codepoint, 0x0492, 0x0492) ||
           in_range(codepoint, 0x0494, 0x0494) ||
           in_range(codepoint, 0x0496, 0x0496) ||
           in_range(codepoint, 0x0498, 0x0498) ||
           in_range(codepoint, 0x049A, 0x049A) ||
           in_range(codepoint, 0x049C, 0x049C) ||
           in_range(codepoint, 0x049E, 0x049E) ||
           in_range(codepoint, 0x04A0, 0x04A0) ||
           in_range(codepoint, 0x04A2, 0x04A2) ||
           in_range(codepoint, 0x04A4, 0x04A4) ||
           in_range(codepoint, 0x04A6, 0x04A6) ||
           in_range(codepoint, 0x04A8, 0x04A8) ||
           in_range(codepoint, 0x04AA, 0x04AA) ||
           in_range(codepoint, 0x04AC, 0x04AC) ||
           in_range(codepoint, 0x04AE, 0x04AE) ||
           in_range(codepoint, 0x04B0, 0x04B0) ||
           in_range(codepoint, 0x04B2, 0x04B2) ||
           in_range(codepoint, 0x04B4, 0x04B4) ||
           in_range(codepoint, 0x04B6, 0x04B6) ||
           in_range(codepoint, 0x04B8, 0x04B8) ||
           in_range(codepoint, 0x04BA, 0x04BA) ||
           in_range(codepoint, 0x04BC, 0x04BC) ||
           in_range(codepoint, 0x04BE, 0x04BE) ||
           in_range(codepoint, 0x04C0, 0x04C1) ||
           in_range(codepoint, 0x04C3, 0x04C3) ||
           in_range(codepoint, 0x04C5, 0x04C5) ||
           in_range(codepoint, 0x04C7, 0x04C7) ||
           in_range(codepoint, 0x04C9, 0x04C9) ||
           in_range(codepoint, 0x04CB, 0x04CB) ||
           in_range(codepoint, 0x04CD, 0x04CD) ||
           in_range(codepoint, 0x04D0, 0x04D0) ||
           in_range(codepoint, 0x04D2, 0x04D2) ||
           in_range(codepoint, 0x04D4, 0x04D4) ||
           in_range(codepoint, 0x04D6, 0x04D6) ||
           in_range(codepoint, 0x04D8, 0x04D8) ||
           in_range(codepoint, 0x04DA, 0x04DA) ||
           in_range(codepoint, 0x04DC, 0x04DC) ||
           in_range(codepoint, 0x04DE, 0x04DE) ||
           in_range(codepoint, 0x04E0, 0x04E0) ||
           in_range(codepoint, 0x04E2, 0x04E2) ||
           in_range(codepoint, 0x04E4, 0x04E4) ||
           in_range(codepoint, 0x04E6, 0x04E6) ||
           in_range(codepoint, 0x04E8, 0x04E8) ||
           in_range(codepoint, 0x04EA, 0x04EA) ||
           in_range(codepoint, 0x04EC, 0x04EC) ||
           in_range(codepoint, 0x04EE, 0x04EE) ||
           in_range(codepoint, 0x04F0, 0x04F0) ||
           in_range(codepoint, 0x04F2, 0x04F2) ||
           in_range(codepoint, 0x04F4, 0x04F4) ||
           in_range(codepoint, 0x04F6, 0x04F6) ||
           in_range(codepoint, 0x04F8, 0x04F8) ||
           in_range(codepoint, 0x04FA, 0x04FA) ||
           in_range(codepoint, 0x04FC, 0x04FC) ||
           in_range(codepoint, 0x04FE, 0x04FE) ||
           in_range(codepoint, 0x0500, 0x0500) ||
           in_range(codepoint, 0x0502, 0x0502) ||
           in_range(codepoint, 0x0504, 0x0504) ||
           in_range(codepoint, 0x0506, 0x0506) ||
           in_range(codepoint, 0x0508, 0x0508) ||
           in_range(codepoint, 0x050A, 0x050A) ||
           in_range(codepoint, 0x050C, 0x050C) ||
           in_range(codepoint, 0x050E, 0x050E) ||
           in_range(codepoint, 0x0510, 0x0510) ||
           in_range(codepoint, 0x0512, 0x0512) ||
           in_range(codepoint, 0x0514, 0x0514) ||
           in_range(codepoint, 0x0516, 0x0516) ||
           in_range(codepoint, 0x0518, 0x0518) ||
           in_range(codepoint, 0x051A, 0x051A) ||
           in_range(codepoint, 0x051C, 0x051C) ||
           in_range(codepoint, 0x051E, 0x051E) ||
           in_range(codepoint, 0x0520, 0x0520) ||
           in_range(codepoint, 0x0522, 0x0522) ||
           in_range(codepoint, 0x0524, 0x0524) ||
           in_range(codepoint, 0x0526, 0x0526) ||
           in_range(codepoint, 0x0528, 0x0528) ||
           in_range(codepoint, 0x052A, 0x052A) ||
           in_range(codepoint, 0x052C, 0x052C) ||
           in_range(codepoint, 0x052E, 0x052E) ||
           in_range(codepoint, 0x0531, 0x0556) ||
           in_range(codepoint, 0x10A0, 0x10C5) ||
           in_range(codepoint, 0x10C7, 0x10C7) ||
           in_range(codepoint, 0x10CD, 0x10CD) ||
           in_range(codepoint, 0x13A0, 0x13F5) ||
           in_range(codepoint, 0x1C90, 0x1CBA) ||
           in_range(codepoint, 0x1CBD, 0x1CBF) ||
           in_range(codepoint, 0x1E00, 0x1E00) ||
           in_range(codepoint, 0x1E02, 0x1E02) ||
           in_range(codepoint, 0x1E04, 0x1E04) ||
           in_range(codepoint, 0x1E06, 0x1E06) ||
           in_range(codepoint, 0x1E08, 0x1E08) ||
           in_range(codepoint, 0x1E0A, 0x1E0A) ||
           in_range(codepoint, 0x1E0C, 0x1E0C) ||
           in_range(codepoint, 0x1E0E, 0x1E0E) ||
           in_range(codepoint, 0x1E10, 0x1E10) ||
           in_range(codepoint, 0x1E12, 0x1E12) ||
           in_range(codepoint, 0x1E14, 0x1E14) ||
           in_range(codepoint, 0x1E16, 0x1E16) ||
           in_range(codepoint, 0x1E18, 0x1E18) ||
           in_range(codepoint, 0x1E1A, 0x1E1A) ||
           in_range(codepoint, 0x1E1C, 0x1E1C) ||
           in_range(codepoint, 0x1E1E, 0x1E1E) ||
           in_range(codepoint, 0x1E20, 0x1E20) ||
           in_range(codepoint, 0x1E22, 0x1E22) ||
           in_range(codepoint, 0x1E24, 0x1E24) ||
           in_range(codepoint, 0x1E26, 0x1E26) ||
           in_range(codepoint, 0x1E28, 0x1E28) ||
           in_range(codepoint, 0x1E2A, 0x1E2A) ||
           in_range(codepoint, 0x1E2C, 0x1E2C) ||
           in_range(codepoint, 0x1E2E, 0x1E2E) ||
           in_range(codepoint, 0x1E30, 0x1E30) ||
           in_range(codepoint, 0x1E32, 0x1E32) ||
           in_range(codepoint, 0x1E34, 0x1E34) ||
           in_range(codepoint, 0x1E36, 0x1E36) ||
           in_range(codepoint, 0x1E38, 0x1E38) ||
           in_range(codepoint, 0x1E3A, 0x1E3A) ||
           in_range(codepoint, 0x1E3C, 0x1E3C) ||
           in_range(codepoint, 0x1E3E, 0x1E3E) ||
           in_range(codepoint, 0x1E40, 0x1E40) ||
           in_range(codepoint, 0x1E42, 0x1E42) ||
           in_range(codepoint, 0x1E44, 0x1E44) ||
           in_range(codepoint, 0x1E46, 0x1E46) ||
           in_range(codepoint, 0x1E48, 0x1E48) ||
           in_range(codepoint, 0x1E4A, 0x1E4A) ||
           in_range(codepoint, 0x1E4C, 0x1E4C) ||
           in_range(codepoint, 0x1E4E, 0x1E4E) ||
           in_range(codepoint, 0x1E50, 0x1E50) ||
           in_range(codepoint, 0x1E52, 0x1E52) ||
           in_range(codepoint, 0x1E54, 0x1E54) ||
           in_range(codepoint, 0x1E56, 0x1E56) ||
           in_range(codepoint, 0x1E58, 0x1E58) ||
           in_range(codepoint, 0x1E5A, 0x1E5A) ||
           in_range(codepoint, 0x1E5C, 0x1E5C) ||
           in_range(codepoint, 0x1E5E, 0x1E5E) ||
           in_range(codepoint, 0x1E60, 0x1E60) ||
           in_range(codepoint, 0x1E62, 0x1E62) ||
           in_range(codepoint, 0x1E64, 0x1E64) ||
           in_range(codepoint, 0x1E66, 0x1E66) ||
           in_range(codepoint, 0x1E68, 0x1E68) ||
           in_range(codepoint, 0x1E6A, 0x1E6A) ||
           in_range(codepoint, 0x1E6C, 0x1E6C) ||
           in_range(codepoint, 0x1E6E, 0x1E6E) ||
           in_range(codepoint, 0x1E70, 0x1E70) ||
           in_range(codepoint, 0x1E72, 0x1E72) ||
           in_range(codepoint, 0x1E74, 0x1E74) ||
           in_range(codepoint, 0x1E76, 0x1E76) ||
           in_range(codepoint, 0x1E78, 0x1E78) ||
           in_range(codepoint, 0x1E7A, 0x1E7A) ||
           in_range(codepoint, 0x1E7C, 0x1E7C) ||
           in_range(codepoint, 0x1E7E, 0x1E7E) ||
           in_range(codepoint, 0x1E80, 0x1E80) ||
           in_range(codepoint, 0x1E82, 0x1E82) ||
           in_range(codepoint, 0x1E84, 0x1E84) ||
           in_range(codepoint, 0x1E86, 0x1E86) ||
           in_range(codepoint, 0x1E88, 0x1E88) ||
           in_range(codepoint, 0x1E8A, 0x1E8A) ||
           in_range(codepoint, 0x1E8C, 0x1E8C) ||
           in_range(codepoint, 0x1E8E, 0x1E8E) ||
           in_range(codepoint, 0x1E90, 0x1E90) ||
           in_range(codepoint, 0x1E92, 0x1E92) ||
           in_range(codepoint, 0x1E94, 0x1E94) ||
           in_range(codepoint, 0x1E9E, 0x1E9E) ||
           in_range(codepoint, 0x1EA0, 0x1EA0) ||
           in_range(codepoint, 0x1EA2, 0x1EA2) ||
           in_range(codepoint, 0x1EA4, 0x1EA4) ||
           in_range(codepoint, 0x1EA6, 0x1EA6) ||
           in_range(codepoint, 0x1EA8, 0x1EA8) ||
           in_range(codepoint, 0x1EAA, 0x1EAA) ||
           in_range(codepoint, 0x1EAC, 0x1EAC) ||
           in_range(codepoint, 0x1EAE, 0x1EAE) ||
           in_range(codepoint, 0x1EB0, 0x1EB0) ||
           in_range(codepoint, 0x1EB2, 0x1EB2) ||
           in_range(codepoint, 0x1EB4, 0x1EB4) ||
           in_range(codepoint, 0x1EB6, 0x1EB6) ||
           in_range(codepoint, 0x1EB8, 0x1EB8) ||
           in_range(codepoint, 0x1EBA, 0x1EBA) ||
           in_range(codepoint, 0x1EBC, 0x1EBC) ||
           in_range(codepoint, 0x1EBE, 0x1EBE) ||
           in_range(codepoint, 0x1EC0, 0x1EC0) ||
           in_range(codepoint, 0x1EC2, 0x1EC2) ||
           in_range(codepoint, 0x1EC4, 0x1EC4) ||
           in_range(codepoint, 0x1EC6, 0x1EC6) ||
           in_range(codepoint, 0x1EC8, 0x1EC8) ||
           in_range(codepoint, 0x1ECA, 0x1ECA) ||
           in_range(codepoint, 0x1ECC, 0x1ECC) ||
           in_range(codepoint, 0x1ECE, 0x1ECE) ||
           in_range(codepoint, 0x1ED0, 0x1ED0) ||
           in_range(codepoint, 0x1ED2, 0x1ED2) ||
           in_range(codepoint, 0x1ED4, 0x1ED4) ||
           in_range(codepoint, 0x1ED6, 0x1ED6) ||
           in_range(codepoint, 0x1ED8, 0x1ED8) ||
           in_range(codepoint, 0x1EDA, 0x1EDA) ||
           in_range(codepoint, 0x1EDC, 0x1EDC) ||
           in_range(codepoint, 0x1EDE, 0x1EDE) ||
           in_range(codepoint, 0x1EE0, 0x1EE0) ||
           in_range(codepoint, 0x1EE2, 0x1EE2) ||
           in_range(codepoint, 0x1EE4, 0x1EE4) ||
           in_range(codepoint, 0x1EE6, 0x1EE6) ||
           in_range(codepoint, 0x1EE8, 0x1EE8) ||
           in_range(codepoint, 0x1EEA, 0x1EEA) ||
           in_range(codepoint, 0x1EEC, 0x1EEC) ||
           in_range(codepoint, 0x1EEE, 0x1EEE) ||
           in_range(codepoint, 0x1EF0, 0x1EF0) ||
           in_range(codepoint, 0x1EF2, 0x1EF2) ||
           in_range(codepoint, 0x1EF4, 0x1EF4) ||
           in_range(codepoint, 0x1EF6, 0x1EF6) ||
           in_range(codepoint, 0x1EF8, 0x1EF8) ||
           in_range(codepoint, 0x1EFA, 0x1EFA) ||
           in_range(codepoint, 0x1EFC, 0x1EFC) ||
           in_range(codepoint, 0x1EFE, 0x1EFE) ||
           in_range(codepoint, 0x1F08, 0x1F0F) ||
           in_range(codepoint, 0x1F18, 0x1F1D) ||
           in_range(codepoint, 0x1F28, 0x1F2F) ||
           in_range(codepoint, 0x1F38, 0x1F3F) ||
           in_range(codepoint, 0x1F48, 0x1F4D) ||
           in_range(codepoint, 0x1F59, 0x1F59) ||
           in_range(codepoint, 0x1F5B, 0x1F5B) ||
           in_range(codepoint, 0x1F5D, 0x1F5D) ||
           in_range(codepoint, 0x1F5F, 0x1F5F) ||
           in_range(codepoint, 0x1F68, 0x1F6F) ||
           in_range(codepoint, 0x1FB8, 0x1FBB) ||
           in_range(codepoint, 0x1FC8, 0x1FCB) ||
           in_range(codepoint, 0x1FD8, 0x1FDB) ||
           in_range(codepoint, 0x1FE8, 0x1FEC) ||
           in_range(codepoint, 0x1FF8, 0x1FFB) ||
           in_range(codepoint, 0x2102, 0x2102) ||
           in_range(codepoint, 0x2107, 0x2107) ||
           in_range(codepoint, 0x210B, 0x210D) ||
           in_range(codepoint, 0x2110, 0x2112) ||
           in_range(codepoint, 0x2115, 0x2115) ||
           in_range(codepoint, 0x2119, 0x211D) ||
           in_range(codepoint, 0x2124, 0x2124) ||
           in_range(codepoint, 0x2126, 0x2126) ||
           in_range(codepoint, 0x2128, 0x2128) ||
           in_range(codepoint, 0x212A, 0x212D) ||
           in_range(codepoint, 0x2130, 0x2133) ||
           in_range(codepoint, 0x213E, 0x213F) ||
           in_range(codepoint, 0x2145, 0x2145) ||
           in_range(codepoint, 0x2183, 0x2183) ||
           in_range(codepoint, 0x2C00, 0x2C2E) ||
           in_range(codepoint, 0x2C60, 0x2C60) ||
           in_range(codepoint, 0x2C62, 0x2C64) ||
           in_range(codepoint, 0x2C67, 0x2C67) ||
           in_range(codepoint, 0x2C69, 0x2C69) ||
           in_range(codepoint, 0x2C6B, 0x2C6B) ||
           in_range(codepoint, 0x2C6D, 0x2C70) ||
           in_range(codepoint, 0x2C72, 0x2C72) ||
           in_range(codepoint, 0x2C75, 0x2C75) ||
           in_range(codepoint, 0x2C7E, 0x2C80) ||
           in_range(codepoint, 0x2C82, 0x2C82) ||
           in_range(codepoint, 0x2C84, 0x2C84) ||
           in_range(codepoint, 0x2C86, 0x2C86) ||
           in_range(codepoint, 0x2C88, 0x2C88) ||
           in_range(codepoint, 0x2C8A, 0x2C8A) ||
           in_range(codepoint, 0x2C8C, 0x2C8C) ||
           in_range(codepoint, 0x2C8E, 0x2C8E) ||
           in_range(codepoint, 0x2C90, 0x2C90) ||
           in_range(codepoint, 0x2C92, 0x2C92) ||
           in_range(codepoint, 0x2C94, 0x2C94) ||
           in_range(codepoint, 0x2C96, 0x2C96) ||
           in_range(codepoint, 0x2C98, 0x2C98) ||
           in_range(codepoint, 0x2C9A, 0x2C9A) ||
           in_range(codepoint, 0x2C9C, 0x2C9C) ||
           in_range(codepoint, 0x2C9E, 0x2C9E) ||
           in_range(codepoint, 0x2CA0, 0x2CA0) ||
           in_range(codepoint, 0x2CA2, 0x2CA2) ||
           in_range(codepoint, 0x2CA4, 0x2CA4) ||
           in_range(codepoint, 0x2CA6, 0x2CA6) ||
           in_range(codepoint, 0x2CA8, 0x2CA8) ||
           in_range(codepoint, 0x2CAA, 0x2CAA) ||
           in_range(codepoint, 0x2CAC, 0x2CAC) ||
           in_range(codepoint, 0x2CAE, 0x2CAE) ||
           in_range(codepoint, 0x2CB0, 0x2CB0) ||
           in_range(codepoint, 0x2CB2, 0x2CB2) ||
           in_range(codepoint, 0x2CB4, 0x2CB4) ||
           in_range(codepoint, 0x2CB6, 0x2CB6) ||
           in_range(codepoint, 0x2CB8, 0x2CB8) ||
           in_range(codepoint, 0x2CBA, 0x2CBA) ||
           in_range(codepoint, 0x2CBC, 0x2CBC) ||
           in_range(codepoint, 0x2CBE, 0x2CBE) ||
           in_range(codepoint, 0x2CC0, 0x2CC0) ||
           in_range(codepoint, 0x2CC2, 0x2CC2) ||
           in_range(codepoint, 0x2CC4, 0x2CC4) ||
           in_range(codepoint, 0x2CC6, 0x2CC6) ||
           in_range(codepoint, 0x2CC8, 0x2CC8) ||
           in_range(codepoint, 0x2CCA, 0x2CCA) ||
           in_range(codepoint, 0x2CCC, 0x2CCC) ||
           in_range(codepoint, 0x2CCE, 0x2CCE) ||
           in_range(codepoint, 0x2CD0, 0x2CD0) ||
           in_range(codepoint, 0x2CD2, 0x2CD2) ||
           in_range(codepoint, 0x2CD4, 0x2CD4) ||
           in_range(codepoint, 0x2CD6, 0x2CD6) ||
           in_range(codepoint, 0x2CD8, 0x2CD8) ||
           in_range(codepoint, 0x2CDA, 0x2CDA) ||
           in_range(codepoint, 0x2CDC, 0x2CDC) ||
           in_range(codepoint, 0x2CDE, 0x2CDE) ||
           in_range(codepoint, 0x2CE0, 0x2CE0) ||
           in_range(codepoint, 0x2CE2, 0x2CE2) ||
           in_range(codepoint, 0x2CEB, 0x2CEB) ||
           in_range(codepoint, 0x2CED, 0x2CED) ||
           in_range(codepoint, 0x2CF2, 0x2CF2) ||
           in_range(codepoint, 0xA640, 0xA640) ||
           in_range(codepoint, 0xA642, 0xA642) ||
           in_range(codepoint, 0xA644, 0xA644) ||
           in_range(codepoint, 0xA646, 0xA646) ||
           in_range(codepoint, 0xA648, 0xA648) ||
           in_range(codepoint, 0xA64A, 0xA64A) ||
           in_range(codepoint, 0xA64C, 0xA64C) ||
           in_range(codepoint, 0xA64E, 0xA64E) ||
           in_range(codepoint, 0xA650, 0xA650) ||
           in_range(codepoint, 0xA652, 0xA652) ||
           in_range(codepoint, 0xA654, 0xA654) ||
           in_range(codepoint, 0xA656, 0xA656) ||
           in_range(codepoint, 0xA658, 0xA658) ||
           in_range(codepoint, 0xA65A, 0xA65A) ||
           in_range(codepoint, 0xA65C, 0xA65C) ||
           in_range(codepoint, 0xA65E, 0xA65E) ||
           in_range(codepoint, 0xA660, 0xA660) ||
           in_range(codepoint, 0xA662, 0xA662) ||
           in_range(codepoint, 0xA664, 0xA664) ||
           in_range(codepoint, 0xA666, 0xA666) ||
           in_range(codepoint, 0xA668, 0xA668) ||
           in_range(codepoint, 0xA66A, 0xA66A) ||
           in_range(codepoint, 0xA66C, 0xA66C) ||
           in_range(codepoint, 0xA680, 0xA680) ||
           in_range(codepoint, 0xA682, 0xA682) ||
           in_range(codepoint, 0xA684, 0xA684) ||
           in_range(codepoint, 0xA686, 0xA686) ||
           in_range(codepoint, 0xA688, 0xA688) ||
           in_range(codepoint, 0xA68A, 0xA68A) ||
           in_range(codepoint, 0xA68C, 0xA68C) ||
           in_range(codepoint, 0xA68E, 0xA68E) ||
           in_range(codepoint, 0xA690, 0xA690) ||
           in_range(codepoint, 0xA692, 0xA692) ||
           in_range(codepoint, 0xA694, 0xA694) ||
           in_range(codepoint, 0xA696, 0xA696) ||
           in_range(codepoint, 0xA698, 0xA698) ||
           in_range(codepoint, 0xA69A, 0xA69A) ||
           in_range(codepoint, 0xA722, 0xA722) ||
           in_range(codepoint, 0xA724, 0xA724) ||
           in_range(codepoint, 0xA726, 0xA726) ||
           in_range(codepoint, 0xA728, 0xA728) ||
           in_range(codepoint, 0xA72A, 0xA72A) ||
           in_range(codepoint, 0xA72C, 0xA72C) ||
           in_range(codepoint, 0xA72E, 0xA72E) ||
           in_range(codepoint, 0xA732, 0xA732) ||
           in_range(codepoint, 0xA734, 0xA734) ||
           in_range(codepoint, 0xA736, 0xA736) ||
           in_range(codepoint, 0xA738, 0xA738) ||
           in_range(codepoint, 0xA73A, 0xA73A) ||
           in_range(codepoint, 0xA73C, 0xA73C) ||
           in_range(codepoint, 0xA73E, 0xA73E) ||
           in_range(codepoint, 0xA740, 0xA740) ||
           in_range(codepoint, 0xA742, 0xA742) ||
           in_range(codepoint, 0xA744, 0xA744) ||
           in_range(codepoint, 0xA746, 0xA746) ||
           in_range(codepoint, 0xA748, 0xA748) ||
           in_range(codepoint, 0xA74A, 0xA74A) ||
           in_range(codepoint, 0xA74C, 0xA74C) ||
           in_range(codepoint, 0xA74E, 0xA74E) ||
           in_range(codepoint, 0xA750, 0xA750) ||
           in_range(codepoint, 0xA752, 0xA752) ||
           in_range(codepoint, 0xA754, 0xA754) ||
           in_range(codepoint, 0xA756, 0xA756) ||
           in_range(codepoint, 0xA758, 0xA758) ||
           in_range(codepoint, 0xA75A, 0xA75A) ||
           in_range(codepoint, 0xA75C, 0xA75C) ||
           in_range(codepoint, 0xA75E, 0xA75E) ||
           in_range(codepoint, 0xA760, 0xA760) ||
           in_range(codepoint, 0xA762, 0xA762) ||
           in_range(codepoint, 0xA764, 0xA764) ||
           in_range(codepoint, 0xA766, 0xA766) ||
           in_range(codepoint, 0xA768, 0xA768) ||
           in_range(codepoint, 0xA76A, 0xA76A) ||
           in_range(codepoint, 0xA76C, 0xA76C) ||
           in_range(codepoint, 0xA76E, 0xA76E) ||
           in_range(codepoint, 0xA779, 0xA779) ||
           in_range(codepoint, 0xA77B, 0xA77B) ||
           in_range(codepoint, 0xA77D, 0xA77E) ||
           in_range(codepoint, 0xA780, 0xA780) ||
           in_range(codepoint, 0xA782, 0xA782) ||
           in_range(codepoint, 0xA784, 0xA784) ||
           in_range(codepoint, 0xA786, 0xA786) ||
           in_range(codepoint, 0xA78B, 0xA78B) ||
           in_range(codepoint, 0xA78D, 0xA78D) ||
           in_range(codepoint, 0xA790, 0xA790) ||
           in_range(codepoint, 0xA792, 0xA792) ||
           in_range(codepoint, 0xA796, 0xA796) ||
           in_range(codepoint, 0xA798, 0xA798) ||
           in_range(codepoint, 0xA79A, 0xA79A) ||
           in_range(codepoint, 0xA79C, 0xA79C) ||
           in_range(codepoint, 0xA79E, 0xA79E) ||
           in_range(codepoint, 0xA7A0, 0xA7A0) ||
           in_range(codepoint, 0xA7A2, 0xA7A2) ||
           in_range(codepoint, 0xA7A4, 0xA7A4) ||
           in_range(codepoint, 0xA7A6, 0xA7A6) ||
           in_range(codepoint, 0xA7A8, 0xA7A8) ||
           in_range(codepoint, 0xA7AA, 0xA7AE) ||
           in_range(codepoint, 0xA7B0, 0xA7B4) ||
           in_range(codepoint, 0xA7B6, 0xA7B6) ||
           in_range(codepoint, 0xA7B8, 0xA7B8) ||
           in_range(codepoint, 0xA7BA, 0xA7BA) ||
           in_range(codepoint, 0xA7BC, 0xA7BC) ||
           in_range(codepoint, 0xA7BE, 0xA7BE) ||
           in_range(codepoint, 0xA7C2, 0xA7C2) ||
           in_range(codepoint, 0xA7C4, 0xA7C7) ||
           in_range(codepoint, 0xA7C9, 0xA7C9) ||
           in_range(codepoint, 0xA7F5, 0xA7F5) ||
           in_range(codepoint, 0xFF21, 0xFF3A) ||
           in_range(codepoint, 0x10400, 0x10427) ||
           in_range(codepoint, 0x104B0, 0x104D3) ||
           in_range(codepoint, 0x10C80, 0x10CB2) ||
           in_range(codepoint, 0x118A0, 0x118BF) ||
           in_range(codepoint, 0x16E40, 0x16E5F) ||
           in_range(codepoint, 0x1D400, 0x1D419) ||
           in_range(codepoint, 0x1D434, 0x1D44D) ||
           in_range(codepoint, 0x1D468, 0x1D481) ||
           in_range(codepoint, 0x1D49C, 0x1D49C) ||
           in_range(codepoint, 0x1D49E, 0x1D49F) ||
           in_range(codepoint, 0x1D4A2, 0x1D4A2) ||
           in_range(codepoint, 0x1D4A5, 0x1D4A6) ||
           in_range(codepoint, 0x1D4A9, 0x1D4AC) ||
           in_range(codepoint, 0x1D4AE, 0x1D4B5) ||
           in_range(codepoint, 0x1D4D0, 0x1D4E9) ||
           in_range(codepoint, 0x1D504, 0x1D505) ||
           in_range(codepoint, 0x1D507, 0x1D50A) ||
           in_range(codepoint, 0x1D50D, 0x1D514) ||
           in_range(codepoint, 0x1D516, 0x1D51C) ||
           in_range(codepoint, 0x1D538, 0x1D539) ||
           in_range(codepoint, 0x1D53B, 0x1D53E) ||
           in_range(codepoint, 0x1D540, 0x1D544) ||
           in_range(codepoint, 0x1D546, 0x1D546) ||
           in_range(codepoint, 0x1D54A, 0x1D550) ||
           in_range(codepoint, 0x1D56C, 0x1D585) ||
           in_range(codepoint, 0x1D5A0, 0x1D5B9) ||
           in_range(codepoint, 0x1D5D4, 0x1D5ED) ||
           in_range(codepoint, 0x1D608, 0x1D621) ||
           in_range(codepoint, 0x1D63C, 0x1D655) ||
           in_range(codepoint, 0x1D670, 0x1D689) ||
           in_range(codepoint, 0x1D6A8, 0x1D6C0) ||
           in_range(codepoint, 0x1D6E2, 0x1D6FA) ||
           in_range(codepoint, 0x1D71C, 0x1D734) ||
           in_range(codepoint, 0x1D756, 0x1D76E) ||
           in_range(codepoint, 0x1D790, 0x1D7A8) ||
           in_range(codepoint, 0x1D7CA, 0x1D7CA) ||
           in_range(codepoint, 0x1E900, 0x1E921);
}

inline bool text::unicode_category_mc(datum_type codepoint)
{
    return in_range(codepoint, 0x0903, 0x0903) ||
           in_range(codepoint, 0x093B, 0x093B) ||
           in_range(codepoint, 0x093E, 0x0940) ||
           in_range(codepoint, 0x0949, 0x094C) ||
           in_range(codepoint, 0x094E, 0x094F) ||
           in_range(codepoint, 0x0982, 0x0983) ||
           in_range(codepoint, 0x09BE, 0x09C0) ||
           in_range(codepoint, 0x09C7, 0x09C8) ||
           in_range(codepoint, 0x09CB, 0x09CC) ||
           in_range(codepoint, 0x09D7, 0x09D7) ||
           in_range(codepoint, 0x0A03, 0x0A03) ||
           in_range(codepoint, 0x0A3E, 0x0A40) ||
           in_range(codepoint, 0x0A83, 0x0A83) ||
           in_range(codepoint, 0x0ABE, 0x0AC0) ||
           in_range(codepoint, 0x0AC9, 0x0AC9) ||
           in_range(codepoint, 0x0ACB, 0x0ACC) ||
           in_range(codepoint, 0x0B02, 0x0B03) ||
           in_range(codepoint, 0x0B3E, 0x0B3E) ||
           in_range(codepoint, 0x0B40, 0x0B40) ||
           in_range(codepoint, 0x0B47, 0x0B48) ||
           in_range(codepoint, 0x0B4B, 0x0B4C) ||
           in_range(codepoint, 0x0B57, 0x0B57) ||
           in_range(codepoint, 0x0BBE, 0x0BBF) ||
           in_range(codepoint, 0x0BC1, 0x0BC2) ||
           in_range(codepoint, 0x0BC6, 0x0BC8) ||
           in_range(codepoint, 0x0BCA, 0x0BCC) ||
           in_range(codepoint, 0x0BD7, 0x0BD7) ||
           in_range(codepoint, 0x0C01, 0x0C03) ||
           in_range(codepoint, 0x0C41, 0x0C44) ||
           in_range(codepoint, 0x0C82, 0x0C83) ||
           in_range(codepoint, 0x0CBE, 0x0CBE) ||
           in_range(codepoint, 0x0CC0, 0x0CC4) ||
           in_range(codepoint, 0x0CC7, 0x0CC8) ||
           in_range(codepoint, 0x0CCA, 0x0CCB) ||
           in_range(codepoint, 0x0CD5, 0x0CD6) ||
           in_range(codepoint, 0x0D02, 0x0D03) ||
           in_range(codepoint, 0x0D3E, 0x0D40) ||
           in_range(codepoint, 0x0D46, 0x0D48) ||
           in_range(codepoint, 0x0D4A, 0x0D4C) ||
           in_range(codepoint, 0x0D57, 0x0D57) ||
           in_range(codepoint, 0x0D82, 0x0D83) ||
           in_range(codepoint, 0x0DCF, 0x0DD1) ||
           in_range(codepoint, 0x0DD8, 0x0DDF) ||
           in_range(codepoint, 0x0DF2, 0x0DF3) ||
           in_range(codepoint, 0x0F3E, 0x0F3F) ||
           in_range(codepoint, 0x0F7F, 0x0F7F) ||
           in_range(codepoint, 0x102B, 0x102C) ||
           in_range(codepoint, 0x1031, 0x1031) ||
           in_range(codepoint, 0x1038, 0x1038) ||
           in_range(codepoint, 0x103B, 0x103C) ||
           in_range(codepoint, 0x1056, 0x1057) ||
           in_range(codepoint, 0x1062, 0x1064) ||
           in_range(codepoint, 0x1067, 0x106D) ||
           in_range(codepoint, 0x1083, 0x1084) ||
           in_range(codepoint, 0x1087, 0x108C) ||
           in_range(codepoint, 0x108F, 0x108F) ||
           in_range(codepoint, 0x109A, 0x109C) ||
           in_range(codepoint, 0x17B6, 0x17B6) ||
           in_range(codepoint, 0x17BE, 0x17C5) ||
           in_range(codepoint, 0x17C7, 0x17C8) ||
           in_range(codepoint, 0x1923, 0x1926) ||
           in_range(codepoint, 0x1929, 0x192B) ||
           in_range(codepoint, 0x1930, 0x1931) ||
           in_range(codepoint, 0x1933, 0x1938) ||
           in_range(codepoint, 0x1A19, 0x1A1A) ||
           in_range(codepoint, 0x1A55, 0x1A55) ||
           in_range(codepoint, 0x1A57, 0x1A57) ||
           in_range(codepoint, 0x1A61, 0x1A61) ||
           in_range(codepoint, 0x1A63, 0x1A64) ||
           in_range(codepoint, 0x1A6D, 0x1A72) ||
           in_range(codepoint, 0x1B04, 0x1B04) ||
           in_range(codepoint, 0x1B35, 0x1B35) ||
           in_range(codepoint, 0x1B3B, 0x1B3B) ||
           in_range(codepoint, 0x1B3D, 0x1B41) ||
           in_range(codepoint, 0x1B43, 0x1B44) ||
           in_range(codepoint, 0x1B82, 0x1B82) ||
           in_range(codepoint, 0x1BA1, 0x1BA1) ||
           in_range(codepoint, 0x1BA6, 0x1BA7) ||
           in_range(codepoint, 0x1BAA, 0x1BAA) ||
           in_range(codepoint, 0x1BE7, 0x1BE7) ||
           in_range(codepoint, 0x1BEA, 0x1BEC) ||
           in_range(codepoint, 0x1BEE, 0x1BEE) ||
           in_range(codepoint, 0x1BF2, 0x1BF3) ||
           in_range(codepoint, 0x1C24, 0x1C2B) ||
           in_range(codepoint, 0x1C34, 0x1C35) ||
           in_range(codepoint, 0x1CE1, 0x1CE1) ||
           in_range(codepoint, 0x1CF7, 0x1CF7) ||
           in_range(codepoint, 0x302E, 0x302F) ||
           in_range(codepoint, 0xA823, 0xA824) ||
           in_range(codepoint, 0xA827, 0xA827) ||
           in_range(codepoint, 0xA880, 0xA881) ||
           in_range(codepoint, 0xA8B4, 0xA8C3) ||
           in_range(codepoint, 0xA952, 0xA953) ||
           in_range(codepoint, 0xA983, 0xA983) ||
           in_range(codepoint, 0xA9B4, 0xA9B5) ||
           in_range(codepoint, 0xA9BA, 0xA9BB) ||
           in_range(codepoint, 0xA9BE, 0xA9C0) ||
           in_range(codepoint, 0xAA2F, 0xAA30) ||
           in_range(codepoint, 0xAA33, 0xAA34) ||
           in_range(codepoint, 0xAA4D, 0xAA4D) ||
           in_range(codepoint, 0xAA7B, 0xAA7B) ||
           in_range(codepoint, 0xAA7D, 0xAA7D) ||
           in_range(codepoint, 0xAAEB, 0xAAEB) ||
           in_range(codepoint, 0xAAEE, 0xAAEF) ||
           in_range(codepoint, 0xAAF5, 0xAAF5) ||
           in_range(codepoint, 0xABE3, 0xABE4) ||
           in_range(codepoint, 0xABE6, 0xABE7) ||
           in_range(codepoint, 0xABE9, 0xABEA) ||
           in_range(codepoint, 0xABEC, 0xABEC) ||
           in_range(codepoint, 0x11000, 0x11000) ||
           in_range(codepoint, 0x11002, 0x11002) ||
           in_range(codepoint, 0x11082, 0x11082) ||
           in_range(codepoint, 0x110B0, 0x110B2) ||
           in_range(codepoint, 0x110B7, 0x110B8) ||
           in_range(codepoint, 0x1112C, 0x1112C) ||
           in_range(codepoint, 0x11145, 0x11146) ||
           in_range(codepoint, 0x11182, 0x11182) ||
           in_range(codepoint, 0x111B3, 0x111B5) ||
           in_range(codepoint, 0x111BF, 0x111C0) ||
           in_range(codepoint, 0x111CE, 0x111CE) ||
           in_range(codepoint, 0x1122C, 0x1122E) ||
           in_range(codepoint, 0x11232, 0x11233) ||
           in_range(codepoint, 0x11235, 0x11235) ||
           in_range(codepoint, 0x112E0, 0x112E2) ||
           in_range(codepoint, 0x11302, 0x11303) ||
           in_range(codepoint, 0x1133E, 0x1133F) ||
           in_range(codepoint, 0x11341, 0x11344) ||
           in_range(codepoint, 0x11347, 0x11348) ||
           in_range(codepoint, 0x1134B, 0x1134D) ||
           in_range(codepoint, 0x11357, 0x11357) ||
           in_range(codepoint, 0x11362, 0x11363) ||
           in_range(codepoint, 0x11435, 0x11437) ||
           in_range(codepoint, 0x11440, 0x11441) ||
           in_range(codepoint, 0x11445, 0x11445) ||
           in_range(codepoint, 0x114B0, 0x114B2) ||
           in_range(codepoint, 0x114B9, 0x114B9) ||
           in_range(codepoint, 0x114BB, 0x114BE) ||
           in_range(codepoint, 0x114C1, 0x114C1) ||
           in_range(codepoint, 0x115AF, 0x115B1) ||
           in_range(codepoint, 0x115B8, 0x115BB) ||
           in_range(codepoint, 0x115BE, 0x115BE) ||
           in_range(codepoint, 0x11630, 0x11632) ||
           in_range(codepoint, 0x1163B, 0x1163C) ||
           in_range(codepoint, 0x1163E, 0x1163E) ||
           in_range(codepoint, 0x116AC, 0x116AC) ||
           in_range(codepoint, 0x116AE, 0x116AF) ||
           in_range(codepoint, 0x116B6, 0x116B6) ||
           in_range(codepoint, 0x11720, 0x11721) ||
           in_range(codepoint, 0x11726, 0x11726) ||
           in_range(codepoint, 0x1182C, 0x1182E) ||
           in_range(codepoint, 0x11838, 0x11838) ||
           in_range(codepoint, 0x11930, 0x11935) ||
           in_range(codepoint, 0x11937, 0x11938) ||
           in_range(codepoint, 0x1193D, 0x1193D) ||
           in_range(codepoint, 0x11940, 0x11940) ||
           in_range(codepoint, 0x11942, 0x11942) ||
           in_range(codepoint, 0x119D1, 0x119D3) ||
           in_range(codepoint, 0x119DC, 0x119DF) ||
           in_range(codepoint, 0x119E4, 0x119E4) ||
           in_range(codepoint, 0x11A39, 0x11A39) ||
           in_range(codepoint, 0x11A57, 0x11A58) ||
           in_range(codepoint, 0x11A97, 0x11A97) ||
           in_range(codepoint, 0x11C2F, 0x11C2F) ||
           in_range(codepoint, 0x11C3E, 0x11C3E) ||
           in_range(codepoint, 0x11CA9, 0x11CA9) ||
           in_range(codepoint, 0x11CB1, 0x11CB1) ||
           in_range(codepoint, 0x11CB4, 0x11CB4) ||
           in_range(codepoint, 0x11D8A, 0x11D8E) ||
           in_range(codepoint, 0x11D93, 0x11D94) ||
           in_range(codepoint, 0x11D96, 0x11D96) ||
           in_range(codepoint, 0x11EF5, 0x11EF6) ||
           in_range(codepoint, 0x16F51, 0x16F87) ||
           in_range(codepoint, 0x16FF0, 0x16FF1) ||
           in_range(codepoint, 0x1D165, 0x1D166) ||
           in_range(codepoint, 0x1D16D, 0x1D172);
}

inline bool text::unicode_category_mn(datum_type codepoint)
{
    return in_range(codepoint, 0x0300, 0x036F) ||
           in_range(codepoint, 0x0483, 0x0487) ||
           in_range(codepoint, 0x0591, 0x05BD) ||
           in_range(codepoint, 0x05BF, 0x05BF) ||
           in_range(codepoint, 0x05C1, 0x05C2) ||
           in_range(codepoint, 0x05C4, 0x05C5) ||
           in_range(codepoint, 0x05C7, 0x05C7) ||
           in_range(codepoint, 0x0610, 0x061A) ||
           in_range(codepoint, 0x064B, 0x065F) ||
           in_range(codepoint, 0x0670, 0x0670) ||
           in_range(codepoint, 0x06D6, 0x06DC) ||
           in_range(codepoint, 0x06DF, 0x06E4) ||
           in_range(codepoint, 0x06E7, 0x06E8) ||
           in_range(codepoint, 0x06EA, 0x06ED) ||
           in_range(codepoint, 0x0711, 0x0711) ||
           in_range(codepoint, 0x0730, 0x074A) ||
           in_range(codepoint, 0x07A6, 0x07B0) ||
           in_range(codepoint, 0x07EB, 0x07F3) ||
           in_range(codepoint, 0x07FD, 0x07FD) ||
           in_range(codepoint, 0x0816, 0x0819) ||
           in_range(codepoint, 0x081B, 0x0823) ||
           in_range(codepoint, 0x0825, 0x0827) ||
           in_range(codepoint, 0x0829, 0x082D) ||
           in_range(codepoint, 0x0859, 0x085B) ||
           in_range(codepoint, 0x08D3, 0x08E1) ||
           in_range(codepoint, 0x08E3, 0x0902) ||
           in_range(codepoint, 0x093A, 0x093A) ||
           in_range(codepoint, 0x093C, 0x093C) ||
           in_range(codepoint, 0x0941, 0x0948) ||
           in_range(codepoint, 0x094D, 0x094D) ||
           in_range(codepoint, 0x0951, 0x0957) ||
           in_range(codepoint, 0x0962, 0x0963) ||
           in_range(codepoint, 0x0981, 0x0981) ||
           in_range(codepoint, 0x09BC, 0x09BC) ||
           in_range(codepoint, 0x09C1, 0x09C4) ||
           in_range(codepoint, 0x09CD, 0x09CD) ||
           in_range(codepoint, 0x09E2, 0x09E3) ||
           in_range(codepoint, 0x09FE, 0x09FE) ||
           in_range(codepoint, 0x0A01, 0x0A02) ||
           in_range(codepoint, 0x0A3C, 0x0A3C) ||
           in_range(codepoint, 0x0A41, 0x0A42) ||
           in_range(codepoint, 0x0A47, 0x0A48) ||
           in_range(codepoint, 0x0A4B, 0x0A4D) ||
           in_range(codepoint, 0x0A51, 0x0A51) ||
           in_range(codepoint, 0x0A70, 0x0A71) ||
           in_range(codepoint, 0x0A75, 0x0A75) ||
           in_range(codepoint, 0x0A81, 0x0A82) ||
           in_range(codepoint, 0x0ABC, 0x0ABC) ||
           in_range(codepoint, 0x0AC1, 0x0AC5) ||
           in_range(codepoint, 0x0AC7, 0x0AC8) ||
           in_range(codepoint, 0x0ACD, 0x0ACD) ||
           in_range(codepoint, 0x0AE2, 0x0AE3) ||
           in_range(codepoint, 0x0AFA, 0x0AFF) ||
           in_range(codepoint, 0x0B01, 0x0B01) ||
           in_range(codepoint, 0x0B3C, 0x0B3C) ||
           in_range(codepoint, 0x0B3F, 0x0B3F) ||
           in_range(codepoint, 0x0B41, 0x0B44) ||
           in_range(codepoint, 0x0B4D, 0x0B4D) ||
           in_range(codepoint, 0x0B55, 0x0B56) ||
           in_range(codepoint, 0x0B62, 0x0B63) ||
           in_range(codepoint, 0x0B82, 0x0B82) ||
           in_range(codepoint, 0x0BC0, 0x0BC0) ||
           in_range(codepoint, 0x0BCD, 0x0BCD) ||
           in_range(codepoint, 0x0C00, 0x0C00) ||
           in_range(codepoint, 0x0C04, 0x0C04) ||
           in_range(codepoint, 0x0C3E, 0x0C40) ||
           in_range(codepoint, 0x0C46, 0x0C48) ||
           in_range(codepoint, 0x0C4A, 0x0C4D) ||
           in_range(codepoint, 0x0C55, 0x0C56) ||
           in_range(codepoint, 0x0C62, 0x0C63) ||
           in_range(codepoint, 0x0C81, 0x0C81) ||
           in_range(codepoint, 0x0CBC, 0x0CBC) ||
           in_range(codepoint, 0x0CBF, 0x0CBF) ||
           in_range(codepoint, 0x0CC6, 0x0CC6) ||
           in_range(codepoint, 0x0CCC, 0x0CCD) ||
           in_range(codepoint, 0x0CE2, 0x0CE3) ||
           in_range(codepoint, 0x0D00, 0x0D01) ||
           in_range(codepoint, 0x0D3B, 0x0D3C) ||
           in_range(codepoint, 0x0D41, 0x0D44) ||
           in_range(codepoint, 0x0D4D, 0x0D4D) ||
           in_range(codepoint, 0x0D62, 0x0D63) ||
           in_range(codepoint, 0x0D81, 0x0D81) ||
           in_range(codepoint, 0x0DCA, 0x0DCA) ||
           in_range(codepoint, 0x0DD2, 0x0DD4) ||
           in_range(codepoint, 0x0DD6, 0x0DD6) ||
           in_range(codepoint, 0x0E31, 0x0E31) ||
           in_range(codepoint, 0x0E34, 0x0E3A) ||
           in_range(codepoint, 0x0E47, 0x0E4E) ||
           in_range(codepoint, 0x0EB1, 0x0EB1) ||
           in_range(codepoint, 0x0EB4, 0x0EBC) ||
           in_range(codepoint, 0x0EC8, 0x0ECD) ||
           in_range(codepoint, 0x0F18, 0x0F19) ||
           in_range(codepoint, 0x0F35, 0x0F35) ||
           in_range(codepoint, 0x0F37, 0x0F37) ||
           in_range(codepoint, 0x0F39, 0x0F39) ||
           in_range(codepoint, 0x0F71, 0x0F7E) ||
           in_range(codepoint, 0x0F80, 0x0F84) ||
           in_range(codepoint, 0x0F86, 0x0F87) ||
           in_range(codepoint, 0x0F8D, 0x0F97) ||
           in_range(codepoint, 0x0F99, 0x0FBC) ||
           in_range(codepoint, 0x0FC6, 0x0FC6) ||
           in_range(codepoint, 0x102D, 0x1030) ||
           in_range(codepoint, 0x1032, 0x1037) ||
           in_range(codepoint, 0x1039, 0x103A) ||
           in_range(codepoint, 0x103D, 0x103E) ||
           in_range(codepoint, 0x1058, 0x1059) ||
           in_range(codepoint, 0x105E, 0x1060) ||
           in_range(codepoint, 0x1071, 0x1074) ||
           in_range(codepoint, 0x1082, 0x1082) ||
           in_range(codepoint, 0x1085, 0x1086) ||
           in_range(codepoint, 0x108D, 0x108D) ||
           in_range(codepoint, 0x109D, 0x109D) ||
           in_range(codepoint, 0x135D, 0x135F) ||
           in_range(codepoint, 0x1712, 0x1714) ||
           in_range(codepoint, 0x1732, 0x1734) ||
           in_range(codepoint, 0x1752, 0x1753) ||
           in_range(codepoint, 0x1772, 0x1773) ||
           in_range(codepoint, 0x17B4, 0x17B5) ||
           in_range(codepoint, 0x17B7, 0x17BD) ||
           in_range(codepoint, 0x17C6, 0x17C6) ||
           in_range(codepoint, 0x17C9, 0x17D3) ||
           in_range(codepoint, 0x17DD, 0x17DD) ||
           in_range(codepoint, 0x180B, 0x180D) ||
           in_range(codepoint, 0x1885, 0x1886) ||
           in_range(codepoint, 0x18A9, 0x18A9) ||
           in_range(codepoint, 0x1920, 0x1922) ||
           in_range(codepoint, 0x1927, 0x1928) ||
           in_range(codepoint, 0x1932, 0x1932) ||
           in_range(codepoint, 0x1939, 0x193B) ||
           in_range(codepoint, 0x1A17, 0x1A18) ||
           in_range(codepoint, 0x1A1B, 0x1A1B) ||
           in_range(codepoint, 0x1A56, 0x1A56) ||
           in_range(codepoint, 0x1A58, 0x1A5E) ||
           in_range(codepoint, 0x1A60, 0x1A60) ||
           in_range(codepoint, 0x1A62, 0x1A62) ||
           in_range(codepoint, 0x1A65, 0x1A6C) ||
           in_range(codepoint, 0x1A73, 0x1A7C) ||
           in_range(codepoint, 0x1A7F, 0x1A7F) ||
           in_range(codepoint, 0x1AB0, 0x1ABD) ||
           in_range(codepoint, 0x1ABF, 0x1AC0) ||
           in_range(codepoint, 0x1B00, 0x1B03) ||
           in_range(codepoint, 0x1B34, 0x1B34) ||
           in_range(codepoint, 0x1B36, 0x1B3A) ||
           in_range(codepoint, 0x1B3C, 0x1B3C) ||
           in_range(codepoint, 0x1B42, 0x1B42) ||
           in_range(codepoint, 0x1B6B, 0x1B73) ||
           in_range(codepoint, 0x1B80, 0x1B81) ||
           in_range(codepoint, 0x1BA2, 0x1BA5) ||
           in_range(codepoint, 0x1BA8, 0x1BA9) ||
           in_range(codepoint, 0x1BAB, 0x1BAD) ||
           in_range(codepoint, 0x1BE6, 0x1BE6) ||
           in_range(codepoint, 0x1BE8, 0x1BE9) ||
           in_range(codepoint, 0x1BED, 0x1BED) ||
           in_range(codepoint, 0x1BEF, 0x1BF1) ||
           in_range(codepoint, 0x1C2C, 0x1C33) ||
           in_range(codepoint, 0x1C36, 0x1C37) ||
           in_range(codepoint, 0x1CD0, 0x1CD2) ||
           in_range(codepoint, 0x1CD4, 0x1CE0) ||
           in_range(codepoint, 0x1CE2, 0x1CE8) ||
           in_range(codepoint, 0x1CED, 0x1CED) ||
           in_range(codepoint, 0x1CF4, 0x1CF4) ||
           in_range(codepoint, 0x1CF8, 0x1CF9) ||
           in_range(codepoint, 0x1DC0, 0x1DF9) ||
           in_range(codepoint, 0x1DFB, 0x1DFF) ||
           in_range(codepoint, 0x20D0, 0x20DC) ||
           in_range(codepoint, 0x20E1, 0x20E1) ||
           in_range(codepoint, 0x20E5, 0x20F0) ||
           in_range(codepoint, 0x2CEF, 0x2CF1) ||
           in_range(codepoint, 0x2D7F, 0x2D7F) ||
           in_range(codepoint, 0x2DE0, 0x2DFF) ||
           in_range(codepoint, 0x302A, 0x302D) ||
           in_range(codepoint, 0x3099, 0x309A) ||
           in_range(codepoint, 0xA66F, 0xA66F) ||
           in_range(codepoint, 0xA674, 0xA67D) ||
           in_range(codepoint, 0xA69E, 0xA69F) ||
           in_range(codepoint, 0xA6F0, 0xA6F1) ||
           in_range(codepoint, 0xA802, 0xA802) ||
           in_range(codepoint, 0xA806, 0xA806) ||
           in_range(codepoint, 0xA80B, 0xA80B) ||
           in_range(codepoint, 0xA825, 0xA826) ||
           in_range(codepoint, 0xA82C, 0xA82C) ||
           in_range(codepoint, 0xA8C4, 0xA8C5) ||
           in_range(codepoint, 0xA8E0, 0xA8F1) ||
           in_range(codepoint, 0xA8FF, 0xA8FF) ||
           in_range(codepoint, 0xA926, 0xA92D) ||
           in_range(codepoint, 0xA947, 0xA951) ||
           in_range(codepoint, 0xA980, 0xA982) ||
           in_range(codepoint, 0xA9B3, 0xA9B3) ||
           in_range(codepoint, 0xA9B6, 0xA9B9) ||
           in_range(codepoint, 0xA9BC, 0xA9BD) ||
           in_range(codepoint, 0xA9E5, 0xA9E5) ||
           in_range(codepoint, 0xAA29, 0xAA2E) ||
           in_range(codepoint, 0xAA31, 0xAA32) ||
           in_range(codepoint, 0xAA35, 0xAA36) ||
           in_range(codepoint, 0xAA43, 0xAA43) ||
           in_range(codepoint, 0xAA4C, 0xAA4C) ||
           in_range(codepoint, 0xAA7C, 0xAA7C) ||
           in_range(codepoint, 0xAAB0, 0xAAB0) ||
           in_range(codepoint, 0xAAB2, 0xAAB4) ||
           in_range(codepoint, 0xAAB7, 0xAAB8) ||
           in_range(codepoint, 0xAABE, 0xAABF) ||
           in_range(codepoint, 0xAAC1, 0xAAC1) ||
           in_range(codepoint, 0xAAEC, 0xAAED) ||
           in_range(codepoint, 0xAAF6, 0xAAF6) ||
           in_range(codepoint, 0xABE5, 0xABE5) ||
           in_range(codepoint, 0xABE8, 0xABE8) ||
           in_range(codepoint, 0xABED, 0xABED) ||
           in_range(codepoint, 0xFB1E, 0xFB1E) ||
           in_range(codepoint, 0xFE00, 0xFE0F) ||
           in_range(codepoint, 0xFE20, 0xFE2F) ||
           in_range(codepoint, 0x101FD, 0x101FD) ||
           in_range(codepoint, 0x102E0, 0x102E0) ||
           in_range(codepoint, 0x10376, 0x1037A) ||
           in_range(codepoint, 0x10A01, 0x10A03) ||
           in_range(codepoint, 0x10A05, 0x10A06) ||
           in_range(codepoint, 0x10A0C, 0x10A0F) ||
           in_range(codepoint, 0x10A38, 0x10A3A) ||
           in_range(codepoint, 0x10A3F, 0x10A3F) ||
           in_range(codepoint, 0x10AE5, 0x10AE6) ||
           in_range(codepoint, 0x10D24, 0x10D27) ||
           in_range(codepoint, 0x10EAB, 0x10EAC) ||
           in_range(codepoint, 0x10F46, 0x10F50) ||
           in_range(codepoint, 0x11001, 0x11001) ||
           in_range(codepoint, 0x11038, 0x11046) ||
           in_range(codepoint, 0x1107F, 0x11081) ||
           in_range(codepoint, 0x110B3, 0x110B6) ||
           in_range(codepoint, 0x110B9, 0x110BA) ||
           in_range(codepoint, 0x11100, 0x11102) ||
           in_range(codepoint, 0x11127, 0x1112B) ||
           in_range(codepoint, 0x1112D, 0x11134) ||
           in_range(codepoint, 0x11173, 0x11173) ||
           in_range(codepoint, 0x11180, 0x11181) ||
           in_range(codepoint, 0x111B6, 0x111BE) ||
           in_range(codepoint, 0x111C9, 0x111CC) ||
           in_range(codepoint, 0x111CF, 0x111CF) ||
           in_range(codepoint, 0x1122F, 0x11231) ||
           in_range(codepoint, 0x11234, 0x11234) ||
           in_range(codepoint, 0x11236, 0x11237) ||
           in_range(codepoint, 0x1123E, 0x1123E) ||
           in_range(codepoint, 0x112DF, 0x112DF) ||
           in_range(codepoint, 0x112E3, 0x112EA) ||
           in_range(codepoint, 0x11300, 0x11301) ||
           in_range(codepoint, 0x1133B, 0x1133C) ||
           in_range(codepoint, 0x11340, 0x11340) ||
           in_range(codepoint, 0x11366, 0x1136C) ||
           in_range(codepoint, 0x11370, 0x11374) ||
           in_range(codepoint, 0x11438, 0x1143F) ||
           in_range(codepoint, 0x11442, 0x11444) ||
           in_range(codepoint, 0x11446, 0x11446) ||
           in_range(codepoint, 0x1145E, 0x1145E) ||
           in_range(codepoint, 0x114B3, 0x114B8) ||
           in_range(codepoint, 0x114BA, 0x114BA) ||
           in_range(codepoint, 0x114BF, 0x114C0) ||
           in_range(codepoint, 0x114C2, 0x114C3) ||
           in_range(codepoint, 0x115B2, 0x115B5) ||
           in_range(codepoint, 0x115BC, 0x115BD) ||
           in_range(codepoint, 0x115BF, 0x115C0) ||
           in_range(codepoint, 0x115DC, 0x115DD) ||
           in_range(codepoint, 0x11633, 0x1163A) ||
           in_range(codepoint, 0x1163D, 0x1163D) ||
           in_range(codepoint, 0x1163F, 0x11640) ||
           in_range(codepoint, 0x116AB, 0x116AB) ||
           in_range(codepoint, 0x116AD, 0x116AD) ||
           in_range(codepoint, 0x116B0, 0x116B5) ||
           in_range(codepoint, 0x116B7, 0x116B7) ||
           in_range(codepoint, 0x1171D, 0x1171F) ||
           in_range(codepoint, 0x11722, 0x11725) ||
           in_range(codepoint, 0x11727, 0x1172B) ||
           in_range(codepoint, 0x1182F, 0x11837) ||
           in_range(codepoint, 0x11839, 0x1183A) ||
           in_range(codepoint, 0x1193B, 0x1193C) ||
           in_range(codepoint, 0x1193E, 0x1193E) ||
           in_range(codepoint, 0x11943, 0x11943) ||
           in_range(codepoint, 0x119D4, 0x119D7) ||
           in_range(codepoint, 0x119DA, 0x119DB) ||
           in_range(codepoint, 0x119E0, 0x119E0) ||
           in_range(codepoint, 0x11A01, 0x11A0A) ||
           in_range(codepoint, 0x11A33, 0x11A38) ||
           in_range(codepoint, 0x11A3B, 0x11A3E) ||
           in_range(codepoint, 0x11A47, 0x11A47) ||
           in_range(codepoint, 0x11A51, 0x11A56) ||
           in_range(codepoint, 0x11A59, 0x11A5B) ||
           in_range(codepoint, 0x11A8A, 0x11A96) ||
           in_range(codepoint, 0x11A98, 0x11A99) ||
           in_range(codepoint, 0x11C30, 0x11C36) ||
           in_range(codepoint, 0x11C38, 0x11C3D) ||
           in_range(codepoint, 0x11C3F, 0x11C3F) ||
           in_range(codepoint, 0x11C92, 0x11CA7) ||
           in_range(codepoint, 0x11CAA, 0x11CB0) ||
           in_range(codepoint, 0x11CB2, 0x11CB3) ||
           in_range(codepoint, 0x11CB5, 0x11CB6) ||
           in_range(codepoint, 0x11D31, 0x11D36) ||
           in_range(codepoint, 0x11D3A, 0x11D3A) ||
           in_range(codepoint, 0x11D3C, 0x11D3D) ||
           in_range(codepoint, 0x11D3F, 0x11D45) ||
           in_range(codepoint, 0x11D47, 0x11D47) ||
           in_range(codepoint, 0x11D90, 0x11D91) ||
           in_range(codepoint, 0x11D95, 0x11D95) ||
           in_range(codepoint, 0x11D97, 0x11D97) ||
           in_range(codepoint, 0x11EF3, 0x11EF4) ||
           in_range(codepoint, 0x16AF0, 0x16AF4) ||
           in_range(codepoint, 0x16B30, 0x16B36) ||
           in_range(codepoint, 0x16F4F, 0x16F4F) ||
           in_range(codepoint, 0x16F8F, 0x16F92) ||
           in_range(codepoint, 0x16FE4, 0x16FE4) ||
           in_range(codepoint, 0x1BC9D, 0x1BC9E) ||
           in_range(codepoint, 0x1D167, 0x1D169) ||
           in_range(codepoint, 0x1D17B, 0x1D182) ||
           in_range(codepoint, 0x1D185, 0x1D18B) ||
           in_range(codepoint, 0x1D1AA, 0x1D1AD) ||
           in_range(codepoint, 0x1D242, 0x1D244) ||
           in_range(codepoint, 0x1DA00, 0x1DA36) ||
           in_range(codepoint, 0x1DA3B, 0x1DA6C) ||
           in_range(codepoint, 0x1DA75, 0x1DA75) ||
           in_range(codepoint, 0x1DA84, 0x1DA84) ||
           in_range(codepoint, 0x1DA9B, 0x1DA9F) ||
           in_range(codepoint, 0x1DAA1, 0x1DAAF) ||
           in_range(codepoint, 0x1E000, 0x1E006) ||
           in_range(codepoint, 0x1E008, 0x1E018) ||
           in_range(codepoint, 0x1E01B, 0x1E021) ||
           in_range(codepoint, 0x1E023, 0x1E024) ||
           in_range(codepoint, 0x1E026, 0x1E02A) ||
           in_range(codepoint, 0x1E130, 0x1E136) ||
           in_range(codepoint, 0x1E2EC, 0x1E2EF) ||
           in_range(codepoint, 0x1E8D0, 0x1E8D6) ||
           in_range(codepoint, 0x1E944, 0x1E94A) ||
           in_range(codepoint, 0xE0100, 0xE01EF);
}

inline bool text::unicode_category_nd(datum_type codepoint)
{
    return in_range(codepoint, 0x0030, 0x0039) ||
           in_range(codepoint, 0x0660, 0x0669) ||
           in_range(codepoint, 0x06F0, 0x06F9) ||
           in_range(codepoint, 0x07C0, 0x07C9) ||
           in_range(codepoint, 0x0966, 0x096F) ||
           in_range(codepoint, 0x09E6, 0x09EF) ||
           in_range(codepoint, 0x0A66, 0x0A6F) ||
           in_range(codepoint, 0x0AE6, 0x0AEF) ||
           in_range(codepoint, 0x0B66, 0x0B6F) ||
           in_range(codepoint, 0x0BE6, 0x0BEF) ||
           in_range(codepoint, 0x0C66, 0x0C6F) ||
           in_range(codepoint, 0x0CE6, 0x0CEF) ||
           in_range(codepoint, 0x0D66, 0x0D6F) ||
           in_range(codepoint, 0x0DE6, 0x0DEF) ||
           in_range(codepoint, 0x0E50, 0x0E59) ||
           in_range(codepoint, 0x0ED0, 0x0ED9) ||
           in_range(codepoint, 0x0F20, 0x0F29) ||
           in_range(codepoint, 0x1040, 0x1049) ||
           in_range(codepoint, 0x1090, 0x1099) ||
           in_range(codepoint, 0x17E0, 0x17E9) ||
           in_range(codepoint, 0x1810, 0x1819) ||
           in_range(codepoint, 0x1946, 0x194F) ||
           in_range(codepoint, 0x19D0, 0x19D9) ||
           in_range(codepoint, 0x1A80, 0x1A89) ||
           in_range(codepoint, 0x1A90, 0x1A99) ||
           in_range(codepoint, 0x1B50, 0x1B59) ||
           in_range(codepoint, 0x1BB0, 0x1BB9) ||
           in_range(codepoint, 0x1C40, 0x1C49) ||
           in_range(codepoint, 0x1C50, 0x1C59) ||
           in_range(codepoint, 0xA620, 0xA629) ||
           in_range(codepoint, 0xA8D0, 0xA8D9) ||
           in_range(codepoint, 0xA900, 0xA909) ||
           in_range(codepoint, 0xA9D0, 0xA9D9) ||
           in_range(codepoint, 0xA9F0, 0xA9F9) ||
           in_range(codepoint, 0xAA50, 0xAA59) ||
           in_range(codepoint, 0xABF0, 0xABF9) ||
           in_range(codepoint, 0xFF10, 0xFF19) ||
           in_range(codepoint, 0x104A0, 0x104A9) ||
           in_range(codepoint, 0x10D30, 0x10D39) ||
           in_range(codepoint, 0x11066, 0x1106F) ||
           in_range(codepoint, 0x110F0, 0x110F9) ||
           in_range(codepoint, 0x11136, 0x1113F) ||
           in_range(codepoint, 0x111D0, 0x111D9) ||
           in_range(codepoint, 0x112F0, 0x112F9) ||
           in_range(codepoint, 0x11450, 0x11459) ||
           in_range(codepoint, 0x114D0, 0x114D9) ||
           in_range(codepoint, 0x11650, 0x11659) ||
           in_range(codepoint, 0x116C0, 0x116C9) ||
           in_range(codepoint, 0x11730, 0x11739) ||
           in_range(codepoint, 0x118E0, 0x118E9) ||
           in_range(codepoint, 0x11950, 0x11959) ||
           in_range(codepoint, 0x11C50, 0x11C59) ||
           in_range(codepoint, 0x11D50, 0x11D59) ||
           in_range(codepoint, 0x11DA0, 0x11DA9) ||
           in_range(codepoint, 0x16A60, 0x16A69) ||
           in_range(codepoint, 0x16B50, 0x16B59) ||
           in_range(codepoint, 0x1D7CE, 0x1D7FF) ||
           in_range(codepoint, 0x1E140, 0x1E149) ||
           in_range(codepoint, 0x1E2F0, 0x1E2F9) ||
           in_range(codepoint, 0x1E950, 0x1E959) ||
           in_range(codepoint, 0x1FBF0, 0x1FBF9);
}

inline bool text::unicode_category_nl(datum_type codepoint)
{
    return in_range(codepoint, 0x16EE, 0x16F0) ||
           in_range(codepoint, 0x2160, 0x2182) ||
           in_range(codepoint, 0x2185, 0x2188) ||
           in_range(codepoint, 0x3007, 0x3007) ||
           in_range(codepoint, 0x3021, 0x3029) ||
           in_range(codepoint, 0x3038, 0x303A) ||
           in_range(codepoint, 0xA6E6, 0xA6EF) ||
           in_range(codepoint, 0x10140, 0x10174) ||
           in_range(codepoint, 0x10341, 0x10341) ||
           in_range(codepoint, 0x1034A, 0x1034A) ||
           in_range(codepoint, 0x103D1, 0x103D5) ||
           in_range(codepoint, 0x12400, 0x1246E);
}

inline bool text::unicode_category_pc(datum_type codepoint)
{
    return in_range(codepoint, 0x005F, 0x005F) ||
           in_range(codepoint, 0x203F, 0x2040) ||
           in_range(codepoint, 0x2054, 0x2054) ||
           in_range(codepoint, 0xFE33, 0xFE34) ||
           in_range(codepoint, 0xFE4D, 0xFE4F) ||
           in_range(codepoint, 0xFF3F, 0xFF3F);
}

inline bool text::unicode_category_zs(datum_type codepoint)
{
    return in_range(codepoint, 0x0020, 0x0020) ||
           in_range(codepoint, 0x00A0, 0x00A0) ||
           in_range(codepoint, 0x1680, 0x1680) ||
           in_range(codepoint, 0x2000, 0x200A) ||
           in_range(codepoint, 0x202F, 0x202F) ||
           in_range(codepoint, 0x205F, 0x205F) ||
           in_range(codepoint, 0x3000, 0x3000);
}


#define PARSE_PROLOG                                                    \
    char_type buffer[NUMERIC_BUFFER_PARSE_SIZE];                        \
                                                                        \
    const datum_type* ptr(lexeme);                                      \
                                                                        \
    uint32_t k = 0;                                                     \
    uint32_t n = std::min<uint32_t>(length, NUMERIC_BUFFER_PARSE_SIZE); \
                                                                        \
    for(uint32_t i = 0; i < n; i++, ptr++)                              \
    {                                                                   \
        if(*ptr != L'_')                                                \
        {                                                               \
            *(buffer + k++) = static_cast<char_type>(*ptr);             \
        }                                                               \
    }                                                                   \
                                                                        \
    *(buffer + k) = 0;                                                  \
                                                                        \
    char_type* end;


template <typename T>
inline T text::parse(const datum_type* lexeme, uint32_t length, int base)
{
    return T {};
}

template <>
inline int8_t text::parse<int8_t>(const datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    int8_t result = static_cast<int8_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline uint8_t text::parse<uint8_t>(const datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    uint8_t result = static_cast<uint8_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline int16_t text::parse<int16_t>(const datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    int16_t result = static_cast<int16_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline uint16_t text::parse<uint16_t>(const datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    uint16_t result = static_cast<uint16_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline int32_t text::parse<int32_t>(const datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    int32_t result = static_cast<int32_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline uint32_t text::parse<uint32_t>(const datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    uint32_t result = static_cast<uint32_t>(std::wcstol(buffer, &end, base));

    return result;
}

template <>
inline int64_t text::parse<int64_t>(const datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    int64_t result = static_cast<int64_t>(std::wcstoll(buffer, &end, base));

    return result;
}

template <>
inline uint64_t text::parse<uint64_t>(const datum_type* lexeme, uint32_t length, int base)
{
    PARSE_PROLOG

    uint64_t result = static_cast<uint64_t>(std::wcstoll(buffer, &end, base));

    return result;
}

template <>
inline float text::parse<float>(const datum_type* lexeme, uint32_t length, int)
{
    PARSE_PROLOG

    float result = std::wcstof(buffer, &end);

    return result;
}

template <>
inline double text::parse<double>(const datum_type* lexeme, uint32_t length, int)
{
    PARSE_PROLOG

    double result = std::wcstod(buffer, &end);

    return result;
}

END_NAMESPACE

#endif // __TEXT_H__
