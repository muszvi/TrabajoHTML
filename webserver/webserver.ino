//Libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Ticker.h>

//External files
#include "config.h"
#include "index.h"

//Server variables
ESP8266WebServer server(8080);

//Time server
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);

//Interrupctions
Ticker realTime;

//NodeMCU variables
#define LED 2

//Global variables
const int timeRefreshRate = 10;
int timeHours = 0;
int timeMinutes = 0;
const int alarmHour = 13;
const int alarmMinute = 12;

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

  //Connect to WiFi
  ConnectToWiFi();

  //Connect to NPT server
  timeClient.begin();

  delay(5000);

  //Interrupction setup
  realTime.attach(timeRefreshRate,getTime);
}

/*
 * Loop
 */
void loop(void)
{
  //Handle client requests
  server.handleClient();

  if (alarmHour == timeHours && alarmMinute == timeMinutes)
  {
    digitalWrite(LED,LOW);
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
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
}

/*
 * Actions depending on the header
 */
void headerActions()
{
  server.on("/", mainPage);
  server.on("/on", turnONLED);
  server.on("/off", turnOFFLED);
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
void turnONLED()
{
  server.send(200, "text/html", MAIN_PAGE);
  digitalWrite(LED, LOW);
}
void turnOFFLED()
{
  server.send(200, "text/html", MAIN_PAGE);
  digitalWrite(LED, HIGH);
}
void testingFunction()
{
  server.send(200, "text/html", TEST_PAGE);
}
