# Robot shm ipc

使用共享内存实现适用于机器人进程间的实时通信。

`/example/driver.cpp`模拟硬件驱动进程，`/example/controller.cpp`模拟发送控制指令的应用进程，`/example/monitor.cpp`模拟其他不发送控制指令的应用进程。

## Run

```
mkdir build
cd build
cmake ..
make -j`nporc`

./example/state
./example/command
./example/monitor
```