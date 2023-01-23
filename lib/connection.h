#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "time.h"

#ifndef CONNECTION
#define CONNECTION



WiFiMulti wifiMulti;

void connectToWifi() {
    M5.Lcd.printf("\nConnecting to %s", ssid);                            
    //WiFi.begin(ssid, password);  // Connect wifi and return connection status.

    wifiMulti.addAP(ssid, password);

    if ((wifiMulti.run() != WL_CONNECTED)) {
        delay(500);         // delay 0.5s. 
        M5.Lcd.print(".");
        Serial.print(".");
    }

    // while (WiFi.status() !=
    //        WL_CONNECTED) {  // If the wifi connection fails. 
    //     delay(500);         // delay 0.5s. 
    //     M5.Lcd.print(".");
    //     Serial.print(".");
    // }

    M5.Lcd.println("\nCONNECTED!");
    std::string str(ssid);
    Serial.println("\nCONNECTED");
    configTime(gmtOffset_sec, daylightOffset_sec,
               ntpServer);  // init and get the time.  
    // WiFi.disconnect(true);  // Disconnect wifi. 
    // WiFi.mode(WIFI_OFF);  // Set the wifi mode to off.  
    
    delay(3000);
}

#endif