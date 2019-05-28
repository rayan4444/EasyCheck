/*
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* + Pin number +  ZERO Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            | Digital Low      |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 0          | 0 -> RX          |  PA11  |                 | EIC/EXTINT[11] ADC/AIN[19]           PTC/X[3] *SERCOM0/PAD[3]  SERCOM2/PAD[3]  TCC0/WO[3]  TCC1/WO[1]
* | 1          | 1 <- TX          |  PA10  |                 | EIC/EXTINT[10] ADC/AIN[18]           PTC/X[2] *SERCOM0/PAD[2]                  TCC0/WO[2]  TCC1/WO[0]
* | 2          | 2                |  PA14  |                 | EIC/EXTINT[14]                                 SERCOM2/PAD[2]  SERCOM4/PAD[2]  TC3/WO[0]   TCC0/WO[4]
* | 3          | ~3               |  PA09  |                 | EIC/EXTINT[9]  ADC/AIN[17]           PTC/X[1]  SERCOM0/PAD[1]  SERCOM2/PAD[1] *TCC0/WO[1]  TCC1/WO[3]
* | 4          | ~4               |  PA08  |                 | EIC/NMI        ADC/AIN[16]           PTC/X[0]  SERCOM0/PAD[0]  SERCOM2/PAD[0] *TCC0/WO[0]  TCC1/WO[2]
* | 5          | ~5               |  PA15  |                 | EIC/EXTINT[15]                                 SERCOM2/PAD[3]  SERCOM4/PAD[3] *TC3/WO[1]   TCC0/WO[5]
* | 6          | ~6               |  PA20  |                 | EIC/EXTINT[4]                        PTC/X[8]  SERCOM5/PAD[2]  SERCOM3/PAD[2]             *TCC0/WO[6]
* | 7          | 7                |  PA21  |                 | EIC/EXTINT[5]                        PTC/X[9]  SERCOM5/PAD[3]  SERCOM3/PAD[3]              TCC0/WO[7]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            | Digital High     |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 8          | ~8               |  PA06  |                 | EIC/EXTINT[6]  ADC/AIN[6]  AC/AIN[2] PTC/Y[4]  SERCOM0/PAD[2]                 *TCC1/WO[0]
* | 9          | ~9               |  PA07  |                 | EIC/EXTINT[7]  ADC/AIN[7]  AC/AIN[3] PTC/Y[5]  SERCOM0/PAD[3]                 *TCC1/WO[1]
* | 10         | ~10              |  PA18  |                 | EIC/EXTINT[2]                        PTC/X[6] +SERCOM1/PAD[2]  SERCOM3/PAD[2] *TC3/WO[0]    TCC0/WO[2]
* | 11         | ~11              |  PA16  |                 | EIC/EXTINT[0]                        PTC/X[4] +SERCOM1/PAD[0]  SERCOM3/PAD[0] *TCC2/WO[0]   TCC0/WO[6]
* | 12         | ~12              |  PA19  |                 | EIC/EXTINT[3]                        PTC/X[7] +SERCOM1/PAD[3]  SERCOM3/PAD[3]  TC3/WO[1]   *TCC0/WO[3]
* | 13         | ~13              |  PA17  | LED             | EIC/EXTINT[1]                        PTC/X[5] +SERCOM1/PAD[1]  SERCOM3/PAD[1] *TCC2/WO[1]   TCC0/WO[7]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            | Analog Connector |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 14         | A0               |  PA02  | A0              | EIC/EXTINT[2] *ADC/AIN[0]  DAC/VOUT  PTC/Y[0]
* | 15         | A1               |  PB08  | A1              | EIC/EXTINT[8] *ADC/AIN[2]            PTC/Y[14] SERCOM4/PAD[0]                  TC4/WO[0]
* | 16         | A2               |  PB09  | A2              | EIC/EXTINT[9] *ADC/AIN[3]            PTC/Y[15] SERCOM4/PAD[1]                  TC4/WO[1]
* | 17         | A3               |  PA04  | A3              | EIC/EXTINT[4] *ADC/AIN[4]  AC/AIN[0] PTC/Y[2]  SERCOM0/PAD[0]                  TCC0/WO[0]
* | 18         | A4               |  PA05  | A4              | EIC/EXTINT[5] *ADC/AIN[5]  AC/AIN[1] PTC/Y[5]  SERCOM0/PAD[1]                  TCC0/WO[1]
* | 19         | A5               |  PB02  | A5              | EIC/EXTINT[2] *ADC/AIN[10]           PTC/Y[8]  SERCOM5/PAD[0]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            | Wire             |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 20         | SDA              |  PA22  | SDA             | EIC/EXTINT[6]                        PTC/X[10] *SERCOM3/PAD[0] SERCOM5/PAD[0] TC4/WO[0] TCC0/WO[4]
* | 21         | SCL              |  PA23  | SCL             | EIC/EXTINT[7]                        PTC/X[11] *SERCOM3/PAD[1] SERCOM5/PAD[1] TC4/WO[1] TCC0/WO[5]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            |SPI (Legacy ICSP) |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 22         | 1                |  PA12  | MISO            | EIC/EXTINT[12] SERCOM2/PAD[0] *SERCOM4/PAD[0] TCC2/WO[0] TCC0/WO[6]
* |            | 2                |        | 5V0             |
* | 23         | 4                |  PB10  | MOSI            | EIC/EXTINT[10]                *SERCOM4/PAD[2] TC5/WO[0]  TCC0/WO[4]
* | 24         | 3                |  PB11  | SCK             | EIC/EXTINT[11]                *SERCOM4/PAD[3] TC5/WO[1]  TCC0/WO[5]
* |            | 5                |        | RESET           |
* |            | 6                |        | GND             |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            | LEDs             |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 25         |                  |  PB03  | RX              |
* | 26         |                  |  PA27  | TX              |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            | USB              |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 27         |                  |  PA28  | USB_HOST_ENABLE | EIC/EXTINT[8]
* | 28         |                  |  PA24  | USB_NEGATIVE    | *USB/DM
* | 29         |                  |  PA25  | USB_POSITIVE    | *USB/DP
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            | EDBG             |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 30         |                  |  PB22  | EDBG_UART TX    | *SERCOM5/PAD[2]
* | 31         |                  |  PB23  | EDBG_UART RX    | *SERCOM5/PAD[3]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 32         |                  |  PA22  | EDBG_SDA        | Pin 20 (SDA)
* | 33         |                  |  PA23  | EDBG_SCPA0L        | Pin 21 (SCL)
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 34         |                  |  PA19  | EDBG_MISO       | EIC/EXTINT[3] *SERCOM1/PAD[3] SERCOM3/PAD[3] TC3/WO[1]  TCC0/WO[3]
* | 35         |                  |  PA16  | EDBG_MOSI       | EIC/EXTINT[0] *SERCOM1/PAD[0] SERCOM3/PAD[0] TCC2/WO[0] TCC0/WO[6]
* | 36         |                  |  PA18  | EDBG_SS         | EIC/EXTINT[2] *SERCOM1/PAD[2] SERCOM3/PAD[2] TC3/WO[0]  TCC0/WO[2]
* | 37         |                  |  PA17  | EDBG_SCK        | EIC/EXTINT[1] *SERCOM1/PAD[1] SERCOM3/PAD[1] TCC2/WO[1] TCC0/WO[7]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 38         | ATN              |  PA13  | EDBG_GPIO0      | EIC/EXTINT[13] SERCOM2/PAD[1] SERCOM4/PAD[1] *TCC2/WO[1] TCC0/WO[7]
* | 39         |                  |  PA21  | EDBG_GPIO1      | Pin 7
* | 40         |                  |  PA06  | EDBG_GPIO2      | Pin 8
* | 41         |                  |  PA07  | EDBG_GPIO3      | Pin 9
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            |                  |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            | GND              |        |                 |
* | 42         | AREF             |  PA03  |                 | EIC/EXTINT[3] *[ADC|DAC]/VREFA ADC/AIN[1] PTC/Y[1]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* | 43         |                  |  PA02  |                 | Alternate use of A0 (DAC output)
* | 44         |                  |  PA30  |                 | SWCLK, alternate use EXTINT[10] TCC1/WO[0] SERCOM1/PAD[2]
* | 45         |                  |  PA31  |                 | SWDIO, alternate use EXTINT[11] TCC1/WO[1] SERCOM1/PAD[3]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            |32.768KHz Crystal |        |                 |
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
* |            |                  |  PA00  | XIN32           | EIC/EXTINT[0] SERCOM1/PAD[0] TCC2/WO[0]
* |            |                  |  PA01  | XOUT32          | EIC/EXTINT[1] SERCOM1/PAD[1] TCC2/WO[1]
* +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
*/

