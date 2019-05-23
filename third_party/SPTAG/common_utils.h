// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef VECTORSEARCH_VSEARCH_COMMON_UTILS_H_
#define VECTORSEARCH_VSEARCH_COMMON_UTILS_H_
#include "common.h"

#include <unordered_map>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>

#include <omp.h>
#include <string.h>
#include <time.h>

#define PREFETCH

#ifndef _MSC_VER
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <cstring>

#define InterlockedCompareExchange(a, b, c) __sync_val_compare_and_swap(a, c, b)
#define Sleep(a) usleep(a * 1000)
#define strtok_s(a, b, c) strtok_r(a, b, c)
#endif

namespace vsearch {
class Utils {
 public:
  static int rand_int(int high = RAND_MAX,
                      int low = 0)  // Generates a random int value.
  {
    return low + (int)(float(high - low) * (std::rand() / (RAND_MAX + 1.0)));
  }

  static inline float atomic_float_add(volatile float* ptr,
                                       const float operand) {
    union {
      volatile long iOld;
      float fOld;
    };
    union {
      long iNew;
      float fNew;
    };

    while (true) {
      iOld = *(volatile long*)ptr;
      fNew = fOld + operand;
      if (InterlockedCompareExchange((long*)ptr, iNew, iOld) == iOld) {
        return fNew;
      }
    }
  }

  static double GetVector(char* cstr, const char* sep, std::vector<float>& arr,
                          int& NumDim) {
    char* current;
    char* context = NULL;

    int i = 0;
    double sum = 0;
    arr.clear();
    current = strtok_s(cstr, sep, &context);
    while (current != NULL && (i < NumDim || NumDim < 0)) {
      try {
        float val = (float)atof(current);
        arr.push_back(val);
      } catch (std::exception e) {
        std::cout << "Exception:" << e.what() << std::endl;
        return -2;
      }

      sum += arr[i] * arr[i];
      current = strtok_s(NULL, sep, &context);
      i++;
    }

    if (NumDim < 0) NumDim = i;
    if (i < NumDim) return -2;
    return std::sqrt(sum);
  }

  template <typename T>
  static void Normalize(T* arr, int col, int base) {
    double vecLen = 0;
    for (int j = 0; j < col; j++) {
      double val = arr[j];
      vecLen += val * val;
    }
    vecLen = std::sqrt(vecLen);
    if (vecLen < 1e-6) {
      T val = (T)(1.0 / std::sqrt((double)col) * base);
      for (int j = 0; j < col; j++) arr[j] = val;
    } else {
      for (int j = 0; j < col; j++) arr[j] = (T)(arr[j] / vecLen * base);
    }
  }

  static size_t ProcessLine(std::string& currentLine, std::vector<float>& arr,
                            int& D, int base, DistCalcMethod distCalcMethod) {
    size_t index;
    double vecLen;
    if (currentLine.length() == 0 ||
        (index = currentLine.find_last_of("\t")) == std::string::npos ||
        (vecLen = GetVector(const_cast<char*>(currentLine.c_str() + index + 1),
                            "|", arr, D)) < -1) {
      std::cout << "Parse vector error: " + currentLine << std::endl;
      // throw MyException("Error in parsing data " + currentLine);
      return -1;
    }
    if (distCalcMethod == DistCalcMethod::Cosine) {
      Normalize(arr.data(), D, base);
    }
    return index;
  }

  template <typename T>
  static void PrepareQuerys(std::ifstream& inStream,
                            std::vector<std::string>& qString,
                            std::vector<std::vector<T>>& Query, int& NumQuery,
                            int& NumDim, DistCalcMethod distCalcMethod,
                            int base) {
    std::string currentLine;
    std::vector<float> arr;
    int i = 0;
    size_t index;
    while ((NumQuery < 0 || i < NumQuery) && !inStream.eof()) {
      std::getline(inStream, currentLine);
      if (currentLine.length() <= 1 ||
          (index = ProcessLine(currentLine, arr, NumDim, base,
                               distCalcMethod)) < 0) {
        continue;
      }
      qString.push_back(currentLine.substr(0, index));
      if (Query.size() < i + 1) Query.push_back(std::vector<T>(NumDim, 0));

      for (int j = 0; j < NumDim; j++) Query[i][j] = (T)arr[j];
      i++;
    }
    NumQuery = i;
    std::cout << "Load data: (" << NumQuery << ", " << NumDim << ")"
              << std::endl;
  }

  template <typename T>
  static inline int GetBase() {
    if (GetEnumValueType<T>() != VectorValueType::Float) {
      return (int)(std::numeric_limits<T>::max)();
    }
    return 1;
  }

  static inline void AddNeighbor(int idx, float dist, int* neighbors,
                                 float* dists, int size) {
    size--;
    if (dist < dists[size] || (dist == dists[size] && idx < neighbors[size])) {
      int nb;
      for (nb = 0; nb <= size && neighbors[nb] != idx; nb++)
        ;

      if (nb > size) {
        nb = size;
        while (nb > 0 && (dist < dists[nb - 1] ||
            (dist == dists[nb - 1] && idx < neighbors[nb - 1]))) {
          dists[nb] = dists[nb - 1];
          neighbors[nb] = neighbors[nb - 1];
          nb--;
        }
        dists[nb] = dist;
        neighbors[nb] = idx;
      }
    }
  }
};
} // vsearch

#endif //VECTORSEARCH_VSEARCH_COMMON_UTILS_H_
