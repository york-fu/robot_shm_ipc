#ifndef _shmIpc_h_
#define _shmIpc_h_

#include <stdio.h>
#include <stdint.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <iostream>
#include <vector>
#include <mutex>

#include "shmData.h"

namespace ShmIpc
{
  bool shmIpcInit(ShmClass **shmObj);
  bool shmIpcDt();
  bool shmIpcDelete();
} // namespace ShmIpc

#endif
