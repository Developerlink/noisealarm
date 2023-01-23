#include <ctime>
#include <string>

#ifndef TIMEHELPER
#define TIMEHELPER

bool IsTimeMonitoringStarts(int startHour, int startMinute) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int hour = ltm->tm_hour + 1;
    int minute = ltm->tm_min;

    if (hour == startHour && minute == startMinute) {
        Serial.println(ltm);
        Serial.println("Monitoring is on");
        return true;
    } else {
        return false;
    }
}

bool IsTimeMonitoringEnds(int endHour, int endMinute) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int hour = ltm->tm_hour + 1;
    int minute = ltm->tm_min;

    // Serial.println("Hour is");
    // Serial.println(hour);
    // Serial.println("Minute is");
    // Serial.println(minute);

    if (hour == endHour && minute == endMinute) {
        Serial.println(ltm);
        Serial.println("Monitoring is off");
        return true;
    } else {
        return false;
    }
}

#endif