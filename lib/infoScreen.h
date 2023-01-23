#include <iostream>
#include <M5Stack.h>
#include <ctime>
#include <string>

#ifndef INFOSCREEN
#define INFOSCREEN

std::string BoolToString(bool b)
{
  return b ? "true" : "false";
}

std::string FormattedTime(int hour, int minute) {
    std::string h = hour < 10 ? "0" + std::to_string(hour) : std::to_string(hour);
    std::string m = minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute);
    return h + ":" + m;
}

/**
 * @brief   Outputs current configuration to the screen
 * @param   deviceName string
 * @param   monitoringIsOn bool
 * @param   alarmIsOn bool
 * @param   startHour int
 * @param   startMinute int
 * @param   endHour int
 * @param   endMinute int
 * @retval  true
 *          false 
*/
void infoScreen(std::string deviceName, bool monitoringIsOn, bool alarmIsOn, int startHour, int startMinute, int endHour, int endMinute) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(WHITE);  
    M5.Lcd.setTextSize(1);     
    M5.Lcd.setCursor(135, 10);                              
    M5.Lcd.println("HOME");

    // Current config info  
    M5.Lcd.println("Device name: ");
    M5.Lcd.println(deviceName.c_str());
    M5.Lcd.println("");

    M5.Lcd.println("Monitoring is on: ");
    M5.Lcd.println(BoolToString(monitoringIsOn).c_str());
    M5.Lcd.println("");

    M5.Lcd.println("Alarm is on: ");
    M5.Lcd.println(BoolToString(alarmIsOn).c_str());
    M5.Lcd.println("");

    M5.Lcd.println("Alarm monitoring starts");
    std::string alarmMonitoringStartTime = FormattedTime(startHour, startMinute);
    M5.Lcd.println(alarmMonitoringStartTime.c_str());    
    M5.Lcd.println("");

    M5.Lcd.println("Alarm monitoring ends");
    std::string alarmMonitoringEndTime = FormattedTime(endHour, endMinute);
    M5.Lcd.println(alarmMonitoringEndTime.c_str());
    M5.Lcd.println("");

    // Button info
    M5.Lcd.println("");
    M5.Lcd.println("");
    M5.Lcd.println("");
    M5.Lcd.setTextColor(BLUE);  
    M5.Lcd.println("BUTTON A: Turn monitoring on/off");
    M5.Lcd.println("");

    M5.Lcd.setTextColor(YELLOW);  
    M5.Lcd.println("BUTTON B: Read config from database");
    M5.Lcd.println("");

    M5.Lcd.setTextColor(RED);  
    M5.Lcd.println("BUTTON C: Turn alarm on/off");
    M5.Lcd.println("");

    M5.Lcd.setCursor(66, 220);    
    M5.Lcd.setTextColor(BLUE);  
    M5.Lcd.setTextSize(2);   
    M5.Lcd.print("A");
    
    M5.Lcd.setTextColor(YELLOW);  
    M5.Lcd.setCursor(153, 220);
    M5.Lcd.print("B");
    
    M5.Lcd.setTextColor(RED);  
    M5.Lcd.setCursor(245, 220);
    M5.Lcd.print("C");
}

#endif