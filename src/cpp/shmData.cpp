#include "shmData.h"
#include "unistd.h"
#include "string.h"

namespace ShmIpc
{
  void ShmClass::init()
  {
    mtxRW.unlock();
  }

  bool ShmClass::getState(ShmData_t &data)
  {
    if (!mtxRW.try_lock_for(std::chrono::milliseconds(MUTEX_TIMEOUT_MS)))
    {
      std::cerr << "Error: try_lock timeout!\n";
      std::cerr << "On file " << __FILE__ << ", line " << __LINE__ << std::endl;
      return false;
    }

    data.state.tv = _shmData.state.tv;
    data.state.dim = _shmData.state.dim;
    memcpy(data.state.ids, _shmData.state.ids, sizeof(uint32_t) * _shmData.state.dim);
    memcpy(data.state.positions, _shmData.state.positions, sizeof(double) * _shmData.state.dim);
    memcpy(data.state.velocities, _shmData.state.velocities, sizeof(double) * _shmData.state.dim);
    memcpy(data.state.forces, _shmData.state.forces, sizeof(double) * _shmData.state.dim);

    mtxRW.unlock();
    return true;
  }

  bool ShmClass::getCommand(ShmData_t &data)
  {
    if (!mtxRW.try_lock_for(std::chrono::milliseconds(MUTEX_TIMEOUT_MS)))
    {
      std::cerr << "Error: try_lock timeout!\n";
      std::cerr << "On file " << __FILE__ << ", line " << __LINE__ << std::endl;
      return false;
    }

    data.command.tv = _shmData.command.tv;
    data.command.ctrlMode = _shmData.command.ctrlMode;
    data.command.dim = _shmData.command.dim;
    memcpy(data.command.ids, _shmData.command.ids, sizeof(uint32_t) * _shmData.command.dim);
    memcpy(data.command.forces, _shmData.command.forces, sizeof(double) * _shmData.command.dim);
    memcpy(data.command.forceFeedforward, _shmData.command.forceFeedforward, sizeof(double) * _shmData.command.dim);
    memcpy(data.command.velocities, _shmData.command.velocities, sizeof(double) * _shmData.command.dim);
    memcpy(data.command.velocityFeedforward, _shmData.command.velocityFeedforward, sizeof(double) * _shmData.command.dim);
    memcpy(data.command.positions, _shmData.command.positions, sizeof(double) * _shmData.command.dim);
    memcpy(data.command.positionsFeedforward, _shmData.command.positionsFeedforward, sizeof(double) * _shmData.command.dim);

    mtxRW.unlock();
    return true;
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
    _shmData.state.dim = data.state.dim;
    memcpy(_shmData.state.ids, data.state.ids, sizeof(uint32_t) * data.state.dim);
    memcpy(_shmData.state.positions, data.state.positions, sizeof(double) * data.state.dim);
    memcpy(_shmData.state.velocities, data.state.velocities, sizeof(double) * data.state.dim);
    memcpy(_shmData.state.forces, data.state.forces, sizeof(double) * data.state.dim);
    getCommand(data);
    cmdUpdate = false;

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
    _shmData.command.dim = data.command.dim;
    memcpy(_shmData.command.ids, data.command.ids, sizeof(uint32_t) * data.command.dim);
    memcpy(_shmData.command.forces, data.command.forces, sizeof(double) * data.command.dim);
    memcpy(_shmData.command.forceFeedforward, data.command.forceFeedforward, sizeof(double) * data.command.dim);
    memcpy(_shmData.command.velocities, data.command.velocities, sizeof(double) * data.command.dim);
    memcpy(_shmData.command.velocityFeedforward, data.command.velocityFeedforward, sizeof(double) * data.command.dim);
    memcpy(_shmData.command.positions, data.command.positions, sizeof(double) * data.command.dim);
    memcpy(_shmData.command.positionsFeedforward, data.command.positionsFeedforward, sizeof(double) * data.command.dim);
    getState(data);
    cmdUpdate = true;

    mtxRW.unlock();
    return true;
  }

  bool ShmClass::isCommandUpdate()
  {
    if (!mtxRW.try_lock_for(std::chrono::microseconds(100)))
    {
      std::cerr << "Error: try_lock timeout!\n";
      std::cerr << "On file " << __FILE__ << ", line " << __LINE__ << std::endl;
      return false;
    }
    if (cmdUpdate != true)
    {
      mtxRW.unlock();
      return false;
    }
    mtxRW.unlock();
    return true;
  }
} // namespace ShmIpc
