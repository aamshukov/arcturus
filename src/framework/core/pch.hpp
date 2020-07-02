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
#define PROJECT arcturus

#define BEGIN_NAMESPACE(__x__) namespace COMPANY { namespace PROJECT { ## namespace __x__ {
#define END_NAMESPACE   }; }; };

#define USINGNAMESPACE(__x__) using namespace COMPANY::PROJECT::##__x__;

#ifdef interface
#   undef interface
#endif
#define interface struct

#define INVALID_VALUE (static_cast<uint32_t>(-1))

BEGIN_NAMESPACE(core)

using byte = uint8_t;
using size_type = std::size_t;
using cp_type = uint32_t;   // code point
using offset_type = long;   // fseek, etc.
using loc_type = int64_t;   // position, locus, location ...
using hash_type = uint64_t;
using char_type = wchar_t;
using string_type = std::basic_string<char_type>;

const int bits_in_byte = 8;

END_NAMESPACE

#endif // __PCH_H__
