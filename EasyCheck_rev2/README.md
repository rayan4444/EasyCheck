# Easy Check rev 2
## Overview
This version includes bug fixes from the previous iteration and is designed to fit in the prototype, made for HAX Xi'an Demo Day 2019. The design has 4 boards:
* A motherboard: power circuit, MCU (ATSAMD21G), control for heater, fans and servo.
* A connection board for testing chips: ADC, chip connectors
* An Interface board: LCD screen, buzzer, 4 buttons, power LED
* Test chip boards: Serial number IC, contacts for sensing paper, power LED, NTC (temperature sensing)

## Hardware Connections
## Programming
To set up the programming environment, follow the same steps as rev1.

**ATTENTION!**

To run all code properly some modification to the original board definition files from ARDUINO and 3rd party libraries are necessary: the modified files are available in this repository in the /Board Definition and /Libraries used folders.



## Errata
- I2C bus: change R11 and R14 from 4.7k to 2.2k
- The design doesn't account for a battery to keep the RTC powered on. Time gets reset to a user specified date and time at every reboot.

**Note**: the 2019/07/12 prototype uses a slightly different version of the PCB with connection errors that have ALREADY BEEN CORRECTED in the files in this repository
## Recommendations for future designs:

###### Hardware:
- To implement stackable drawers system: consider having a small microcontroller on each connection board that handles serial communication with the motherboard through RS485. you can then daisy chain the drawers as you stack them. The microcontroller on the drawer board can also handle reading the ADC and serial numbers.
- Accommodating to new test papers: move the resistor divider to the chip PCB, so you can customize it for new types of sensing papers with different resistances.

- This design uses an 8W heater, which makes the heating up process slow. Use a more powerful heater to get the system to reach 37 degrees faster. However, make sure the current consumption is within the limits of your power supply.
- Data: consider having some sort of connectivity (Wifi/ethernet) to export data and update the RTC clock.


###### Firmware:
- Scaling of the x-axis in the LCD diplay temperature plot according to test duration
- Improve temperature PID controller by tuning Ki, Kp and Kd better
- Consider using hardware SPI for communication to the display screen
- Consider modifying libraries to include only the necessary parts and reduce memory usage

- Improve timing: consider using timer interrupts to have more precise timing and simplify code structure
- Improve memory usage (variables, function structures etc.)
