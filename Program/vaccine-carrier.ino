#include <SPI.h> //for the SD card module
#include <SD.h> // for the SD card
#include <Wire.h>
#include "RTClib.h" // for RTC
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); //The LCD address and size. 
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2 //pin for sensor
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
//Buzzer
int buzzer = 8; // set the buzzer control digital IO pin
//SD Card
const int chipSelect = 4; 
// Create a file to store the data
File myFile;
//RTC DS3231
RTC_DS3231 rtc;
//SETUP
void setup() {
 // start serial port
 Serial.begin(9600);
 Serial.println("Vaccine Carrier");
 // Start up the library
 sensors.begin();
 lcd.init();
 // Print a message to the LCD.
 lcd.backlight();
 lcd.setCursor(0, 0);
 lcd.print("Vaccine Carrier");
 lcd.setCursor(0, 1);
 lcd.print("Start!");
 delay(2000);
 lcd.clear();
 //Buzzer:
 pinMode(buzzer, OUTPUT); // set pin 8 as output
 //SD card
 Serial.print("Initializing SD card...");
 if(!SD.begin(chipSelect)) {
 Serial.println("initialization failed!");
 return;
 }
 Serial.println("initialization done.");
 myFile=SD.open("DATA.txt", FILE_WRITE); //open file
 // if the file opened ok, write to it:
 if (myFile) {
 Serial.println("File opened ok"); // print the headings for our data
 myFile.println(" Date ; Time ; Temperature ºC");
 }
 myFile.close();
 //RTC
 if (! rtc.begin()) {
 Serial.println("Couldn't find RTC Module");
 while (1);
 }
 if (rtc.lostPower()) 
 {
 Serial.println("RTC lost power, lets set the time!");
 rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 }
 rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
//VOID LOOP
void loop() { 
 DateTime now = rtc.now();
 Serial.print("Requesting temperatures...");
 sensors.requestTemperatures(); // Send the command to get temperatures
 Serial.println("DONE");
 float tempC = sensors.getTempCByIndex(0);
 
 // Read temperature as Celsius
 // Jika Sensor suhu terpasang
 if (tempC != DEVICE_DISCONNECTED_C)
 {
 // Print tanggal waktu dan suhu pada serial
 Serial.print("Temperature for the device 1 (index 0) is: ");
 Serial.print(now.day(), DEC);
 Serial.print("/");
 Serial.print(now.month(), DEC);
 Serial.print("/");
 Serial.print(now.year(), DEC);
 Serial.print(" ; ");
 Serial.print(now.hour(), DEC);
 Serial.print(":");
 Serial.print(now.minute(), DEC);
 Serial.print(":");
 Serial.print(now.second(), DEC);
 Serial.print(" ; ");
 Serial.print(tempC,DEC);
 Serial.println("ºC");
 
 //Print tanggal, waktu, dan suhu pada LCD
 lcd.setCursor(0, 0);
 lcd.print("Date|Temp(");
 lcd.print((char)223);
 lcd.print("C)");
 lcd.setCursor(0, 1);
 lcd.print(now.day());
 lcd.print("/");
 lcd.print(now.month());
 lcd.print("/");
 lcd.print(now.year());
 lcd.print("|");
 lcd.print(tempC);
 //Pengaturan write data pada SD Card
 myFile = SD.open("DATA.txt", FILE_WRITE);
 if (myFile) {
 Serial.println("open with success");
 myFile.print(now.day(), DEC);
 myFile.print('/');
 myFile.print(now.month(), DEC);
 myFile.print('/');
 myFile.print(now.year(), DEC);
 myFile.print(" ; ");
 myFile.print(now.hour(), DEC);
 myFile.print(':');
 myFile.print(now.minute(), DEC);
 myFile.print(':');
 myFile.print(now.second(), DEC);
 myFile.print(" ; ");
 myFile.print(tempC, DEC);
 myFile.println("ºC");
 }
 myFile.close(); 
 //lcd.print(" | ");

 //lcd.print(DallasTemperature::toFahrenheit(tempC));
 //lcd.print(" F");
 }
 else
 {
 Serial.println("Error: Could not read temperature data");
 //SD CARD
 myFile = SD.open("DATA.txt", FILE_WRITE);
 if (myFile) {
 Serial.println("open with success");
 myFile.print(now.day(), DEC);
 myFile.print('/');
 myFile.print(now.month(), DEC);
 myFile.print('/');
 myFile.print(now.year(), DEC);
 myFile.print(" ; ");
 myFile.print(now.hour(), DEC);
 myFile.print(':');
 myFile.print(now.minute(), DEC);
 myFile.print(':');
 myFile.print(now.second(), DEC);
 myFile.print(" ; ");
 myFile.println("Error");
 }
 myFile.close(); 
 }
 //Buzzer
 if (tempC > 7.5 or tempC < 2.5)
 {
 for (int i = 0; i < 80; i++) { // make a sound
 digitalWrite(buzzer, HIGH); // send high signal to buzzer 
 delay(1); // delay 1ms
 digitalWrite(buzzer, LOW); // send low signal to buzzer
 delay(1);}
 } 
 delay(1000);
}


