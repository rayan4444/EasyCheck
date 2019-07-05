//include libraies
//#include "OneWire.h" // for temperature sensor
//#include "DallasTemperature.h" //for temperature sensor (makes it easier to use OneWire)
#include "SPI.h"
#include "SD.h"
#include "Adafruit_GFX.h" //for LCD screen
#include "Adafruit_ILI9341.h"// for LCD screen
#include "wiring_private.h" //pinPeripheral function
#include "Servo.h" // for servo to cover chips
#include "DS2411.h" // reading chip serial numbers
#include "Wire.h" //I2C library
#include "ADC128D818.h" //library to read ADC
#include "RTCZero.h" //library to interface with RTC

//Create an rtc object
RTCZero rtc;

//pin definitions
#define btn1 19 //PB02
#define btn2 25 //PB03
#define btn3 15 //PB08
#define btn4 30 //PB22
#define buzzer 27 //PA28
#define fan0 8 // pa06
#define fan_speed 16 //pb09
#define heater 11 //PA16

//init device temperature sensor
// #define MB_temp 7 //PA21
// OneWire oneWire(MB_temp);
// DallasTemperature tempSensor(&oneWire);

//pins for LCD Screen
#define TFT_DC 17 //PA04
#define TFT_CS 10 //PA18
#define TFT_MOSI 12 //PA19
#define TFT_MISO 6 //PA20
#define TFT_CLK 13 //PA17
#define TFT_RST 31 //PB23


//create screen object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

//create Servo object
Servo servo1;
int servo_angle=10; //change number to tune to correct position

//Serial Number pins
DS2411 ch0(18); //PA05
DS2411 ch1(5); //PA15
DS2411 ch2(2); //PA14
DS2411 ch3(38); //PA13
DS2411 ch4(0); //PA11
DS2411 ch5(1); //PA10
DS2411 ch6(3); //PA09
DS2411 ch7(14); //PA02

//variable to check chip presence
int chip_is_there=0;

//creating ADC objects and specifying address
ADC128D818 adc1(0x1D);
ADC128D818 adc2(0x35);

//Thermistor parameters for temperature calculation
// temp. for nominal resistance
#define TEMPERATURENOMINAL 25
// Nominal resistance (from datatsheet)
#define THERMISTORNOMINAL 10000
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 4250

// SD card
File logfile;
const int chipSelect=4;

//define operation flags
volatile bool btn1_flag=0;
volatile bool btn2_flag=0;
volatile bool btn3_flag=0;
volatile bool btn4_flag=0;
bool proceed_to_test=0;

void setup() {
  SerialUSB.begin(115200);

  //screen setup
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  tft.setCursor(80, 100);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(3);
  tft.println("Easy Check");
  delay(1000);

  //initialize RTC
  rtc.begin();
  rtc.setTime(15, 0, 0);//every time the device powers up it will reset to this date and time.
  rtc.setDate(5, 7, 19);//see README.md file for suggestions to fix this

  //initialise I2C
  Wire.begin();

  //setup for buttons and buzzer
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  pinMode(btn4, INPUT);
  pinMode(buzzer, OUTPUT); //use tone(buzzer, Freq); and noTone(buzzer) to start and stop sound

  attachInterrupt(digitalPinToInterrupt(btn1), btn1_press, FALLING);
  attachInterrupt(digitalPinToInterrupt(btn2), btn2_press, FALLING);
  attachInterrupt(digitalPinToInterrupt(btn3), btn3_press, FALLING);
  attachInterrupt(digitalPinToInterrupt(btn4), btn4_press, FALLING);

  //servo on PA07, pin9
  servo1.attach(9);

  //setup ADC
  adc1.setReferenceMode(INTERNAL_REF);
  adc1.setOperationMode(SINGLE_ENDED);
  adc1.begin();
  adc2.setReferenceMode(INTERNAL_REF);
  adc2.setOperationMode(SINGLE_ENDED);
  adc2.begin();

   //SD card setup
   if (!SD.begin(chipSelect)) {
   Serial.println("SD card reading error");
   delay(2000);
   }
   tft.setCursor(50, 160);
   tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
   tft.println("initialization done");
   delay(1000);

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(80, 100);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  tft.println("Load test chips");
  tft.setCursor(159, 120);
  tft.println("&");
  tft.setCursor(50, 140);
  tft.println("press start to begin");



}

