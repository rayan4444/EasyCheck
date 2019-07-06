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

//define test  parameters
#define TEST_DURATION 300000 //5 minutes
    //#define TEST_DURATION 900000//15 minutes
    //#define TEST_DURATION 7200000 //2 hours
#define LOG_INTERVAL 10000 //10 seconds
    // #define LOG_INTERVAL 60000 //1 minute
    // #define LOG_INTERVAL 300000//5 minutes
#define TEST_TEMP 37.0 //temperature to keep for the test
#define DRYING_DURATION 60000 //1 minute
    //#define DRYING_DURATION 300000 //5 minutes
    //#define DRYING_DURATION 900000 //15 minutes

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
int servo_angle=35; //change number to tune to correct position

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
File main_folder; // initialize folder for saving
File logfile; // initialize sd file
const int chipSelect=4;
int prev_file_indx = 0; // used for file naming
String fileName = "000";

//define operation flags and variables
volatile bool btn1_flag=0;
volatile bool btn2_flag=0;
volatile bool btn3_flag=0;
volatile bool btn4_flag=0;
bool proceed_to_test=0;
unsigned long start_time=0;
int x_temp=0; //x-axis for plotting temperature on screen

//define temperature PID control variables
unsigned long lastTime;
float Input, Output, Output_heat, Output_fan, Setpoint;
float errSum, lastErr;
float kp, ki, kd;

//containers for last resistance results
float Chip0_last_res =0;
float Chip1_last_res =0;
float Chip2_last_res =0;
float Chip3_last_res =0;
float Chip4_last_res =0;
float Chip5_last_res =0;
float Chip6_last_res =0;
float Chip7_last_res =0;