//#define ECHO_TO_SERIAL // Allows serial output if uncommented

#include "SPI.h"
#include "SD.h"
#include "Wire.h"
#include "ADC128D818.h"
#include "RTCZero.h"
#include "OneWire.h" // for temperature sensor
#include "DallasTemperature.h" //for temperature sensor (makes it easier to use OneWire)
#include "wiring_private.h" //pinPeripheral function

  //section incomplete. To be completed once pinout for rev2 is confirmed
  // #include "DS2411.h"
  // DS2411 ch0();
  // DS2411 ch1();
  // DS2411 ch2();
  // DS2411 ch3();
  // DS2411 ch4();
  // DS2411 ch5();
  // DS2411 ch6();
  // DS2411 ch7();

//Setup Overall device temperature sensor
OneWire oneWire(7);
DallasTemperature tempSensor(&oneWire);
OneWire ChipID(6);

ADC128D818 adc1(0x1D);
ADC128D818 adc2(0x35);

const int chipSelect = 10;

#define status_0 5
#define status_1 2
#define status_2 0
#define status_3 1
#define status_4 9
#define status_5 8
#define status_6 31
#define status_7 26

#define pwm0 15
#define pwm1 16

#define btn1 19
#define btn2 25

//define SPI interface
SPIClass SPI1 (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3); //Sercom1 for SPI to sdcard

