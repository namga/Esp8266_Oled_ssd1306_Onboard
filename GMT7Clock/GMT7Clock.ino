// We need to include Wire.h for I2C communication
#include <Wire.h>
#include "OLED.h"
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char *ssid     = "your_network";
const char *password = "your_password";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

// Declare OLED display follow format: display(SDA, SCL);
// SDA and SCL are the GPIO pins of ESP8266 that are connected to respective pins of display.
// Display module on board using SDA is GPIO14, and SCL is GPIO12
OLED display(14, 12);
bool wifi_connect {false};
uint32_t retryTimes = 0;

void setup()
{
  // Initialize serial
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Initialize display
  display.begin();
  display.print("The internet is");
  display.print("connecting ...", 3);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (retryTimes > 10)
    {
      demo_info_print();
      delay(5000);
      return;
    }
    Serial.print("Waitting connect to the internet ... \n");
    delay(1000);
    retryTimes++;
  }

  wifi_connect = true;
  display.clear();

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +7 = 25200
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(25200);
  print_time_to_serial();
}

void demo_info_print()
{
  display.clear();
  display.print("Esp8266 ssd1306");
  display.print("This is demo app", 2);
  display.print("Need to update", 4);
  display.print("new source code", 5);

  display.print("Thanks you!", 7);
}

void print_time_to_serial()
{
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  

  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);
}

void loop()
{
  if (wifi_connect)
  {
    display.print("Esp8266 ssd1306");

    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime); 

    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon+1;
    int currentYear = ptm->tm_year+1900;

    String formattedTime = timeClient.getFormattedTime();
    char* ch_formattedTime = &formattedTime[0];
    display.print("Times:", 2);
    display.print(ch_formattedTime, 2, 7); 

    String weekDay = weekDays[timeClient.getDay()];
    String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);

    char* ch_weekDay = &weekDay[0];
    char* ch_date = &currentDate[0];
    display.print(ch_date, 4);
    display.print("Day:", 6);
    display.print(ch_weekDay, 6, 5);
  }
  else
  {
    display.clear();
    display.print("No internet ...");
    display.print("Please reboot!", 2);
    delay(7000);
  }

  delay(1000);
}
