//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PCH_H__
#define __PCH_H__

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include <cstddef>
#include <cctype>
#include <cassert>

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <exception>
#include <stdexcept>

#include <memory>
#include <new>

#include <typeindex>
#include <type_traits>

#include <optional>

#include <algorithm>
#include <functional>
#include <utility>
#include <limits>
#include <locale>

#include <string>
#include <charconv>
#include <array>
#include <valarray>
#include <bitset>
#include <tuple>
#include <list>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <queue>

#include <variant>

#include <cfenv>
#include <codecvt>

#include <chrono>

#include <filesystem>

#ifdef _WIN32
#   define NOMINMAX
#   include <windows.h>
#   include <strsafe.h>
#endif

#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustdio.h>

#define COMPANY uilab

#define BEGIN_NAMESPACE(__x__) namespace COMPANY { ## namespace __x__ {
#define END_NAMESPACE   }; };

#define USINGNAMESPACE(__x__) using namespace COMPANY::##__x__;

#ifdef interface
#   undef interface
#endif
#define interface struct

#define UTF8_ENCODING       L"UTF-8"
#define UTF16_LE_ENCODING   L"UTF-16-LE"
#define UTF16_BE_ENCODING   L"UTF-16-BE"
#define UTF32_LE_ENCODING   L"UTF-32-LE"
#define UTF32_BE_ENCODING   L"UTF-32-BE"
#define UTF7_ENCODING       L"UTF-7"
#define UTF1_ENCODING       L"UTF-1"
#define UTF_EBCDIC_ENCODING L"UTF-EBCDIC"
#define SDSU_ENCODING       L"SDSU"
#define BOCU1_ENCODING      L"BOCU-1"
#define GB18030_ENCODING    L"GB-18030"
#define DEFAULT_ENCODING    L"DEFAULT"

#define INVALID_VALUE (static_cast<uint32_t>(-1))

BEGIN_NAMESPACE(core)

using byte = uint8_t;

using size_type = std::size_t;
using char_type = wchar_t;
using string_type = std::basic_string<char_type>;
using cp_type = uint32_t;   // code point
using offset_type = long;   // fseek, etc.
using loc_type = uint64_t;  // position, locus, location ...
using hash_type = uint64_t;

END_NAMESPACE

#endif // __PCH_H__
