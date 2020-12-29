#include "history.h"

const int n = 60;
int history_data[n] = {};
float current_average = 0.0;
int measurements_count = 0;
uint32_t shiftInstant = 0;

// ====== Drawing parameters ======
const int BASE = 100;
// const int min = 140;
// const int max = 160;
// ================================

void updateAverage(DateTime now, int newVal, Adafruit_ST7735 tft) {

  uint32_t secondsNow = now.secondstime();
  uint32_t modSeconds = secondsNow % 60;
  uint32_t newInstant = secondsNow - modSeconds;
 
  if (shiftInstant == 0 || shiftInstant != newInstant) {
    shiftInstant = newInstant;    
    printHistory();        
    shiftHistory();
    drawHistory(tft);
  }
  measurements_count++;
  current_average = current_average + ((newVal - current_average)/measurements_count);
}

void shiftHistory() {
  for(int i=0; i<n-1; i++) {
    // Serial.println(i);
    history_data[i] = history_data[i+1];
  }
  Serial.print("avg: ");
  Serial.println(current_average);
  history_data[n-1] = round(current_average); //todo: is this correct rounding?
  current_average = 0.0;
  measurements_count = 0;
}

void printHistory() {
  for(int i=0; i<n; i++) {
    Serial.print(history_data[i]);
    Serial.print(", ");
  }
  Serial.println();
}

void drawHistory(Adafruit_ST7735 tft) {
  tft.fillRect(0, BASE, 160, 128 - BASE, ST7735_BLACK);
  drawChart(tft);
  for(int i=0; i<n; i++) {
    tft.fillRect(1+i*2, BASE + (history_data[i] - 145), 2, 2, ST7735_RED);
    // tft.fillRect(i*2, i*3, 20, 20, ST7735_BLUE);
  }
}


void drawChart(Adafruit_ST7735 tft) {
  for(int i=BASE; i<=128; i+=5) {
    tft.drawFastHLine(0, i, 120, ST7735_WHITE);
  }
  for(int i=20; i<=125; i+=20) {
    tft.drawFastVLine(i, BASE, 128 - BASE, ST7735_WHITE);
  }
}