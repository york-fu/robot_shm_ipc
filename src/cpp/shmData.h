#ifndef _shmData_h_
#define _shmData_h_

#include <iostream>
#include <vector>
#include <mutex>

namespace ShmIpc
{
#define MUTEX_TIMEOUT_MS 100
#define DIMENSION_MAX 12

  typedef struct
  {
    uint64_t sec;
    uint64_t nsec;
    uint64_t count;
  } Timestamp_t;

  typedef struct
  {
    Timestamp_t tv;
    uint32_t dim; // dimension
    uint32_t ids[DIMENSION_MAX];
    double positions[DIMENSION_MAX];
    double velocities[DIMENSION_MAX];
    double forces[DIMENSION_MAX];
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
    uint32_t dim; // dimension
    uint32_t ids[DIMENSION_MAX];
    double forces[DIMENSION_MAX];
    double forceFeedforward[DIMENSION_MAX];
    double velocities[DIMENSION_MAX];
    double velocityFeedforward[DIMENSION_MAX];
    double positions[DIMENSION_MAX];
    double positionsFeedforward[DIMENSION_MAX];
  } CtrlMsg_t;

  typedef struct
  {
    StateMsg_t state;
    CtrlMsg_t command;
  } ShmData_t;

  class ShmClass // Shm operation class
  {
  public:
    void init();
    bool getState(ShmData_t &data); // Only get state
    bool getCommand(ShmData_t &data); // Only get command
    bool setState(ShmData_t &data); // Set state and get command
    bool setCommand(ShmData_t &data); // Set command and get state
    bool isCommandUpdate();

  private:
    std::timed_mutex mtxRW;
    ShmData_t _shmData;
    bool cmdUpdate;
  };
} // namespace ShmIpc

#endif
