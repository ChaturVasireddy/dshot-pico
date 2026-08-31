# dshot-pico
A DShot (Digital Shot) library for the Raspberry Pi Pico using PIO (Programmable I/O).

Currently, the library supports DShot300. Support for additional DShot protocols may be added in the future.

The library is designed for the Pico C/C++ SDK and allows you to control DShot-compatible ESCs through a direct connection from a GPIO pin to the ESC signal input. DShot signal generation is handled by the Pico's PIO hardware, so the CPU does not need to generate the precise signal timing in software.

I developed and tested this library with a DShot-compatible ESC. Other ESCs that support DShot should also work, but I have not tested the library with every ESC or DShot configuration.

## Getting the library and compiling
The library can be added to a Pico SDK project using CMake FetchContent.

Add the following to your `CMakeLists.txt`:
```cmake
include(FetchContent)

FetchContent_Declare(
    dshot
    GIT_REPOSITORY https://github.com/ChaturVasireddy/DShotPIO.git
    GIT_TAG main
)

FetchContent_MakeAvailable(dshot)
```
Then link the library to your executable:
```cmake
target_link_libraries(your_project
    pico_stdlib
    dshot    #add this
)
```
## Usage
### 1.Create dshot_actuator object type:
The pin member specifies the GPIO connected to the ESC signal input:
```c
dshot_actuator example_motor = {
    .pin = 2
};
```

### 2.Initialize the PIO:
```c
dshot_init(&example_motor);
```
> Note that `dshot_init` is blocking and takes about 7ms.
### 3.Arm ESC:
```c
dshot_arm(&example_motor);
```
### 4. Send Throttle Commands
After arming, peridically call the throttle function: 
```c
dshot_throttle(&example_motor, throttle value);
```
if you stop calling `dshot_throttle` for too long, you are required to arming sequence again. Im not really sure about the maximum time interval, but i believe 1ms is a good time period between each throttle function call. As for the minimum time interval, i observed that it works well when its above 200us (0.2ms).

> `dshot_throttle` is basically instantaneous since it just loads data into the PIO state machines after which it can run parllely with the main core.

## Throttle Values
For normal throttle operation, use values between 48 and 2047 where 48 is slow spin and 2047 is the max spin. 

Values between 0 47 since are reserved for speacial commands. For advanced usecases, you can send the required value. You can find the special commands [here](https://brushlesswhoop.com/dshot-and-bidirectional-dshot/).

## 3D Mode
3d mode lets you rotate the actuator in both direction, but this splits the resolution in half. you have to use 48 to 1047 for one direction and 1048 to 2047 for the other direction where 48/1048 is slow spin and 1047/2047 is the max spin.
```c
dshot_mode3d(&example_motor);
```
> This is basically is equivalent to calling `dshot_throttle` with the required special command. However i still made it into a function since its used commonly.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