//define new I2C interfaces
TwoWire I2C1(&sercom3, 20, 21); //Sercom 3 for I2C1 to external ADC
TwoWire I2C2(&sercom4, 22, 38); // Sercom 4 for I2C2 to  OLED display
//Debugging  I2C3
//TwoWire I2C3(&sercom2, 4, 3); // Sercom2 for I2C3 to external DAC

volatile bool new_test_status =0;
volatile bool current_test_status=0;

#define cardSelect 10;

extern "C" char *sbrk(int i);

// Change these values to set the current initial time
const byte hours = 12;
const byte minutes = 0;
const byte seconds = 0;
// Change these values to set the current initial date
const byte day = 4;
const byte month = 12;
const byte year = 95;

RTCZero rtc;    // Create RTC object
File logfile;   // Create file object
int NextAlarmSec; // Variable to hold next alarm time in seconds
unsigned int CurrentCycleCount;  // Num of smaples in current cycle, before uSD flush call

void setup(){
 pinMode(status_0, OUTPUT);
 pinMode(status_1, OUTPUT);
 pinMode(status_2, OUTPUT);
 pinMode(status_3, OUTPUT);
 pinMode(status_4, OUTPUT);
 pinMode(status_5, OUTPUT);
 pinMode(status_6, OUTPUT);
 pinMode(status_7, OUTPUT);

 pinMode(pwm0, OUTPUT);
 pinMode(pwm1, OUTPUT);

attachInterrupt(19, btn1_press() , FALLING);
attachInterrupt(25, btn2_press() , FALLING);

  SerialUSB.begin(115200);
//wait for Serial port
  while(!SerialUSB){}

  // begin SPI communication
  SPI1.begin();
  //begin I2C communication
  I2C2.begin();
  I2C3.begin();

  // Assign pins 11, 12, 13 to SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);

   //Assign pins to SERCOM functionality (I2C1)
   pinPeripheral(20, PIO_SERCOM);
   pinPeripheral(21, PIO_SERCOM);

   //Assign pins to SERCOM functionality (I2C2)
   pinPeripheral(22, PIO_SERCOM_ALT);
   pinPeripheral(38, PIO_SERCOM_ALT);

   // //Assign pins to SERCOM functionality (I2C3)
   // pinPeripheral(4, PIO_SERCOM_ALT);
   // pinPeripheral(3, PIO_SERCOM_ALT);

   //Enable external Interrupt on pin 25 (PB03)
   pinPeripheral(25, PIO_EXTINT);

   adc1.setReferenceMode(INTERNAL_REF);
   adc1.setOperationMode(SINGLE_ENDED);
   adc1.begin();

   adc2.setReferenceMode(INTERNAL_REF);
   adc2.setOperationMode(SINGLE_ENDED);
   adc2.begin();

   rtc.begin();    // Start the RTC in 24hr mode
   rtc.setTime(hours, minutes, seconds);   // Set the time
   rtc.setDate(day, month, year);    // Set the date


   // see if the card is present and can be initialized:
   if (!SD.begin(cardSelect)) {
     SerialUSB.println("Card init. failed! or Card not present");
   }
   char filename[15];
   strcpy(filename, "TESTLOG00.CSV");
   for (uint8_t i = 0; i < 100; i++) {
     filename[6] = '0' + i/10;
     filename[7] = '0' + i%10;
     // create if does not exist, do not open existing, write, sync after write
     if (! SD.exists(filename)) {
       break;
     }
   }

   logfile = SD.open(filename, FILE_WRITE);
   if( ! logfile ) {
     SerialUSB.print("Couldnt create ");
     SerialUSB.println(filename);
   }
   Serial.print("Writing to ");
   Serial.println(filename);

   Serial.println("Logging ....");

   //write list of  Chip ID to Logfile
   SD_chipID();

   //writing header to logfile
   logfile.println("DD:MM:YYYY hh:mm:ss, Device Temperature, Chip0 Temp, Chip0 Res, Chip1 Temp, Chip1 Res, Chip2 Temp, Chip2 Res, Chip3 Temp, Chip3 Res, Chip4 Temp, Chip4 Res, Chip5 Temp, Chip5 Res, Chip6 Temp, Chip6 Res, Chip7 Temp, Chip7 Res");
 }

