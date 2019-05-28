# EasyCheck rev1

This first revision is meant for a purely test basis.
As there are many hardware bugs on this version, the code is just a collection of functions meant to be modified to test a specific feature independently; in preparation for a better version 2 board.

## Hardware Connections
(arduino PIN), Chip Pin: functionality

###### Programming:
- (44) PA30: SWCLK
- (45) PA31: SWDIO
- (28) PA24: USBD_N
- (29) PA25: USBD_P
- NRESET: nRESET

###### Chip Status:
- (26) PA27: Chip 7 status
- (31) PB23: Chip 6 status
- (8) PA06: Chip 5 status
- (9) PA07: Chip 4 status
- (1) PA10: Chip 3 status
- (0) PA11: Chip 2 status
- (2) PA14: Chip 1 status
- (5) PA15: Chip 0 status

###### Chip Serial Number:
- (6) PA20: Serial number 1W

###### User Input:
- (19) PB02: Button 1
- (25) PB03: Button2

###### External ADCs (resistance and chip temperature data):
- (20) PA22 SERCOM 3+5.0: SDA1
- (21) PA23 SERCOM3+5.1: SCL1

###### I2C OLED DISPLAY
- (22) PA12 SERCOM2+4.0: SDA2
- (28) PA13 SERCOM2+4.1: SCL2

###### External DAC (power supply TEC controllers):
- (4) PA08 SERCOM0 + 2.0: SDA3
- (3) PA09 SERCOM 0+ 2.1: SCL3
- (17) PA04: LDAC
- (18) PA05: TEC_EN

###### SPI SD CARD:
- (11) PA16 SERCOM1.0: MOSI
- (13) PA17 SERCOM1.1: SCK
- (10) PA18 SERCOM1.2: /SS
- (12) PA19 SERCOM1.3: MISO
- (30) PB22: Card Detect

###### Device Temperature:
- (7) PA21: 1W Temperature

###### Motor driver TEC input Or Alternative power supply TEC input:
- (15) PB08: PWM0
- (16) PB09: PWM1

###### Alternative power supply TEC input:
- (23) PB10: PWM2
- (24) PB11: PWM3

###### Internal DAC：
- PA02: DAC

## Hardware Errata and fixes

###### Power rail
- 12V to 5V switching regulator: changed from minimum ripple configuration (not working) to minimum cost configuration (see LM5085 datasheet). Still having issues with it output not smooth enough.
- Forgot to connect USB power line to the 5V rail

###### Signals
- I2C1: looks OK
- I2C2: rounded waveform but still functional
- I2C3: does not work. Still uncertain why.
- PWM 2&3 cannot be used with Arduino bootloader
- change 1W resistor to 1.5k for serial number reading
- SD card: remove CS pin pullup

###### Conceptual Errors
- For next version: remove chip status
- Connect every chip serial number to a different pin
- Change I2S screen to SPI screen
- Use Adalogger M0 design for SD card connection
- Change TEC to resistive heater: new driver


## References and Tutorials
###### Programming using OpenOCD and a Raspberry Pi:
- https://learn.adafruit.com/programming-microcontrollers-using-openocd-on-raspberry-pi/overview

###### Assigning SERCOM on SAMD21G:
- https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/overview

###### Logging data on SD card using RTC for timestamps
- https://github.com/cavemoa/Feather-M0-Adalogger/blob/master/LowPowerLogger/LowPowerLogger.ino