void setup() {
  SerialUSB.begin(115200);

  //screen setup
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  tft.setCursor(60, 100);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(3);
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

   //setup PID tuning
   kp=200;
   ki=0;
   kd=0;
   Setpoint=TEST_TEMP;

   //lift aluminum cover to make the drawer
   uncover_chips();

   //startup screen
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
    start_time= millis();
    // close aluminum lid on chips
    cover_chips();

    //update screen to display test progress
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(60, 20);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.println("TEST IN PROGRESS");
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.setCursor (100,50);
    tft.print("Incubating");tft.setCursor (20,80);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.print("Temperature");
    tft.setCursor (220,80);
    tft.print("Time");
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.setCursor(50,110);
    //print initial average temperature
    tft.print(avg_temp()); tft.print(char(167));tft.print("C");
    tft.setCursor(220,110);
    print_time_elapsed();
    tft.setCursor(10, 130);
    tft.println("T(C)");
    tft.drawLine(20, 150, 20, 220,ILI9341_WHITE);
    tft.setTextSize(1);
    tft.setCursor(5, 150);
    tft.println("40");
    tft.setCursor(5, 215);
    tft.println("25");
    tft.setTextSize(2);



    SerialUSB.println("Test start");
    main_folder = SD.open("/"); // open SD card main directory
    fileName = sd_saver(main_folder); //create a new file according to the naming convention

    // SD Card writing and saving
    logfile = SD.open("DATA"+fileName+".csv",FILE_WRITE);
    // if the file is valid, write to it:
    if(logfile){
      logfile.println("EASYCHECK TEST LOG");// File Title
      SerialUSB.println("EASYCHECK TEST LOG");

      logfile.print("TEST DATE: ");
      SerialUSB.print("TEST DATE: ");
      read_date(); //get date

      logfile.println("CHIP ID:");
      SerialUSB.println("CHIP ID:");

      for (int i=0; i<8; i++){// get IDs for all the chips. non-plugged in chips will have ID 0
        read_chip_ID(i);
      }
      //headings for data log
      logfile.println("TEMPERATURE LOG, DEGREES CELCIUS");
      SerialUSB.println("TEMPERATURE LOG, DEGREES CELCIUS");

      logfile.print("TIME"); logfile.print("\t");
      SerialUSB.print("TIME"); SerialUSB.print("\t");

      logfile.print("CHIP 0"); logfile.print("\t");
      SerialUSB.print("CHIP 0"); SerialUSB.print("\t");

      logfile.print("CHIP 1"); logfile.print("\t");
      SerialUSB.print("CHIP 1"); SerialUSB.print("\t");

      logfile.print("CHIP 2"); logfile.print("\t");
      SerialUSB.print("CHIP 2"); SerialUSB.print("\t"); ;

      logfile.print("CHIP 3"); logfile.print("\t");
      SerialUSB.print("CHIP 3"); SerialUSB.print("\t");

      logfile.print("CHIP 4"); logfile.print("\t");
      SerialUSB.print("CHIP 4"); SerialUSB.print("\t");

      logfile.print("CHIP 5"); logfile.print("\t");
      SerialUSB.print("CHIP 5"); SerialUSB.print("\t");

      logfile.print("CHIP 6"); logfile.print("\t");
      SerialUSB.print("CHIP 6"); SerialUSB.print("\t");

      logfile.print("CHIP 7"); logfile.print("\n");
      SerialUSB.print("CHIP 7"); SerialUSB.print("\n");

      unsigned long test_time = millis();
      unsigned long log_time = millis();
      x_temp=0;

      while (test_time-start_time <= TEST_DURATION){
        //take the average temperature accross chips to calculate PID outpur
        Input = avg_temp();
        compute_temp_PID();
        analogWrite(heater, Output_heat);
        analogWrite(fan0, Output_fan);

        // refresh temperature plot, avg temp and time.
        refresh_temp();
        tft.print(avg_temp());tft.print(" ");tft.print(char(167));tft.print("C");
        refresh_time();
        print_time_elapsed();
        x_temp++;
        plot_temp();

          //log temperatures whenever it's time
          if(test_time-log_time >= LOG_INTERVAL){
            read_time();
            logfile.print("\t");
            SerialUSB.print("\t");

            logfile.print(read_chip_temp(0)); logfile.print("\t");
            SerialUSB.print(read_chip_temp(0));SerialUSB.print("\t");

            logfile.print(read_chip_temp(1)); logfile.print("\t");
            SerialUSB.print(read_chip_temp(1)); SerialUSB.print("\t");

            logfile.print(read_chip_temp(2)); logfile.print("\t");
            SerialUSB.print(read_chip_temp(2)); SerialUSB.print("\t");

            logfile.print(read_chip_temp(3)); logfile.print("\t");
            SerialUSB.print(read_chip_temp(3)); SerialUSB.print("\t");

            logfile.print(read_chip_temp(4)); logfile.print("\t");
            SerialUSB.print(read_chip_temp(4)); SerialUSB.print("\t");

            logfile.print(read_chip_temp(5)); logfile.print("\t");
            SerialUSB.print(read_chip_temp(5)); SerialUSB.print("\t");

            logfile.print(read_chip_temp(6)); logfile.print("\t");
            SerialUSB.print(read_chip_temp(6)); SerialUSB.print("\t");

            logfile.print(read_chip_temp(7)); logfile.print("\n");
            SerialUSB.print(read_chip_temp(7)); SerialUSB.print("\n");

            log_time=millis();
          }

        delay(1000);

        test_time=millis();
        if (btn3_flag){//check if the pause button has been pressed
          logfile.println(); SerialUSB.println();
          logfile.print("Test Aborted"); logfile.print("\n");
          SerialUSB.print("Test Aborted"); SerialUSB.print("\n");
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(80,110);
          tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
          tft.println("TEST ABORTED");

          test_time= start_time+TEST_DURATION+2000; //this will break the loop
        }
      }
      if (!btn3_flag){
      //after the incubation part is done, buzz three times
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
      delay(1000);
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
      delay(1000);
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
      delay(1000);

      // update screen
      refresh_test_type();
      tft.print("Drying");

      //uncover chips
      uncover_chips();

      unsigned long drying_start=millis();
      unsigned long drying_time=millis();

      while(drying_time-drying_start<=DRYING_DURATION){
        //start Drying
        analogWrite(fan0,50);
        // refresh temperature plot, avg temp and time.
        refresh_temp();
        tft.print(avg_temp());tft.print(" ");tft.print(char(167));tft.print("C");
        refresh_time();
        print_time_elapsed();
        x_temp ++;
        plot_temp();

        delay(1000);
        drying_time=millis();

        if (btn3_flag){//check if the pause button has been pressed
          logfile.println(); SerialUSB.println();
          logfile.print("Test Aborted"); logfile.print("\n");
          SerialUSB.print("Test Aborted"); SerialUSB.print("\n");
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(80,110);
          tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
          tft.println("TEST ABORTED");

        drying_time= drying_start+DRYING_DURATION+2000; //this will break the loop
        }
      }

      if(!btn3_flag){
      //turn off the fan
      analogWrite(fan0,255);

      //beep buzzer
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
      delay(1000);
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
      delay(1000);
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
      delay(1000);

      // measure and log resitance
      logfile.println("CHIP RESISTANCE (in Ohms)");
      SerialUSB.println("CHIP RESISTANCE (in Ohms)");

      logfile.print("TIME"); logfile.print("\t");
      SerialUSB.print("TIME"); SerialUSB.print("\t");

      logfile.print("CHIP 0"); logfile.print("\t");
      SerialUSB.print("CHIP 0"); SerialUSB.print("\t");

      logfile.print("CHIP 1"); logfile.print("\t");
      SerialUSB.print("CHIP 1"); SerialUSB.print("\t");

      logfile.print("CHIP 2"); logfile.print("\t");
      SerialUSB.print("CHIP 2"); SerialUSB.print("\t"); ;

      logfile.print("CHIP 3"); logfile.print("\t");
      SerialUSB.print("CHIP 3"); SerialUSB.print("\t");

      logfile.print("CHIP 4"); logfile.print("\t");
      SerialUSB.print("CHIP 4"); SerialUSB.print("\t");

      logfile.print("CHIP 5"); logfile.print("\t");
      SerialUSB.print("CHIP 5"); SerialUSB.print("\t");

      logfile.print("CHIP 6"); logfile.print("\t");
      SerialUSB.print("CHIP 6"); SerialUSB.print("\t");

      logfile.print("CHIP 7"); logfile.print("\n");
      SerialUSB.print("CHIP 7"); SerialUSB.print("\n");

      read_time();
      logfile.print("\t");
      SerialUSB.print("\t");

      Chip0_last_res =read_chip_res(0);
      logfile.print(Chip0_last_res); logfile.print("\t");
      SerialUSB.print(Chip0_last_res);SerialUSB.print("\t");

      Chip1_last_res =read_chip_res(1);
      logfile.print(Chip1_last_res); logfile.print("\t");
      SerialUSB.print(Chip1_last_res); SerialUSB.print("\t");

      Chip2_last_res = read_chip_res(2);
      logfile.print(Chip2_last_res); logfile.print("\t");
      SerialUSB.print(Chip2_last_res); SerialUSB.print("\t");

      Chip3_last_res =read_chip_res(3);
      logfile.print(Chip3_last_res); logfile.print("\t");
      SerialUSB.print(Chip3_last_res); SerialUSB.print("\t");

      Chip4_last_res =read_chip_res(4);
      logfile.print(Chip4_last_res); logfile.print("\t");
      SerialUSB.print(Chip4_last_res); SerialUSB.print("\t");

      Chip5_last_res =read_chip_res(5);
      logfile.print(Chip5_last_res); logfile.print("\t");
      SerialUSB.print(Chip5_last_res); SerialUSB.print("\t");

      Chip6_last_res =read_chip_res(6);
      logfile.print(Chip6_last_res); logfile.print("\t");
      SerialUSB.print(Chip6_last_res); SerialUSB.print("\t");

      Chip7_last_res =read_chip_res(7);
      logfile.print(Chip7_last_res); logfile.print("\n");
      SerialUSB.print(Chip7_last_res); SerialUSB.print("\n");

      //update display
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(60,60);
      tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
      tft.println("TEST COMPLETED");
      tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
      tft.setCursor (20,100);
      tft.print("Press 'Pg Up' button to display results");
      tft.setCursor (20,160);
      tft.print("Press 'Pg Dn' button to exit");

      //close SD card file
      logfile.close();

      // stay on this page until the buttons are pressed
      while (!(btn2_flag|btn1_flag)){
      //just wait
    }
  }}//close if btn3_flag low (both times)
    proceed_to_test =0; //reset proceed to test flag
    }// close if logfile
  }// close  porceed test

  if (btn1_flag){//user pressed PgDn, so just exit and display initial Screen
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(80, 100);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("Load test chips");
    tft.setCursor(159, 120);
    tft.println("&");
    tft.setCursor(50, 140);
    tft.println("press start to begin");

    //reset flag
    btn1_flag=0;
  }

  if( btn2_flag){//user pressed PgUp, so display last resistance results
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(60, 20);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.println("LAST TEST RESULTS");
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.setCursor (20,50);
    tft.print("CHIP 0:");
    tft.println(Chip0_last_res);
    tft.setCursor (20,70);
    tft.print("CHIP 1:");
    tft.println(Chip1_last_res);
    tft.setCursor (20,90);
    tft.print("CHIP 2:");
    tft.println(Chip2_last_res);
    tft.setCursor (20,110);
    tft.print("CHIP 3:");
    tft.println(Chip3_last_res);
    tft.setCursor (20,130);
    tft.print("CHIP 4:");
    tft.println(Chip4_last_res);
    tft.setCursor (20,150);
    tft.print("CHIP 5:");
    tft.println(Chip5_last_res);
    tft.setCursor (20,170);
    tft.print("CHIP 6:");
    tft.println(Chip6_last_res);
    tft.setCursor (20,190);
    tft.print("CHIP 7:");
    tft.println(Chip7_last_res);
    btn2_flag=0;
  }

  if (btn3_flag){
    //beep buzzer
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
    delay(1000);
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
    delay(1000);
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
    delay(1000);

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(80, 100);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.println("Load test chips");
    tft.setCursor(159, 120);
    tft.println("&");
    tft.setCursor(50, 140);
    tft.println("press start to begin");
    //make the  chips accessible again
    uncover_chips();
    //ensure all temperature control is stopped
    analogWrite(heater,0);
    analogWrite(fan0,255);

    //reset flag
    btn3_flag=0;

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
  servo1.write(servo_angle);
}