void loop(){
  if (btn4_flag){//start
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(80, 100);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("Starting test...");
    SerialUSB.println("Checking for chips");

    //check if a chip is plugged in by reading ID numbers
    for(int i=0; i<8; i++){

      read_chip_ID(i);
    }
    SerialUSB.print("SUM of chip ID:");
    SerialUSB.println(chip_is_there);

    if (chip_is_there!=0){
      proceed_to_test=1;
      SerialUSB.println("you can test");
      chip_is_there=0;
    } else {
      proceed_to_test=0;
      tft.setCursor(80, 140);
      tft.println("No chips found");
      tft.setCursor(60, 180);
      tft.println("Please insert chips");
      tft.setCursor(80, 200);
      tft.println("and try again");
    }
    btn4_flag=0;
  }

  if (proceed_to_test){
    SerialUSB.println("Test start");
    //create test file
    // log start time and date
    // log chip IDs
    // lower heater
    // >> loop :
    // log first line: time x chip x temp
    // display: test status, warm up , plot avg temp, avg temp, time elapsed
    // check temp: if/ do stuff with heater and fan.
    // write to SD card every 30s
    //time elapsed : 15min beep three times
    // > drying
    // time elapsed 20min beep until stop button pressed.
    // measure resistances, write to SD card, display on sceen with colour.
    proceed_to_test =0; //reset proceed to test flag
  }


}

void btn1_press(){
  SerialUSB.println("BUTTON 1!");
  btn1_flag=1;
}

void btn2_press(){
   SerialUSB.println("BUTTON 2!");
   btn2_flag=1;
}

void btn3_press(){
   SerialUSB.println("BUTTON 3!");
   btn3_flag=1;
}

void btn4_press(){
   SerialUSB.println("BUTTON 4!");
   btn4_flag=1;
}

// void temp_read(){// reads overall device temperature
//  tempSensor.requestTemperaturesByIndex(0);
//  //tempSensor.setWaitForConversion(0); //function call is blocking, uncomment this line to make it non-blocking, but then allow enough time for conversion
//  float t= tempSensor.getTempCByIndex(0);
//  SerialUSB.print("Temperature: ");
//  SerialUSB.print(t); //temperature in degrees celcius reported as float
//  SerialUSB.println(" C");
// }

void cover_chips(){
  for (int i=0; i<servo_angle; i++){
  servo1.write(i+1);
  }
  delay(1000); //delay so that nothing else happens when the servo is moving
}

void uncover_chips(){
  int angle = servo1.read();
  for (int i=0; i<angle; i++){
    servo1.write(angle-1);//decrease angle at each cycle
    delay(10);
  }
  delay(1000); //delay so that nothing else happens when the servo is moving
}

