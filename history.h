#include <Arduino.h>
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <RTClib.h>

void updateAverage(DateTime now, int newVal, Adafruit_ST7735 tft);
void shiftHistory();
void printHistory();
void drawHistory(Adafruit_ST7735 tft);