void loop(){
  while (current_test_status==1){
    CurrentCycleCount += 1;     //  Increment samples in current uSD flush cycle

    #ifdef ECHO_TO_SERIAL
      SerialOutput();           // Only logs to serial if ECHO_TO_SERIAL is uncommented at start of code
    #endif

    SdOutput();                 // Output to uSD card

    // Code to limit the number of power hungry writes to the uSD
    if( CurrentCycleCount >= SamplesPerCycle ) {
      logfile.flush();
      CurrentCycleCount = 0;
      #ifdef ECHO_TO_SERIAL
        Serial.println("logfile.flush() called");
      #endif
    }
    //delay(SampleIntSeconds);   // Simple delay for testing only interval set by const in header

    // timing interval and sleep code
    NextAlarmSec = (NextAlarmSec + SampleIntSec) % 60;   // i.e. 65 becomes 5
    rtc.setAlarmSeconds(NextAlarmSec); // RTC time to wake, currently seconds only
    rtc.enableAlarm(rtc.MATCH_SS); // Match seconds only
    rtc.attachInterrupt(alarmMatch); // Attaches function to be called, currently blank
    delay(50); // Brief delay prior to sleeping not really sure its required

    rtc.standbyMode();    // Sleep until next alarm match
  }
  // run whatever happens when the test is not running
}

void temp_read(){// reads overall device temperature
 tempSensor.requestTemperaturesByIndex(0);
 //tempSensor.setWaitForConversion(0); //function call is blocking, uncomment this line to make it non-blocking, but then allow enough time for conversion
 float t= tempSensor.getTempCByIndex(0);
 SerialUSB.print("Temperature: ");
 SerialUSB.print(t); //temperature in degrees celcius reported as float
 SerialUSB.println(" C");
}

