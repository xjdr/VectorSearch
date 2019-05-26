// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef VECTORSEARCH_VSEARCH_WORKSPACE_POOL_H_
#define VECTORSEARCH_VSEARCH_WORKSPACE_POOL_H_

#include "workspace.h"

#include <list>
#include <mutex>

namespace vsearch {
namespace COMMON {

class WorkSpacePool {
 public:
  WorkSpacePool(int p_maxCheck, int p_vectorCount);

  virtual ~WorkSpacePool();

  std::shared_ptr<WorkSpace> Rent();

  void Return(const std::shared_ptr<WorkSpace>& p_workSpace);

  void Init(int size);

 private:
  std::list<std::shared_ptr<WorkSpace>> m_workSpacePool;

  std::mutex m_workSpacePoolMutex;

  int m_maxCheck;

  int m_vectorCount;
};
}
}

#endif //VECTORSEARCH_VSEARCH_WORKSPACE_POOL_H_
