# stm32f401-mcal

The stm32f401-mcal library is an abstraction layer for STM32F401RE mikrocontroller. 
It is developed in and for the MCR lessons @RDF. For that reason it does not aim to be the best library out there. 
Rather it should provide an learning point and abstraction layer for the student in class.

⚠️ This lib is still a work in progress. Not recommend for production setups.

## Usage

This library is designed to be used with the [PlatformIO](https://platformio.org/) ecosystem.

Inside the `platformio.ini` add 

```ini
lib_deps = 
    MCAL=https://codeberg.org/jomaway/stm32f401-mcal.git
```
Inside your code (ex. `main.c`) import the module you want to use. For the `gpio` module:

```c
#include <gpio.h>
```

## Available Modules

The lib provides multiple modules which target different sections of the mcu.


| Module  | Description |
| ------------- | ------------- |
| gpio  | Primary gpio tasks: selecting pin mode, set pin state, reading pin state, ...  |
| delay | Config the systick timer to be used as a blocking delay or setting up timers |
| exti  | Config an external interrupt EXTI0 to EXTI15 |
| uart  | Setting up a uart connection. |
| timer | Config STM timers |