void uncover_chips(){
  servo1.write(0);
}

void read_chip_ID(int chip_number){// turn on Chip green LED
  switch (chip_number) {
    case 0:
    uint8_t ch0_ID[8];
     ch0.Init();
     ch0.GetID(ch0_ID);
     SerialUSB.print("Ch0: ");
       logfile.print(" ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch0_ID[i];
      SerialUSB.print(ch0_ID[i]);
      logfile.print(ch0_ID[i]);
      SerialUSB.print(" ");
      logfile.print(" ");
     }
     SerialUSB.println();
     logfile.println();
      break;

    case 1:
     uint8_t ch1_ID[8];
     ch1.Init();
     ch1.GetID(ch1_ID);
     SerialUSB.print("Ch1: ");
     logfile.print("Ch1:  ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch1_ID[i];
      SerialUSB.print(ch1_ID[i]);
      logfile.print(ch1_ID[i]);
      SerialUSB.print(" ");
        logfile.print(" ");
     }
     SerialUSB.println();
     logfile.println();
      break;

    case 2:
       uint8_t ch2_ID[8];
       ch2.Init();
       ch2.GetID(ch2_ID);
       SerialUSB.print("Ch2: ");
       logfile.print("Ch2:  ");
       for (int i=0; i<8; i++){
        chip_is_there+=ch2_ID[i];
        SerialUSB.print(ch2_ID[i]);
        logfile.print(ch2_ID[i]);
        SerialUSB.print(" ");
        logfile.print(" ");
       }
       SerialUSB.println();
       logfile.println();
       break;

    case 3:
     uint8_t ch3_ID[8];
     ch3.Init();
     ch3.GetID(ch3_ID);
     SerialUSB.print("Ch3: ");
     logfile.print("Ch3:  ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch3_ID[i];
      SerialUSB.print(ch3_ID[i]);
      logfile.print(ch3_ID[i]);
      SerialUSB.print(" ");
      logfile.print(" ");
     }
     SerialUSB.println();
     logfile.println();
      break;

    case 4:
     uint8_t ch4_ID[8];
     ch4.Init();
     ch4.GetID(ch4_ID);
     SerialUSB.print("Ch4: ");
     logfile.print("Ch4:  ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch4_ID[i];
      SerialUSB.print(ch4_ID[i]);
      logfile.print(ch4_ID[i]);
      SerialUSB.print(" ");
      logfile.print(" ");
     }
     SerialUSB.println();
     logfile.println();
     break;

    case 5:
     uint8_t ch5_ID[8];
     ch5.Init();
     ch5.GetID(ch5_ID);
     SerialUSB.print("Ch5: ");
     logfile.print("Ch5:  ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch5_ID[i];
      SerialUSB.print(ch5_ID[i]);
      logfile.print(ch5_ID[i]);
      SerialUSB.print(" ");
      logfile.print(" ");
     }
     SerialUSB.println();
     logfile.println();
      break;

    case 6:
       uint8_t ch6_ID[8];
     ch6.Init();
     ch6.GetID(ch6_ID);
     SerialUSB.print("Ch6: ");
     logfile.print("Ch6:  ");
     for (int i=0; i<8; i++){
      chip_is_there+=ch6_ID[i];
      SerialUSB.print(ch6_ID[i]);
      logfile.print(ch6_ID[i]);
      SerialUSB.print(" ");
      logfile.print(" ");
     }
     SerialUSB.println();
     logfile.println();
      break;

    case 7:
      uint8_t ch7_ID[8];
     ch7.Init();
     ch7.GetID(ch7_ID);
     SerialUSB.print("Ch7: ");
     logfile.print("Ch7:  ");

     for (int i=0; i<8; i++){
      chip_is_there+=ch7_ID[i];
      SerialUSB.print(ch7_ID[i]);
      logfile.print(ch7_ID[i]);
      SerialUSB.print(" ");
      logfile.print(" ");
     }
     SerialUSB.println();
     logfile.println();
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

  float Vout;


  switch (chip_number) {
    case 0:
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

    //convert to Resistance:
    // Vin=3.3; //in volts
    //R1= 10000; // known resistor in Ohms
    //R2= (Vout*R1)/(Vin-Vout)
    float res ;
    res=3.3-Vout;
    res=1/res;
    res=res*Vout*10000;

  return res;
}

void print2digits(int number) {
  if (number < 10) {
    SerialUSB.print("0"); // print a 0 before if the number is < than 10
    logfile.print("0");
  }
  SerialUSB.print(number);
  logfile.print(number);
}

void read_time(){
// print time
 print2digits(rtc.getHours());
 SerialUSB.print(":");
 logfile.print(":");
 print2digits(rtc.getMinutes());
 SerialUSB.print(":");
 logfile.print(":");
 print2digits(rtc.getSeconds());
}

void read_date(){
  // Print date...
 print2digits(rtc.getDay());
 SerialUSB.print("/");
 logfile.print("/");
 print2digits(rtc.getMonth());
 SerialUSB.print("/");
 logfile.print("/");
 print2digits(rtc.getYear());

 SerialUSB.println();
 logfile.println();

}

String sd_saver(File dir){
  while(true){
    // iterate all files to ensure no overwrites
    File entry = dir.openNextFile();
    if (!entry){
      break;
    }
    // naming routine
    String entry_name = entry.name();
    if ((entry_name.substring(4,7)).toInt()>=prev_file_indx){
      prev_file_indx = (entry_name.substring(4,7)).toInt()+1;
      if (prev_file_indx>=100){
        fileName = String(prev_file_indx);
      } else if (prev_file_indx>=10){
        fileName = "0"+String(prev_file_indx);
      } else{
        fileName = "00"+String(prev_file_indx);
      }
    }
    entry.close();
  }
  return fileName;
}

int count_chips(){//count the number of chips plugged in
 int chip_count=0;
 uint8_t data[8];
    ch0.Init();
    ch0.GetID(data);
    int buf=0;
    for(int i=0;i<8; i++){
      if (data[i]!=0){
        buf++;
      }
      data[i]=0;
    }
    if(buf!=0){
      chip_count++;
    }
    ch1.Init();
    ch1.GetID(data);
    buf=0;
    for(int i=0;i<8; i++){
      if (data[i]!=0){
        buf++;
      }
      data[i]=0;
    }
    if(buf!=0){
      chip_count++;
    }
    ch2.Init();
    ch2.GetID(data);
    buf=0;
    for(int i=0;i<8; i++){
      if (data[i]!=0){
        buf++;
      }
      data[i]=0;
    }
    if(buf!=0){
      chip_count++;
    }
    ch3.Init();
    ch3.GetID(data);
    buf=0;
    for(int i=0;i<8; i++){
      if (data[i]!=0){
        buf++;
      }
      data[i]=0;
    }
    if(buf!=0){
      chip_count++;
    }
    ch4.Init();
    ch4.GetID(data);
    buf=0;
    for(int i=0;i<8; i++){
      if (data[i]!=0){
        buf++;
      }
      data[i]=0;
    }
    if(buf!=0){
      chip_count++;
    }
    ch5.Init();
    ch5.GetID(data);
    buf=0;
    for(int i=0;i<8; i++){
      if (data[i]!=0){
        buf++;
      }
      data[i]=0;
    }
    if(buf!=0){
      chip_count++;
    }
    ch6.Init();
    ch6.GetID(data);
    buf=0;
    for(int i=0;i<8; i++){
      if (data[i]!=0){
        buf++;
      }
      data[i]=0;
    }
    if(buf!=0){
      chip_count++;
    }
    ch7.Init();
    ch7.GetID(data);
    buf=0;
    for(int i=0;i<8; i++){
      if (data[i]!=0){
        buf++;
      }
      data[i]=0;
    }
    if(buf!=0){
      chip_count++;
   }

   return chip_count;
}

float avg_temp(){
  //calculate average temperature
  float avg_temp;
  for (int i=0; i<8; i++){
      avg_temp += read_chip_temp(i);
    }
   int chip_n=count_chips();
   //ghetto correction for non-plugged in chips
   avg_temp= avg_temp-((8-chip_n)*1.14);
  avg_temp=avg_temp/chip_n;
  return avg_temp;
}

void print_time_elapsed(){ // could be written better in a loop but not a priority now
  long time_elapsed= millis()-start_time;
  time_elapsed= time_elapsed/1000;//time elapsed in seconds
  time_elapsed=time_elapsed/60; //time elapsed in minutes

  if (time_elapsed<60){//if it's under 1hour
    tft.print("00:");
    if (time_elapsed<10){
      tft.print(0);
    }
      tft.print(time_elapsed);
  } else{
    if (time_elapsed<120){//over 1hour but under 2hours
      tft.print("01:");
      if (time_elapsed-60<10){
        tft.print(0);
      }
      tft.print(time_elapsed-60);
    }else{//over two hours.
      tft.print("01:");
      if (time_elapsed-120<10){
        tft.print(0);
      }
      tft.print(time_elapsed-120);
    }
  }
}

void refresh_test_type(){
  int x_max=tft.width();
  tft.fillRect(0, 50, x_max,20, ILI9341_BLACK);
  tft.setCursor (120, 50);
  tft.setTextColor(ILI9341_WHITE);
}

void refresh_temp(){
  int x_max=tft.width();
  tft.fillRect(0, 105, x_max/2, 20, ILI9341_BLACK);
  tft.setCursor (40, 105);
  tft.setTextColor(ILI9341_WHITE);
}

void refresh_time(){
  int x_max=tft.width();
  tft.fillRect(215, 105, x_max/2, 20, ILI9341_BLACK);
  tft.setCursor (215, 105);
  tft.setTextColor(ILI9341_WHITE);
}

void compute_temp_PID(){
   /*How long since we last calculated*/
   unsigned long now = millis();
   float timeChange = (float)(now - lastTime);

   /*Compute all the working error variables*/
   float error = Setpoint - Input;
   errSum += (error * timeChange);
   float dErr = (error - lastErr) / timeChange;

   /*Compute PID Output*/
   Output = kp * error + ki * errSum + kd * dErr;

   /*set boundaries*/
   if (Output<-255.0){
    Output= -255.0;
   }
    if (Output>255.0){
    Output= 255.0;
   }

    /*decide whether to drive fan or heater*/
   if (Output<0){
    //give the 255 complement of the output to the fan
    Output_fan=255+Output;
    Output_heat=0;
   } else{
    Output_heat=Output;
    Output_fan=255;
   }

   /*Remember some variables for next time*/
   lastErr = error;
   lastTime = now;
}

void plot_temp(){
  //convert temperature to coordinates
  float y=0;
  y=avg_temp();
  y=y-25; // plot lower limit is 25
  y=y*70;
  y=y/15;
  //tft display limits: ylow=220, yhigh=150;
  y=220-y;
  tft.drawPixel(x_temp+20, y, ILI9341_GREEN);
  tft.fillRect(tft.width()-20,150,tft.width(),70, ILI9341_BLACK);
}
