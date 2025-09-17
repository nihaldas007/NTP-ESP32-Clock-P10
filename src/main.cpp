#include <Arduino.h>
#include <PxMatrix.h>
#include <WiFi.h>
#include "Clock_1.h"

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
uint16_t myYellow = display.color565(255, 255, 0);

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
  // display.setBrightness(200);
  display_update_enable(true);
}

void loop()
{
  showClock();

}