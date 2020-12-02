#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//External files
#include "config.h"
#include "index.h"

//Server variables
ESP8266WebServer server(8080);

//NodeMCU variables
#define LED 2

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
}

/*
 * Loop
 */
void loop(void)
{
  server.handleClient();
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
 * Actions depending on the header
 */
void headerActions()
{
  server.on("/", mainPage);
  server.on("/on", turnONLED);
  server.on("/off", turnOFFLED);
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
