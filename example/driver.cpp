#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "shmIpc.h"

uint8_t _process_running = 0;
ShmIpc::ShmClass *shmObj = NULL;

void sigintHandler(int sig)
{
  _process_running = 0;
  ShmIpc::shmIpcDelete();
  printf("\nSIGINT exit.\n");
}

int main(int argv, char **argc)
{
  _process_running = 1;
  signal(SIGINT, sigintHandler);
  bool result = false;
  result = ShmIpc::shmIpcInit(&shmObj);
  if (result == false || shmObj == NULL)
  {
    std::cout << "Failed shmIpcInit!\n";
    return -1;
  }

  struct timespec tv;
  ShmIpc::ShmData_t shmData;
  uint64_t count = 0;

  while (_process_running)
  {
    if (shmObj->isCommandUpdate())
    {
      clock_gettime(CLOCK_MONOTONIC, &tv);
      shmData.state.tv.sec = tv.tv_sec;
      shmData.state.tv.nsec = tv.tv_nsec;
      shmData.state.tv.count = count++;
      shmData.state.dim = 2;
      for (uint32_t i = 0; i < shmData.state.dim; i++)
      {
        shmData.state.ids[i] = i + 1;
        shmData.state.positions[i] = 0;
      }
      shmObj->setState(shmData);

      printf("Received command:\n");
      printf("  transmit time(ms): %f\n", ((tv.tv_sec + tv.tv_nsec * 1e-9) - (shmData.command.tv.sec + shmData.command.tv.nsec * 1e-9)) * 1e3);
      printf("  count: %ld, timestamp: %ld.%ld\n", shmData.command.tv.count, shmData.command.tv.sec, shmData.command.tv.nsec);
      printf("  dim: %d, mode: %d\n", shmData.command.dim, shmData.command.ctrlMode);
      printf("  ids: ");
      for (uint32_t i = 0; i < shmData.command.dim; i++)
      {
        printf("%d  ", shmData.command.ids[i]);
      }
      printf("\n");
      printf("  forces: ");
      for (uint32_t i = 0; i < shmData.command.dim; i++)
      {
        printf("%f  ", shmData.command.forces[i]);
      }
      printf("\n");
    }
    usleep(1);
  }

  return 0;
}