float read_chip_res(int chip_number){//reads resitance of the chip
  //internal vRef= 2.56V
  //12bit ADC: max: 4096
  //see library for conerstion details
  float Vin=3.3; //in volts
  float Vout;
  float R2 ;
  int R1= 10000; // known resistor in Ohms

  #ifdef ECHO_TO_SERIAL
    SerialUSB.print("Resistance of chip");
    SerialUSB.print(chip_number);
    SerialUSB.print(": ");
  #endif

  switch (chip_number) {
    case 0:
      //adc1.read(1)
      //Vout= adc1.read(1)*(2.56)/4096;
      // SerialUSB.print("Resistance of chip 0: ");
      Vout = adc1.readConverted(1) ;

    case 1:
      Vout= adc1.readConverted(3);
      break;
    case 2:
      Vout= adc1.readConverted(5);
      break;
    case 3:
      Vout= adc1.readConverted (7);
      break;
    case 4:
      Vout= adc2.readConverted (1);
      break;
    case 5:
      Vout= adc2.readConverted (3);
      break;
    case 6:
      Vout= adc2.readConverted (5);
      break;
    case 7:
      Vout= adc2.readConverted (7);
      break;
    }

  float res= (Vout*R1)/(Vin-Vout);
  #ifdef ECHO_TO_SERIAL
  SerialUSB.println(res);
  #endif
  return res;
}

float read_chip_temp(int chip_number){//reads temperature of the chip
  //internal vRef= 2.56V
  //12bit ADC: max: 4096
  //see library for conerstion details
  float Vin=3.3; //in volts
  float Vout;
  float R2 ;
  int R1= 10000; // known resistor in Ohms
#ifdef ECHO_TO_SERIAL
  SerialUSB.print("Temperature of chip");
  SerialUSB.print(chip_number);
  SerialUSB.print(": ");
#endif

  switch (chip_number) {
    case 0:
      //adc1.read(0)
      //Vout= adc1.read(0)*(2.56)/4096;
      Vout = adc1.readConverted(0);
      break;
    case 1:
      Vout= adc1.readConverted(2);
      break;
    case 2:
      Vout= adc1.readConverted(4);
      break;
    case 3:
      Vout= adc1.readConverted (6);
      break;
    case 4:
      Vout= adc2.readConverted (0);
      break;
    case 5:
      Vout= adc2.readConverted (2);
      break;
    case 6:
      Vout= adc2.readConverted (4);
      break;
    case 7:
      Vout= adc2.readConverted (6);
      break;
  }
  float temp= (Vout*R1)/(Vin-Vout);

  #ifdef ECHO_TO_SERIAL
  SerialUSB.println(temp);
  #endif

  return temp;
}

byte read_chip_ID(int chip_number){//reads serial number of the chip
byte data[8];
SerialUSB.print("Chip");
SerialUSB.print(chip_number);
SerialUSB.print(": ");

switch (chip_number) {
  case 0:
    ch0.Init();
    ch0.getID(data);
    break;
  case 1:
    ch1.Init() ;
    ch1.getID(data);
    break;
  case 2:
    ch2.Init();
    ch2.getID(data);
    break;
  case 3:
    ch3.Init();
    ch3.getID(data);
    break;
  case 4:
    ch4.Init();
    ch4.getID(data);
    break;
  case 5:
    ch5.Init();
    ch5.getID(data);
    break;
  case 6:
    ch6.Init();
    ch6.getID(data);
    break;
  case 7:
    ch7.Init();
    ch7.getID(data);
    break;
}
  for(byte a=0;a<8;a++){
    SerialUSB.print(data[a]);
    SerialUSB.print(" ");
  }
  Serial.println();
  return data;
}

