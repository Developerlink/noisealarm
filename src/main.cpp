#include <iostream>
#include <Arduino.h>
#include <M5Stack.h>
#include <ctime>
#include <string>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ArduinoJson.h>
#include "time.h"
#include "../lib/alarm_lights.h"
#include "../lib/infoScreen.h"
#include "../lib/timehelper.h"
//#include "../lib/connection.h"

#define M5STACKFIRE_MICROPHONE_PIN 34

// CONNECTION
// Set the name and password of the wifi to be connected.
const char* ssid     = "Claus";
const char* password = "Claus1993";

const char* ntpServer = "1.dk.pool.ntp.org";  // Set the connect NTP server. 
const long gmtOffset_sec     = 0;
const int daylightOffset_sec = 3600;

byte mac[] = { 0XA8, 0X61, 0X0A, 0XAE, 0XA8, 0X56 };


void connectToWifi() {
    M5.Lcd.printf("\nConnecting to %s", ssid);                            
    WiFi.begin(ssid, password);  // Connect wifi and return connection status.

    // wifiMulti.addAP(ssid, password);

    // if ((wifiMulti.run() != WL_CONNECTED)) {
    //     delay(500);         // delay 0.5s. 
    //     M5.Lcd.print(".");
    //     Serial.print(".");
    // }

    while (WiFi.status() !=
           WL_CONNECTED) {  // If the wifi connection fails. 
        delay(500);         // delay 0.5s. 
        M5.Lcd.print(".");
        Serial.print(".");
    }

    M5.Lcd.println("\nCONNECTED!");
    std::string str(ssid);
    Serial.println("\nCONNECTED");
    configTime(gmtOffset_sec, daylightOffset_sec,
               ntpServer);  // init and get the time.  
    // WiFi.disconnect(true);  // Disconnect wifi. 
    // WiFi.mode(WIFI_OFF);  // Set the wifi mode to off.  
    
    delay(3000);
}

    // Configuration variables
    int deviceId = 1;
    int measuringUnit_id = 3;
    std::string deviceName = "sound-monitor-01";
    bool monitoringIsOn = true;
    bool alarmIsOn = false;
    int startHour = 14;
    int startMinute = 52;
    int endHour = 14;
    int endMinute = 55;
    int soundMax = 3000;

    void showInfo() {
    infoScreen(deviceName, monitoringIsOn, alarmIsOn, startHour, startMinute, endHour, endMinute);
    }

//API set up
const char* server = "192.168.1.105";
int portNumber = 80;
const char* resource = "/alarm/create.php";

EthernetClient client; 

bool skipResponseHeaders() {
  //HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";
  bool ok = client.find(endOfHeaders);

  if(!ok) {
    Serial.println("No response or invalid response!");
  }
  return ok;
}

void disconnect() {
  Serial.println("Disconnect");
  client.stop();
}

bool connect(const char* hostName, int portNumber) {
  Serial.print("Connect to ");
  Serial.println(hostName);
  bool ok = client.connect(hostName, portNumber); 

  Serial.println(ok ? "Connected" : "Connection Failed!");
  return ok;
}

bool sendRequest(const char* host, const char* resource, DynamicJsonDocument jsonBuffer) {
    Serial.println("sendRequest");
    //POST request
    Serial.println("Begin POST Request");
    client.println("POST /alarm/create.php HTTP/2.0");
    Serial.println("POST /alarm/create.php HTTP/2.0"); 

    client.println("Host: 192.168.1.105");
    Serial.println(server);

    client.println("User-Agent: Arduino/1.0");
    Serial.println("User-Agent: Arduino/1.0"); 

    client.println("Content-Type: application/json");
    Serial.println("Content-Type: application/json"); 

    client.println("Connection: keep-alive");
    Serial.println("Connection: keep-alive");

    client.print("Content-Length: ");
    Serial.print("Content-Length: "); 

    client.println(measureJson(jsonBuffer));
    Serial.println(measureJson(jsonBuffer)); 

    client.println();
    Serial.print(F("Sending: "));
    serializeJson(jsonBuffer, Serial);
    Serial.println();

    serializeJson(jsonBuffer, client);
    return true;
} 

void sendDataToAPI(DynamicJsonDocument jsonBuffer)
{
    Serial.println("SendDataToAPI");
    if(connect(server, portNumber)) {
        if(sendRequest(server, resource, jsonBuffer) && skipResponseHeaders()) {
        Serial.println("HTTP POST request finished.");
        }
    }
    disconnect();
}

void registerAlarmToDatabase(bool monitoringIsOn) {
    //Serial.println("registerAlarmToDatabase");
    //Reserve memory space for your JSON data
    //StaticJsonDocument<200> jsonBuffer;
    //Build your own object tree in memory to store the data you want to send in the request
    const size_t capacity = JSON_OBJECT_SIZE(5);
    DynamicJsonDocument jsonBuffer(capacity);

    jsonBuffer["date_time"] = "000000";
    jsonBuffer["isOn"] = "1";
    jsonBuffer["device_id"] = deviceId;
    jsonBuffer["value"] = "NULL"; 
    jsonBuffer["measuringUnit_id"] = measuringUnit_id;

    sendDataToAPI(jsonBuffer);
}

//7821848a25d8


void setup() {
    M5.begin();             // Init M5Core.  
    M5.Power.begin();       // Init power  
    M5.Lcd.setTextSize(2);  // Set the font size to 2.  
    Serial.begin(9600);

    //connectToWifi();

    Serial.print(F("Starting ethernet..."));

      if(!Ethernet.begin(mac)) Serial.println(F("failed"));
        else Serial.println(Ethernet.localIP());

    // Serial.print(F("Starting ethernet..."));
    // if(!Ethernet.begin(mac)) Serial.println(F("failed"));
    // else Serial.println(Ethernet.localIP());

    
    showInfo();  
}
 
void loop() {
    delay(10); 
    M5.update();  // Read the press state of the key
    int micValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);   

    // check time if alarm should be on
    if (IsTimeMonitoringStarts(startHour, startMinute)) {    
        monitoringIsOn = true;  
        showInfo();  
    } 

    // check time if alarm should be off
    if (IsTimeMonitoringEnds(endHour, endMinute)) {        
        monitoringIsOn = false;
        showInfo();
    }

    if (monitoringIsOn && micValue > soundMax) {
        alarmIsOn = true;
        // registerAlarmToDatabase(monitoringIsOn);
        showInfo();
    } 

    // show alarm condition          
    if (alarmIsOn == true){     
        showAlarm();
    } else {
        turnOffAlarm();
    }

    // button actions
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
        monitoringIsOn = !monitoringIsOn;
        showInfo();
    } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
        M5.Lcd.clear(); 
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setCursor(1, 105);                              
        M5.Lcd.print("Fetching latest configuration from database");
        delay(5000);
        showInfo();
    } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {        
        alarmIsOn = !alarmIsOn;

        showInfo();
    } else if (M5.BtnB.wasReleasefor(700)) {
        M5.Lcd.clear();
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setCursor(1, 105);                              
        M5.Lcd.print("Registering device in database");
        delay(5000);
        showInfo();        
    }  
}