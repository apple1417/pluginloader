#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <tlhelp32.h>

#ifdef __cplusplus

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::int8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

#endif

#if defined(__clang__) || defined(__MINGW32__)
#define DLL_EXPORT extern "C" [[gnu::dllexport]]
#elif defined(_MSC_VER)
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#error Unknown dllexport attribute
#endif

#endif /* PCH_H */
