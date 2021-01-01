#include "history.h"

const int n = 64;
int history_data[n] = {};
float current_average= 0.0;
int measurements_count = 0;
uint32_t shiftInstant = 0;

// ====== Drawing parameters ======
const int BASE = 78;
const int SHIFT = 62;
const int min = 140;
const int max = 165;
const int step = 10;
// ================================

void updateAverage(DateTime now, int newVal, Adafruit_ST7735 tft) {

  uint32_t secondsNow = now.secondstime();
  uint32_t modSeconds = secondsNow % (3600 * 6);
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

int measurementToPosition(int measurement) {
  int position = (BASE - measurement + SHIFT) * 2 + 127;
  // Serial.println(position);
  return position;
}

int positionToMeasurement(int p) {
  int m = ((p-127)/2-BASE-SHIFT) * -1;
  // Serial.println(m);
  return m;
}

void drawHistory(Adafruit_ST7735 tft) {
  tft.fillRect(0, BASE, 160, 127 - BASE, ST7735_BLACK);
  drawChart(tft);
  for (int i=n; i>=0; i--) {
    int position = measurementToPosition(history_data[i]);
    if (position>=BASE) {
      tft.fillRect(1+i*2, position, 2, 2, ST7735_GREEN);
    }
  }
}


#define COLOR_GRAY	0x83EF
void drawChart(Adafruit_ST7735 tft) {
  // Y-axis
  for(int i=measurementToPosition(max); i<=measurementToPosition(min); i+=step) {
    if (positionToMeasurement(i) % 10 == 0) {
      tft.drawFastHLine(0, i, 134, ST7735_WHITE);
      tft.setCursor(135, i);
      tft.print(positionToMeasurement(i));
      Serial.print("a");
    } else {
      Serial.print("b");
      tft.drawFastHLine(0, i, 131, COLOR_GRAY);
    }
  }

  // X-axis  
  for(int i=128; i>=0; i-=8) {
    tft.drawFastVLine(i, BASE, 127 - BASE, COLOR_GRAY);
  }
}