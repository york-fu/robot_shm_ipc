#ifndef _shmData_h_
#define _shmData_h_

#include <iostream>
#include <vector>
#include <mutex>

namespace ShmIpc
{
#define MUTEX_TIMEOUT_MS 500
#define DATA_SIZE 10

  typedef struct
  {
    uint64_t sec;
    uint64_t nsec;
  } Timestamp_t;

  typedef struct
  {
    Timestamp_t tv;
    double positions[DATA_SIZE];
    double velocities[DATA_SIZE];
    double forces[DATA_SIZE];
  } StateMsg_t;

  typedef enum
  {
    CST = 0,
    CSV,
    CSP,
  } CtrlMode_t;

  typedef struct
  {
    Timestamp_t tv;
    CtrlMode_t ctrlMode;
    double forces[DATA_SIZE];
    double forceFeedforward[DATA_SIZE];
    double velocities[DATA_SIZE];
    double velocityFeedforward[DATA_SIZE];
    double positions[DATA_SIZE];
    double positionsFeedforward[DATA_SIZE];
  } CtrlMsg_t;

  typedef struct
  {
    StateMsg_t state;
    CtrlMsg_t command;
  } ShmData_t;

  class ShmClass // shm operation class
  {
  public:
    void init();
    bool setState(ShmData_t &data);
    bool setCommand(ShmData_t &data);

  private:
    std::timed_mutex mtxRW;
    ShmData_t _shmData;
  };
} // namespace ShmIpc

#endif
