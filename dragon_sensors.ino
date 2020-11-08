#include <SPI.h>

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Fonts/FreeSerif9pt7b.h>


#include <Adafruit_SSD1306.h>
#include <splash.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#include "DHT.h"
#define DHTPIN 50
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int lightSensor = A0;
#define VIN 5
#define R 10000

int counter = 0;

unsigned long time;

void setup() {
  Serial.begin(9600);
  //Serial.println(F("DHTxx test!"));
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  dht.begin();
}

void drawDisp(int type, int value) {
  display.clearDisplay();
  display.setTextSize(2);
  //display.setFont(&FreeSerif9pt7b);
  display.setTextColor(WHITE);
  display.setCursor(40,10);
  if(type == 0) {
    display.print(value);
    display.drawPixel(70,10, WHITE);
    display.drawPixel(70,11, WHITE);
    display.drawPixel(70,12, WHITE);
    display.drawPixel(71,10, WHITE);
    display.drawPixel(72,10, WHITE);
    display.drawPixel(73,10, WHITE);
    display.drawPixel(73,11, WHITE);
    display.drawPixel(73,12, WHITE);
    display.drawPixel(72,12, WHITE);
    display.drawPixel(71,12, WHITE);
    display.println(" C");
  }
  if(type == 1) {
    display.print(value);
    display.println(" %");
  }
  if(type == 2) {
    display.setCursor(20,10);
    display.print(value);
    display.println(" LUX");
  }
  
  display.display();
}

void loop() {
  time = millis();
  if(time % 1000 == 0) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t) ) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    
    //float light_read_value = analogRead(lightSensor);
    //float lux = ((2500/light_read_value)-500)/10.0;
    float rawLight = analogRead(lightSensor);
    float Vout = float(rawLight) * (VIN /float(1023));
    float RLDR = (R * (VIN - Vout))/Vout;
    int lux = 500/(RLDR/1000);
  
    Serial.print("LightRead: ");
    Serial.print(lux);
    Serial.print(" Humidity: ");
    Serial.print(h);
    Serial.print(" Temperature: ");
    Serial.print(t);
    Serial.print(" \n");

    if(time % 5000 == 0) {
      if(counter == 0) {
        int tDisp = (int)t;
        drawDisp(0, tDisp);
        counter += 1;
      }
      else if(counter == 1) {
        int hDisp = (int)h;
        drawDisp(1, hDisp);
        counter += 1;
      }
      else if(counter == 2) {
        int lDisp = (int)lux;
        drawDisp(2, lDisp);
        counter = 0;
      }
    }
  }
  
}
