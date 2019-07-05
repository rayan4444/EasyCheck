# Easy Check rev 2
## Overview
This version includes bug fixes from the previous iteration and is designed to fit in the prototype, made for HAX Xi'an Demo Day 2019. The deisgn has 4 boards:
* A motherboard: power circuit, MCU (ATSAMD21G), control for heater, fans and servo.
* A connection board for testing chips: ADC, chip connectors
* An Interface board: LCD screen, buzzer, 4 buttons, power LED
* Test chip boards: Serial number IC, contacts for sensing paper, power LED, NTC (temperature sensing)

## Hardware Connections

## Code structure
To run all code properly some modification to the original board definition files from ARDUINO are necessary:
- modified files are included in this repository
- enable interrupts on pin PB03(in variant.cpp: change EXTERNAL_INT_NONE to EXTERNAL_INT_3)
- enable interrupt on pin PB22 (in variant.cpp: change EXTERNAL_INT_NONE to EXTERNAL_INT_6)
- modifying ADC128D818 library to correct read function error. files included in this repository

## Errata
- (1) Swapping connections for screen SPI:
    - on hardware: signal "CLK" connects to screen CS pins, signal "MOSI" connects to screen clock, signal "CS" connects to screen MOSI.
    - in firmware: PA17 is now CLK, PA19 is now MOSI, PA18 is now CS.
- (2) fan control: swap fan and fan_speed
- (3) I2C bus: change R11 and R14 from 4.7k to 2.2k


## Recommendations for future designs:

###### Hardware:
- To implement stackable drawers system: consider having a small microcontroller on each connection board that handles serial communication with the motherboard through RS485. you can then daisy chain the drawers as you stack them. The microcontroller on the drawer board can also handle reading the ADC and serial numbers.
- Accommodating to new test papers: move the resistor divider to the chip PCB, so you can customize it for new types of sensing papers with different resistances.

###### Firmware:
- Consider using hardware SPI for communication to the display screen
- Consider modifying libraries to include only the necessary parts and reduce memory usage
