#include <Arduino.h>
#include <PxMatrix.h>
#include <WiFi.h>
#include <time.h>

#define P_LAT 26
#define P_A 19
#define P_B 23
#define P_OE 16

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

PxMATRIX display(32, 16, P_LAT, P_OE, P_A, P_B);

// Colors
uint16_t myCyan = display.color565(0, 255, 255);
uint16_t myMagenta = display.color565(255, 0, 255);
uint16_t myGreen = display.color565(0, 255, 0);
uint16_t myRed = display.color565(255, 0, 0);
uint16_t myBlue = display.color565(0, 0, 255);

// WiFi credentials
const char *ssid = "Das_House_lite";
const char *password = "2444666668888888";
// NTP settings
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 6 * 3600; // Bangladesh UTC+6
const int daylightOffset_sec = 0;

void IRAM_ATTR display_updater()
{
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(60);
  portEXIT_CRITICAL_ISR(&timerMux);
}

void display_update_enable(bool is_enable)
{
  if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 4000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  display.begin(4);
  display.setScanPattern(ZAGZIG);
  display.clearDisplay();
  display_update_enable(true);
}

void loop()
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
  strftime(hourStr, sizeof(hourStr), "%I", &timeinfo);
  strftime(minStr, sizeof(minStr), "%M", &timeinfo);
  snprintf(secStr, sizeof(secStr), "%02d", currentSec);

  int prevSec = (lastSec == -1) ? currentSec : lastSec;
  snprintf(prevSecStr, sizeof(prevSecStr), "%02d", prevSec);

  // display.clearDisplay();
  // display.setTextSize(2);
  // Horizontal line at y=8, from x=0 to x=31

  // Double digit scroll at 10,20,...,00
  if (currentSec % 10 == 1)
  {
    for (int y = 8; y >= 4; y--)
    {
      
      display.clearDisplay();
      display.drawLine(16, 0, 16, 15, myGreen);
      display.setTextColor(myRed);
      display.setCursor(4, 0);
      display.print(hourStr);
      display.setTextColor(myRed);
      display.setCursor(4, 9);
      display.print(minStr);

      display.setTextColor(myGreen);
      display.setCursor(18, y);
      display.print(prevSecStr);
      display.setCursor(18, y + 12);
      display.print(secStr);

      delay(20);
    }
  }
  else // Only right digit scroll
  {
    char leftDigit[2] = {secStr[0], '\0'};
    char rightDigit[2] = {secStr[1], '\0'};
    char prevRightDigit[2] = {prevSecStr[1], '\0'};

    // If right digit is 0, keep previous left digit during scroll
    if (secStr[1] == '0')
      leftDigit[0] = prevSecStr[0];

    for (int y = 8; y >= 4; y--)
    {
      display.clearDisplay();
      display.drawLine(16, 0, 16, 15, myGreen);
      display.setTextColor(myRed);
      display.setCursor(4, 0);
      display.print(hourStr);
      display.setTextColor(myRed);
      display.setCursor(4, 9);
      display.print(minStr);

      display.setTextColor(myGreen);
      display.setCursor(18, 4);
      display.print(leftDigit);

      display.setCursor(24, y);
      display.print(prevRightDigit);
      display.setCursor(24, y + 12);
      display.print(rightDigit);

      delay(20);
    }
  }

  lastSec = currentSec;
}