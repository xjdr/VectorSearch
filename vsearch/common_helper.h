// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef VECTORSEARCH_VSEARCH_COMMON_HELPER_H_
#define VECTORSEARCH_VSEARCH_COMMON_HELPER_H_

#include "common.h"

#include <cctype>
#include <cerrno>
#include <functional>
#include <limits>
#include <string>
#include <vector>

namespace vsearch {
namespace Helper {
namespace StrUtils {

void ToLowerInPlace(std::string& p_str);

std::vector<std::string> SplitString(const std::string& p_str,
                                     const std::string& p_separator);

std::pair<const char*, const char*> FindTrimmedSegment(
    const char* p_begin, const char* p_end,
    const std::function<bool(char)>& p_isSkippedChar);

bool StartsWith(const char* p_str, const char* p_prefix);

bool StrEqualIgnoreCase(const char* p_left, const char* p_right);

}  // namespace StrUtils
}  // namespace Helper
}  // namespace vsearch

#endif //VECTORSEARCH_VSEARCH_COMMON_HELPER_H_
