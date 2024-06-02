/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/types/string/from_type.h"
#include "pathlib/types/string/long_string.h"
#include "pathlib/types/string/short_string.h"
#include "pathlib/types/string/short_string_unsafe.h"

namespace Pathlib {

//---
template <u64 RESERVE_CAPACITY>
struct LongStringUnsafe
{
  //---
  utf8* str;
  u64 capacity;
  u64 size;
  
  //---
  LongStringUnsafe()
  {
    capacity = RESERVE_CAPACITY;
    str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1);
    clear();
  }

  //---
  ~LongStringUnsafe()
  {
    if (str) {
      free_unsafe((void**)&str);
    }
  }

  //---
  LongStringUnsafe(const LongStringUnsafe& string)
  {
    str = (utf8*)malloc_unsafe(string.capacity + 1);
    capacity = string.capacity;
    memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
  }
  
  //---
  template <typename... Args>
  LongStringUnsafe(Args&&... args)
  {
    str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1);
    capacity = RESERVE_CAPACITY;
    size = 0;
    (LongStringUnsafe::_append(*this, args), ...);
  }

  //---
  inline LongStringUnsafe& operator =(const LongStringUnsafe& string)
  {
    size = string.size;
    if (size > string.capacity) {
      capacity = size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
    return *this;
  }

  //---
  template <typename T>
  inline LongStringUnsafe& operator =(const T arg)
  {
    u64 arg_size = String::size_of(arg);
    if (arg_size > capacity) {
      capacity = arg_size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    String::_Internal::from_type_grow(arg, &str, &size, &capacity);
    return *this;
  }

  //---
  inline bool operator ==(const utf8* string) const
  {
    return String::compare<true, false>(str, string, size);
  }

  //---
  inline bool operator ==(const LongStringUnsafe& string) const
  {
    return String::compare<true, true>(str, string.str, size, string.size);
  }

  //---
  template <typename T>
  inline const LongStringUnsafe operator +(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "LongStringUnsafe literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    LongStringUnsafe::_append(*this, arg);
    return *this;
  }

  //---
  inline LongStringUnsafe& operator +=(const LongString<RESERVE_CAPACITY>& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  inline LongStringUnsafe& operator +=(const LongStringUnsafe& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  template <u64 CAPACITY>
  inline LongStringUnsafe& operator +=(const ShortString<CAPACITY>& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  template <u64 CAPACITY>
  inline LongStringUnsafe& operator +=(const ShortStringUnsafe<CAPACITY>& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  template <typename T>
  inline LongStringUnsafe& operator +=(const T arg)
  {
    String::_Internal::from_type_grow(arg, &str, &size, &capacity);
    return *this;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(LongStringUnsafe& string_out, 
                             const ShortString<ARG_CAPACITY>& arg)
  {

    u64 new_size = string_out.size + arg.get_size();
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)realloc_unsafe(string_out.str, string_out.capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.get_str(), arg.get_size() + 1);
    string_out.size = new_size;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(LongStringUnsafe& string_out, 
                             const ShortStringUnsafe<ARG_CAPACITY>& arg)
  {
    u64 new_size = string_out.size + arg.size;
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)realloc_unsafe(string_out.str, string_out.capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, arg.size + 1);
    string_out.size = new_size;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(LongStringUnsafe& string_out,
                             const LongString<ARG_CAPACITY>& arg)
  {
    u64 new_size = string_out.size + arg.get_size();
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)realloc_unsafe(string_out.str, string_out.capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.get_str(), arg.get_size() + 1);
    string_out.size = new_size;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(LongStringUnsafe& string_out,
                             const LongStringUnsafe<ARG_CAPACITY>& arg)
  {
    u64 new_size = string_out.size + arg.size;
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)realloc_unsafe(string_out.str, string_out.capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, arg.size + 1);
    string_out.size = new_size;
  }

  //---
  template <typename T>
  static inline void _append(LongStringUnsafe& string_out, 
                             const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        return;
      }
    }
    String::_Internal::from_type_grow(arg, &string_out.str, &string_out.size, &string_out.capacity);
  }

  //---
  template <typename... Args>
  inline void append(Args&&... args)
  {
    (_append(*this, args), ...);
  }

  //---
  template <typename... Args>
  static inline void append(LongStringUnsafe& string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  inline LongStringUnsafe& format(Args&&... args)
  {
    size = 0;
    (LongStringUnsafe::_append(*this, args), ...);
    return *this;
  }

  //---
  template <typename... Args>
  static inline LongStringUnsafe format(LongStringUnsafe& string_out,
                                        Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  inline void clear()
  {
    str[0] = u8'\0';
    size = 0;
  }

  //---
  static inline u32 hash(const utf8* value)
  {
    return Math::hash(value, String::size_of(value));
  }

  //---
  inline u32 hash() const
  {
    return Math::hash(str, size);
  }

  //---
  template <typename T>
  LongStringUnsafe& from_value_hex(T value)
  {
    utf8 chars[] = u8"0123456789ABCDEF";
    constexpr u32 digit_count = sizeof(T) * 2;
    constexpr u32 new_size = digit_count + 2;
    if (new_size > capacity) {
      capacity = new_size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    str[0] = u8'0';
    str[1] = u8'x';
    #pragma unroll
    for (i32 d = digit_count - 1; d >= 0; --d) {
      str[d + 2] = chars[value & 0xF];
      value >>= 4;
    }
    str[new_size] = u8'\0';
    size = new_size;
    return *this;
  }
};
}

//---
template <typename T> struct _is_long_string : false_type {};
template <u64 RESERVE_CAPACITY> struct _is_long_string<Pathlib::LongString<RESERVE_CAPACITY>> : true_type {};
template <u64 RESERVE_CAPACITY> struct _is_long_string<Pathlib::LongStringUnsafe<RESERVE_CAPACITY>> : true_type {};