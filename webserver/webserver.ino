//Libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Ticker.h>
#include <Servo.h>

//External files
#include "config.h"
#include "index.h"

//Server variables
ESP8266WebServer server(8080);

//Time server
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"europe.pool.ntp.org",3600);

//Servo
Servo myServo;

//NodeMCU variables
#define LED 2

//Global variables
int timeHours = 0;
int timeMinutes = 0;
int alarmHour = -1;
int alarmMinute = -1;
double currentTime = 0;
double previousTime = 0;
bool done = false;

/*
 * Setup
 */
void setup(void)
{
  //Serial
  Serial.begin(115200);

  //Init variables
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  //Servo
  myServo.attach(2);
  myServo.write(0);

  //Connect to WiFi
  ConnectToWiFi();

  //Connect to NPT server
  timeClient.begin();
}

/*
 * Loop
 */
void loop(void)
{
  //Handle client requests
  server.handleClient();

  //Update current time
  currentTime = millis()/1000;
  if (currentTime != previousTime)
  {
    getTime();
  }
  previousTime = currentTime;

  //Turn on LED when the alarm goes off
  if (alarmHour == timeHours && alarmMinute == timeMinutes && !done)
  {
    digitalWrite(LED,LOW);
    moveServo();
  }
  else
  {
    digitalWrite(LED,HIGH);
  }
}

/*
 * Connect to WiFi
 */
void ConnectToWiFi()
{
  //WiFi mode
  WiFi.mode(WIFI_STA);
  
  //Connect to assigned WiFi
  WiFi.begin(ssid, password);

  //Wait while connecting to network
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("beep ");
    digitalWrite(LED,LOW);
    delay(500);

    Serial.print("boop ");
    digitalWrite(LED,HIGH);
    delay(500); 
  }
  
  //Print local IP address and start web server
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Actions depending on the header
  headerActions();

  //Begin server
  server.begin();
}

/*
 * Get time from NTP Server
 */
void getTime()
{
  timeClient.update();
  timeHours = timeClient.getHours();
  timeMinutes = timeClient.getMinutes();
  Serial.print(timeHours);
  Serial.print(":");
  Serial.print(timeMinutes);
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
}

/*
 * Actions depending on the header
 */
void headerActions()
{
  server.on("/", mainPage);
  server.on("/set", setAlarm);
  server.on("/test", testingFunction);
  server.onNotFound(errorPage);
}

/*
 * Display webpage depending on the header
 */
void mainPage() 
{
  server.send(200, "text/html", MAIN_PAGE);
}
void errorPage()
{
  server.send(200, "text/html", ERROR_PAGE);
}
void setAlarm()
{
  alarmHour = server.arg("hour").toInt();
  alarmMinute = server.arg("minute").toInt();

  Serial.print("Alarma configurada para las ");
  Serial.print(alarmHour);
  Serial.print(" horas y ");
  Serial.print(alarmMinute);
  Serial.println(" minutos.");
  
  server.send(200, "text/html", SET_PAGE);
}
void testingFunction()
{
  server.send(200, "text/html", ERROR_PAGE);
}

/*
 * Move Servo
 */
void moveServo()
{
  int pos = 0;

  for (pos = 0; pos <= 180; pos += 1)
  { 
    myServo.write(pos);              
    delay(5);                      
  }
  for (pos = 180; pos >= 0; pos -= 1)
  { 
    myServo.write(pos);
    delay(5);
  }
  done = true;
}
