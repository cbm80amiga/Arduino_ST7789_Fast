// ST7789 library example
// (c) 2019-20 Pawel A. Hernik

/*
 ST7789 240x240 IPS (without CS pin) connections (only 6 wires required):

 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> D13/SCK
 #04 SDA -> D11/MOSI
 #05 RES -> D8 or any digital
 #06 DC  -> D7 or any digital
 #07 BLK -> NC
*/

#define SCR_WD   240
#define SCR_HT   240
#include <SPI.h>
#include <Adafruit_GFX.h>

#if (__STM32F1__) // bluepill
#define TFT_DC  PA1
#define TFT_RST PA0
//#include <Arduino_ST7789_STM.h>
#else
#define TFT_DC  7
#define TFT_RST 8
#include <Arduino_ST7789_Fast.h>
//#include <Arduino_ST7789.h>
#endif

Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

void setup() 
{
  Serial.begin(9600);
  lcd.init();
}

void loop(void) 
{
  for(uint8_t rot = 0; rot < 4; rot++) {
    testText(rot);
    delay(2000);
  }
}

unsigned long testText(int rot)
{
  lcd.setRotation(rot);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0, 0);
  lcd.setTextColor(BLUE);
  lcd.setTextSize(1);
  lcd.println("Hello World!");
  lcd.setTextColor(WHITE);
  lcd.print("Rotation = ");
  lcd.println(rot);
  lcd.setTextColor(YELLOW);
  lcd.setTextSize(2);
  lcd.println(1234.56);
  lcd.setTextColor(RED);
  lcd.setTextSize(3);
  lcd.println(0xDEAD, HEX);
  lcd.println();
  lcd.setTextColor(GREEN);
  lcd.setTextSize(4);
  lcd.println("Hello");
}