void read_chip_ID(int chip_number){// turn on Chip green LED
  switch (chip_number) {
    case 0:
    uint8_t ch0_ID[8];
     ch0.Init();
     ch0.GetID(ch0_ID);
     SerialUSB.print("Ch0: ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch0_ID[i];
      SerialUSB.print(ch0_ID[i]);
      SerialUSB.print(" ");
     }
     SerialUSB.println();
      break;

    case 1:
     uint8_t ch1_ID[8];
     ch1.Init();
     ch1.GetID(ch1_ID);
     SerialUSB.print("Ch1: ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch1_ID[i];
      SerialUSB.print(ch1_ID[i]);
      SerialUSB.print(" ");
     }
     SerialUSB.println();
      break;

    case 2:
       uint8_t ch2_ID[8];
       ch2.Init();
       ch2.GetID(ch2_ID);
       SerialUSB.print("Ch2: ");

       for (int i=0; i<8; i++){
        chip_is_there+=ch2_ID[i];
        SerialUSB.print(ch2_ID[i]);
        SerialUSB.print(" ");
       }
       SerialUSB.println();
       break;

    case 3:
     uint8_t ch3_ID[8];
     ch3.Init();
     ch3.GetID(ch3_ID);
     SerialUSB.print("Ch3: ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch3_ID[i];
      SerialUSB.print(ch3_ID[i]);
      SerialUSB.print(" ");
     }
     SerialUSB.println();
      break;

    case 4:
     uint8_t ch4_ID[8];
     ch4.Init();
     ch4.GetID(ch4_ID);
     SerialUSB.print("Ch4: ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch4_ID[i];
      SerialUSB.print(ch4_ID[i]);
      SerialUSB.print(" ");
     }
     SerialUSB.println();
     break;

    case 5:
     uint8_t ch5_ID[8];
     ch5.Init();
     ch5.GetID(ch5_ID);
     SerialUSB.print("Ch5: ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch5_ID[i];
      SerialUSB.print(ch5_ID[i]);
      SerialUSB.print(" ");
     }
     SerialUSB.println();
      break;

    case 6:
       uint8_t ch6_ID[8];
     ch6.Init();
     ch6.GetID(ch6_ID);
     SerialUSB.print("Ch6: ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch6_ID[i];
      SerialUSB.print(ch6_ID[i]);
      SerialUSB.print(" ");
     }
     SerialUSB.println();
      break;

    case 7:
      uint8_t ch7_ID[8];
     ch7.Init();
     ch7.GetID(ch7_ID);
     SerialUSB.print("Ch7: ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch7_ID[i];
      SerialUSB.print(ch7_ID[i]);
      SerialUSB.print(" ");
     }
     SerialUSB.println();
      break;
  }
}

float read_chip_temp(int chip_number){//reads temperature of the chip
  //internal vRef= 2.56V
  //12bit ADC: max: 4096
  //see library for converstion details
  float Vout;
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
      Vout= adc1.readConverted(6);
      break;
    case 4:
      Vout= adc2.readConverted(0);
      break;
    case 5:
      Vout= adc2.readConverted(2);
      break;
    case 6:
      Vout= adc2.readConverted(4);
      break;
    case 7:
      Vout= adc2.readConverted(6);
      break;
  }
  //convert to Resistance:
  // Vin=3.3; //in volts
  //R1= 10000; // known resistor in Ohms
  //R2= (Vout*R1)/(Vin-Vout)
  float R2 ;
  R2=3.3-Vout;
  R2=1/R2;
  R2=R2*Vout*10000;

  //Convert to temperature in degC using Steinhart equations
  float temp;
  temp = R2 / THERMISTORNOMINAL;     // (R/Ro)
  temp = log(temp);                  // ln(R/Ro)
  temp /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  temp += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  temp = 1.0 / temp;                 // Invert
  temp -= 273.15;                         // convert to C

  //SerialUSB.println(temp);
  return temp;
}

float read_chip_res(int chip_number){//reads resitance of the chip
  //internal vRef= 2.56V
  //12bit ADC: max: 4096
  //see library for conerstion details
  float Vin=3.3; //in volts
  float Vout;
  float R2 ;
  int R1= 10000; // known resistor in Ohms

    SerialUSB.print("Resistance of chip");
    SerialUSB.print(chip_number);
    SerialUSB.print(": ");

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

  //float res= (Vout*R1)/(Vin-Vout);
  float res=Vout;
  SerialUSB.println(res);

  return res;
}

void print2digits(int number) {
  if (number < 10) {
    SerialUSB.print("0"); // print a 0 before if the number is < than 10
  }
  SerialUSB.print(number);
}

void read_time(){
  // Print date...
 print2digits(rtc.getDay());
 SerialUSB.print("/");
 print2digits(rtc.getMonth());
 SerialUSB.print("/");
 print2digits(rtc.getYear());
 SerialUSB.print(" ");

 // ...and time
 print2digits(rtc.getHours());
 SerialUSB.print(":");
 print2digits(rtc.getMinutes());
 SerialUSB.print(":");
 print2digits(rtc.getSeconds());

 SerialUSB.println();
}
