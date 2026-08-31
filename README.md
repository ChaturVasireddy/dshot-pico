# dshot-pio
DShot (Digital Shot) library for the Raspberry Pi Pico using PIO (Programmable I/O).

Supports only DShot300 for now.

Library for the Pico C/C++ SDK, allowing you to control DShot-compatible ESCs with a direct connection from a GPIO pin to the ESC signal input. The DShot signal generation is handled by the Pico's PIO, so the CPU does not need to generate the precise timing in software.

I developed and tested this with a DShot-compatible ESC. Other ESCs supporting DShot should also work, but I have not tested every ESC or DShot configuration.

Supports 3D mode.

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
Create an object of type dshot_actuator. The pin member specifies the GPIO connected to the ESC signal input:
```c
dshot_actuator example_motor = {
    .pin = 2
};
```
Initialize the PIO:
```c
dshot_init(&example_motor);
```
Now to run the actuator, you need to first call the arming function:
```c
dshot_arm(&example_motor);
```
Then you need to constantly call the throttle function, if you stop calling the throttle function, you are required to call the arming function again. Im not really sure about the maximum time period, but i believe 1ms is a good time period between each throttle function call. it can be lower than 1ms aswell and i observed that it works better when its above 200us/0.2ms. 
```c
dshot_throttle(&example_motor, throttle value);
```

Note that `dshot_init(&example_motor);` is blocking and takes about 7ms. `dshot_throttle(&example_motor, throttle value);` is basically instantaneous since it just calls the PIO state machines which can run parallely with the main cores.

For the throttle value you have to send a value between 48 and 2047 where 48 is slow spin and 2047 is the max spin. Do note send values between 0 48 since they are reserved for speacial commands. I let the user to 
