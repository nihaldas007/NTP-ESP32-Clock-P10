#pragma once

#include <PxMatrix.h>
#include <time.h>
#include "fonts/Org_01.h"
#include "fonts/Digital5x78pt7b.h"

// Externally defined in main.cpp
extern PxMATRIX display;
extern uint16_t myGreen, myCyan;
inline void display_text(int x, int y, const char *a, uint16_t color)
{
  display.setTextColor(color);
  display.setCursor(x, y);
  display.print(a);
}
inline void showClock()
{
  static int lastSec = -1;
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

  int currentSec = timeinfo.tm_sec;
  if (currentSec == lastSec)
    return;

  // Format time
  char hourStr[3], minStr[3], secStr[3], prevSecStr[3];
  char weekStr[4];
  strftime(hourStr, sizeof(hourStr), "%I", &timeinfo);
  strftime(minStr, sizeof(minStr), "%M", &timeinfo);
  snprintf(secStr, sizeof(secStr), "%02d", currentSec);

  int prevSec = (lastSec == -1) ? currentSec : lastSec;
  snprintf(prevSecStr, sizeof(prevSecStr), "%02d", prevSec);
  strftime(weekStr, sizeof(weekStr), "%a", &timeinfo);
  for (int i = 0; weekStr[i]; i++) {
    weekStr[i] = toupper(weekStr[i]);
  }

  if (currentSec % 10 == 1)
  {
    for (int y = 17; y >= 13; y--)
    {
      display.clearDisplay();
      display.setFont(&Digital5x78pt7b);
      display.setTextColor(myGreen);
      display.setCursor(1, 6);
      display.print(hourStr);
      display.setCursor(1, 15);
      display.print(minStr);
      display.drawLine(13, 0, 13, 15, myCyan);
      display.setFont(&Org_01);
      display.setTextColor(myCyan);
      display.setCursor(15, 5);
      display.print(weekStr);
      display.setFont(&Digital5x78pt7b);
      display.setTextColor(myCyan);
      display.setCursor(18, y);
      display.print(prevSecStr);
      display.setCursor(18, y + 19);
      display.print(secStr);

      delay(20);
    }
  }
  else
  {
    char leftDigit[2] = {secStr[0], '\0'};
    char rightDigit[2] = {secStr[1], '\0'};
    char prevRightDigit[2] = {prevSecStr[1], '\0'};

    if (secStr[1] == '0')
      leftDigit[0] = prevSecStr[0];

    for (int y = 17; y >= 13; y--)
    {
      display.clearDisplay();
      display.setFont(&Digital5x78pt7b);
      display.setTextColor(myGreen);
      display.setCursor(1, 6);
      display.print(hourStr);
      display.setCursor(1, 15);
      display.print(minStr);
      display.drawLine(13, 0, 13, 15, myCyan);
      display.setFont(&Org_01);
      display.setTextColor(myCyan);
      display.setCursor(15, 5);
      display.print(weekStr);
      display.setFont(&Digital5x78pt7b);
      display.setTextColor(myCyan);
      display.setCursor(18, 13);
      display.print(leftDigit);
      display.setCursor(24, y);
      display.print(prevRightDigit);
      display.setCursor(24, y + 21);
      display.print(rightDigit);

      delay(20);
    }
  }

  lastSec = currentSec;
}