#include "shmIpc.h"

namespace ShmIpc
{
  static int shmId;
  static void *shmPtr = NULL;

  bool shmIpcInit(ShmClass **shmObj)
  {
    char filePath[1024] = {"../"};

    key_t key;
    key = ftok(filePath, 'b'); //创建key值,
    if (key < 0)
    {
      printf("Failed ftok!\n");
      return false;
    }
    shmId = shmget(key, sizeof(ShmClass), IPC_CREAT | IPC_EXCL | 0777); //创建/打开共享内存，返回id根据id映射
    if (shmId < 0)
    {
      if (errno == EEXIST) //文件存在时，直接打开文件获取shmid
      {
        printf("Shm eexist.\n");
        shmId = shmget(key, sizeof(ShmClass), 0777); //共享内存存在时，直接打开
      }
      else
      {
        printf("Failed shmget!\n");
        return false;
      }
    }
    shmPtr = shmat(shmId, NULL, 0); //映射，返回地址，根据地址操作
    if (shmPtr == (void *)(-1))
    {
      printf("Failed shmat!\n");
      return false;
    }
    *shmObj = (ShmClass *)shmPtr;
    (*shmObj)->init();
    return true;
  }

  bool shmIpcDt()
  {
    int ret;
    ret = shmdt(shmPtr); //解除映射
    if (ret != 0)
    {
      printf("Failed shmdt!\n");
      return false;
    }
    return true;
  }

  bool shmIpcDelete()
  {
    int ret;
    ret = shmdt(shmPtr); //解除映射
    if (ret != 0)
    {
      printf("Failed shmdtl!\n");
    }
    ret = shmctl(shmId, IPC_RMID, NULL); //删除
    if (ret != 0)
    {
      printf("Failed shmctl!\n");
      return false;
    }
    return true;
  }
} // namespace ShmIpc