void update_chip_status(int chip_number){// turn on Chip green LED
  switch (chip_number) {
    case 0:
      digitalWrite(5,!(digitalRead(5));
      break;

    case 1:
      digitalWrite(2),!(digitalRead(2));
      break;

    case 2:
      digitalWrite(0,!(digitalRead(0));
      break;

    case 3:
      digitalWrite(1,!(digitalRead(1));
      break;

    case 4:
      digitalWrite(9,!(digitalRead(9));
      break;

    case 5:
      digitalWrite(8,!(digitalRead(8));
      break;

    case 6:
      digitalWrite(31,!(digitalRead(31));
      break;

    case 7:
      digitalWrite(26,!(digitalRead(26));
      break;
  }

}

void btn1_press(){
  //btn1 is pressed, start the test
  new_test_status=1;
  SerialUSB.println("btn1_press");
  start_test();
}

void btn2_press(){
  new_test_status=0;
  SerialUSB.println("btn2_press");
  stop_test();
}

void start_test(){
  //ignore double press on button
  //allow to later display options on change of status
  if (new_test_status!=current_test_status){
    current_test_status=new_test_status;
  }
}

void stop_test(){
  //ignore double press on button
  //allow to later display options on change of status
  if (new_test_status!=current_test_status){
    Current_test_status=new_test_status;
  }
}

void SerialOutput(){
  SerialUSB.print(CurrentCycleCount);
  SerialUSB.print(":");
  SerialUSB.print(freeram ());
  SerialUSB.print("-");
  SerialUSB.print(rtc.getDay());
  SerialUSB.print("/");
  SerialUSB.print(rtc.getMonth());
  SerialUSB.print("/");
  SerialUSB.print(rtc.getYear()+2000);
  SerialUSB.print(" ");
  SerialUSB.print(rtc.getHours());
  SerialUSB.print(":");
  if(rtc.getMinutes() < 10){
    SerialUSB.print('0');
  }      // Trick to add leading zero for formatting
  SerialUSB.print(rtc.getMinutes());
  SerialUSB.print(":");
  if(rtc.getSeconds() < 10){
    SerialUSB.print('0');
  }   // Trick to add leading zero for formatting
  SerialUSB.print(rtc.getSeconds());
  SerialUSB.print(",");
  SerialUSB.println(temp_read());   // Print temperature
}

void SdOutput() {
  // Formatting for file out put dd/mm/yyyy hh:mm:ss, [sensor output]
  logfile.print(CurrentCycleCount);
  logfile.print("-");
  logfile.print(rtc.getDay());
  logfile.print("/");
  logfile.print(rtc.getMonth());
  logfile.print("/");
  logfile.print(rtc.getYear()+2000);
  logfile.print(" ");
  logfile.print(rtc.getHours());
  logfile.print(":");
  if(rtc.getMinutes() < 10)
    logfile.print('0');      // Trick to add leading zero for formatting
  logfile.print(rtc.getMinutes());
  logfile.print(":");
  if(rtc.getSeconds() < 10)
    logfile.print('0');      // Trick to add leading zero for formatting
  logfile.print(rtc.getSeconds());
  logfile.print(",");
  logfile.print(temp_read());
  logfile.print(",");
  logfile.print(read_chip_temp(0));
  logfile.print(",");
  logfile.print(read_chip_res(0));
  logfile.print(",");
  logfile.print(read_chip_temp(1));
  logfile.print(",");
  logfile.print(read_chip_res(1));
  logfile.print(",");
  logfile.print(read_chip_temp(2));
  logfile.print(",");
  logfile.print(read_chip_res(2));
  logfile.print(",");
  logfile.print(read_chip_temp(3));
  logfile.print(",");
  logfile.print(read_chip_res(3));
  logfile.print(",");
  logfile.print(read_chip_temp(4));
  logfile.print(",");
  logfile.print(read_chip_res(4));
  logfile.print(",");
  logfile.print(read_chip_temp(5));
  logfile.print(",");
  logfile.print(read_chip_res(5));
  logfile.print(",");
  logfile.print(read_chip_temp(6));
  logfile.print(",");
  logfile.print(read_chip_res(6));
  logfile.print(",");
  logfile.print(read_chip_temp(7));
  logfile.print(",");
  logfile.print(read_chip_res(7));
  logfile.print(",");
}

int freeram() {
  char stack_dummy = 0;
  return &stack_dummy - sbrk(0);
}

void SD_chipID(){
  logfile.print("Chip0: ");
  logfile.println(read_chip_ID(0));
  logfile.print("Chip1: ");
  logfile.println(read_chip_ID(1));
  logfile.print("Chip2: ");
  logfile.println(read_chip_ID(2));
  logfile.print("Chip3: ");
  logfile.println(read_chip_ID(3));
  logfile.print("Chip4: ");
  logfile.println(read_chip_ID(4));
  logfile.print("Chip5: ");
  logfile.println(read_chip_ID(5));
  logfile.print("Chip6: ");
  logfile.println(read_chip_ID(6));
  logfile.print("Chip7: ");
  logfile.println(read_chip_ID(7));
}
