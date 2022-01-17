#include <iostream>
#include <signal.h>
#include "shmIpc.h"

uint8_t _process_running = 0;
ShmIpc::ShmClass *shmObj = NULL;
double dt = 1e-3; // s

void sigintHandler(int sig)
{
  _process_running = 0;
  ShmIpc::shmIpcDt();
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

  struct timespec next_time;
  struct timespec tv;
  ShmIpc::ShmData_t shmData;
  uint64_t count = 0;

  clock_gettime(CLOCK_MONOTONIC, &next_time);
  while (_process_running)
  {
    clock_gettime(CLOCK_MONOTONIC, &tv);
    shmData.command.tv.sec = tv.tv_sec;
    shmData.command.tv.nsec = tv.tv_nsec;
    shmData.command.tv.count = count++;
    shmData.command.dim = 2;
    for (uint32_t i = 0; i < shmData.command.dim; i++)
    {
      shmData.command.ids[i] = i + 1;
      shmData.command.forces[i] = 0;
    }
    shmObj->setCommand(shmData);

    printf("Received state:\n");
    printf("  transmit time(ms): %f\n", ((tv.tv_sec + tv.tv_nsec * 1e-9) - (shmData.state.tv.sec + shmData.state.tv.nsec * 1e-9)) * 1e3);
    printf("  count: %ld, timestamp: %ld.%ld\n", shmData.state.tv.count, shmData.state.tv.sec, shmData.state.tv.nsec);
    printf("  ids: ");
    for (uint32_t i = 0; i < shmData.state.dim; i++)
    {
      printf("%d  ", shmData.state.ids[i]);
    }
    printf("\n");
    printf("  position: ");
    for (uint32_t i = 0; i < shmData.state.dim; i++)
    {
      printf("%f  ", shmData.state.positions[i]);
    }
    printf("\n");

    next_time.tv_sec += (next_time.tv_nsec + dt * 1e9) / 1e9;
    next_time.tv_nsec = (int)(next_time.tv_nsec + dt * 1e9) % (int)1e9;
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_time, NULL);
  }

  return 0;
}
