// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef VECTORSEARCH_VSEARCH_COMMON_H_
#define VECTORSEARCH_VSEARCH_COMMON_H_

#include "definition_list.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#ifndef _MSC_VER
#include <sys/stat.h>
#include <sys/types.h>
#define FolderSep '/'
#define mkdir(a) mkdir(a, ACCESSPERMS)
inline bool direxists(const char* path) {
  struct stat info;
  return stat(path, &info) == 0 && (info.st_mode & S_IFDIR);
}
inline bool fileexists(const char* path) {
  struct stat info;
  return stat(path, &info) == 0 && (info.st_mode & S_IFDIR) == 0;
}
template <class T>
inline T min(T a, T b) {
  return a < b ? a : b;
}
template <class T>
inline T max(T a, T b) {
  return a > b ? a : b;
}

#ifndef _rotl
#define _rotl(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#endif

#else
#define WIN32_LEAN_AND_MEAN
#include <Psapi.h>
#include <Windows.h>
#define FolderSep '\\'
#define mkdir(a) CreateDirectory(a, NULL)
inline bool direxists(const char* path) {
  auto dwAttr = GetFileAttributes((LPCSTR)path);
  return (dwAttr != INVALID_FILE_ATTRIBUTES) &&
         (dwAttr & FILE_ATTRIBUTE_DIRECTORY);
}
inline bool fileexists(const char* path) {
  auto dwAttr = GetFileAttributes((LPCSTR)path);
  return (dwAttr != INVALID_FILE_ATTRIBUTES) &&
         (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0;
}
#endif

namespace vsearch {

typedef std::uint32_t SizeType;

const float MinDist = (std::numeric_limits<float>::min)();
const float MaxDist = (std::numeric_limits<float>::max)();
const float Epsilon = 0.000000001f;

class MyException : public std::exception {
 private:
  std::string Exp;

 public:
  MyException(std::string e) { Exp = e; }
#ifdef _MSC_VER
  const char* what() const { return Exp.c_str(); }
#else
  const char* what() const noexcept { return Exp.c_str(); }
#endif
};

// Type of number index.
typedef std::int32_t IndexType;
static_assert(std::is_integral<IndexType>::value,
              "IndexType must be integral type.");

enum class ErrorCode : std::uint16_t {
#define DefineErrorCode(Name, Value) Name = Value,
#include "definition_list.h"
#undef DefineErrorCode

  Undefined
};
static_assert(static_cast<std::uint16_t>(ErrorCode::Undefined) != 0,
              "Empty ErrorCode!");

enum class DistCalcMethod : std::uint8_t {
#define DefineDistCalcMethod(Name) Name,
#include "definition_list.h"
#undef DefineDistCalcMethod

  Undefined
};

static_assert(static_cast<std::uint8_t>(DistCalcMethod::Undefined) != 0,
              "Empty DistCalcMethod!");

enum class VectorValueType : std::uint8_t {
#define DefineVectorValueType(Name, Type) Name,
#include "definition_list.h"
#undef DefineVectorValueType

  Undefined
};
static_assert(static_cast<std::uint8_t>(VectorValueType::Undefined) != 0,
              "Empty VectorValueType!");

enum class IndexAlgoType : std::uint8_t {
#define DefineIndexAlgo(Name) Name,
#include "definition_list.h"
#undef DefineIndexAlgo

  Undefined
};
static_assert(static_cast<std::uint8_t>(IndexAlgoType::Undefined) != 0,
              "Empty IndexAlgoType!");

template <typename T>
constexpr VectorValueType GetEnumValueType() {
  return VectorValueType::Undefined;
}

#define DefineVectorValueType(Name, Type)              \
  template <>                                          \
  constexpr VectorValueType GetEnumValueType<Type>() { \
    return VectorValueType::Name;                      \
  }

#include "definition_list.h"
#undef DefineVectorValueType

inline std::size_t GetValueTypeSize(VectorValueType p_valueType) {
  switch (p_valueType) {
#define DefineVectorValueType(Name, Type) \
  case VectorValueType::Name:             \
    return sizeof(Type);

#include "definition_list.h"
#undef DefineVectorValueType

    default:
      break;
  }

  return 0;
}

}  // vsearch

#endif //VECTORSEARCH_VSEARCH_COMMON_H_
