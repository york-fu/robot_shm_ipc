#include "shmData.h"
#include "unistd.h"

namespace ShmIpc
{
  void ShmClass::init()
  {
    mtxRW.unlock();
  }

  bool ShmClass::setState(ShmData_t &data)
  {
    if (!mtxRW.try_lock_for(std::chrono::milliseconds(MUTEX_TIMEOUT_MS)))
    {
      std::cerr << "Error: try_lock timeout!\n";
      std::cerr << "On file " << __FILE__ << ", line " << __LINE__ << std::endl;
      return false;
    }

    _shmData.state.tv = data.state.tv;
    data.command.tv = _shmData.command.tv;
    data.command.ctrlMode = _shmData.command.ctrlMode;
    for (uint32_t i = 0; i < DATA_SIZE; i++) // deepcopy
    {
      _shmData.state.positions[i] = data.state.positions[i];
      _shmData.state.velocities[i] = data.state.velocities[i];
      _shmData.state.forces[i] = data.state.forces[i];
      data.command.forces[i] = _shmData.command.forces[i];
      data.command.forceFeedforward[i] = _shmData.command.forceFeedforward[i];
      data.command.velocities[i] = _shmData.command.velocities[i];
      data.command.velocityFeedforward[i] = _shmData.command.velocityFeedforward[i];
      data.command.positions[i] = _shmData.command.positions[i];
      data.command.positionsFeedforward[i] = _shmData.command.positionsFeedforward[i];
    }

    mtxRW.unlock();
    return true;
  }

  bool ShmClass::setCommand(ShmData_t &data)
  {
    if (!mtxRW.try_lock_for(std::chrono::milliseconds(MUTEX_TIMEOUT_MS)))
    {
      std::cerr << "Error: try_lock timeout!\n";
      std::cerr << "On file " << __FILE__ << ", line " << __LINE__ << std::endl;
      return false;
    }

    _shmData.command.tv = data.command.tv;
    _shmData.command.ctrlMode = data.command.ctrlMode;
    data.state.tv = _shmData.state.tv;
    for (uint32_t i = 0; i < DATA_SIZE; i++) // deepcopy
    {
      _shmData.command.forces[i] = data.command.forces[i];
      _shmData.command.forceFeedforward[i] = data.command.forceFeedforward[i];
      _shmData.command.velocities[i] = data.command.velocities[i];
      _shmData.command.velocityFeedforward[i] = data.command.velocityFeedforward[i];
      _shmData.command.positions[i] = data.command.positions[i];
      _shmData.command.positionsFeedforward[i] = data.command.positionsFeedforward[i];
      data.state.positions[i] = _shmData.state.positions[i];
      data.state.velocities[i] = _shmData.state.velocities[i];
      data.state.forces[i] = _shmData.state.forces[i];
    }

    mtxRW.unlock();
    return true;
  }
} // namespace ShmIpc
