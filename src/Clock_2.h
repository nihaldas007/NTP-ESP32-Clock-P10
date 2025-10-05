#pragma once

#include <PxMatrix.h>
#include <time.h>
#include "fonts/BigNumber.h"

extern PxMATRIX display;
extern uint16_t myGreen, myCyan,myRed;

inline void showClock_2()
{
    static int lastMin = -1;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("No");
        display.print("NTP");
        delay(1000);
        return;
    }

    // Only update if minute changed
    if (timeinfo.tm_min == lastMin) return;
    lastMin = timeinfo.tm_min;

    // Format time
    char hourStr[3], minStr[3];
    strftime(hourStr, sizeof(hourStr), "%I", &timeinfo);
    strftime(minStr, sizeof(minStr), "%M", &timeinfo);

    display.clearDisplay();
    display.setFont(&BigNumber7pt7b);
    display.setTextColor(myGreen);

    // Adjust these values for your font and display
    display.setCursor(0, 11); // Try (0, 14) for 16px height
    display.print(hourStr);
    display.drawLine(16, 0, 16, 15, myRed);
    display.setCursor(18, 11); // Try (16, 14) for minutes
    display.print(minStr);
}