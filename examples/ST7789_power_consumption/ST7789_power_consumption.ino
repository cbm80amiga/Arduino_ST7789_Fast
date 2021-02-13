// ST7789 Power Consumption Tests
// (C)2021 Pawel A. Hernik
// requires RRE Font library:
// https://github.com/cbm80amiga/RREFont
// YouTube video: https://youtu.be/M-fKkN0bKA0

/*
 ST7789 240x240 IPS (without CS pin) connections (only 6 wires required):

 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> PA5/SCK
 #04 SDA -> PA7/MOSI
 #05 RES -> PA0 or any digital
 #06 DC  -> PA1 or any digital
 #07 BLK -> NC
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#define TFT_DC     10
#define TFT_RST    9
#define TFT_BL     8

#define SCR_WD 240
#define SCR_HT 240
#include <Arduino_ST7789_Fast.h>
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

#include "RREFont.h"
#include "rre_chicago_20x24.h"


RREFont font;

// needed for RREFont library initialization, define your fillRect
void customRect(int x, int y, int w, int h, int c) { return lcd.fillRect(x, y, w, h, c); }

void rainbow()
{
  for(int i=0;i<240;i+=4) {
    uint8_t r,g,b;
    lcd.rgbWheel(i*512L/240,&r,&g,&b);
    lcd.fillRect(0,i,240,4,RGBto565(r,g,b));
  }
}

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

enum wdt_time {
  SLEEP_15MS,
  SLEEP_30MS, 
  SLEEP_60MS,
  SLEEP_120MS,
  SLEEP_250MS,
  SLEEP_500MS,
  SLEEP_1S,
  SLEEP_2S,
  SLEEP_4S,
  SLEEP_8S,
  SLEEP_FOREVER
};

ISR(WDT_vect) { wdt_disable(); }

void powerDown(uint8_t per)
{
  ADCSRA &= ~(1 << ADEN);  // turn off ADC
  if(per != SLEEP_FOREVER) { // use watchdog timer
    wdt_enable(per);
    WDTCSR |= (1 << WDIE);  
  }
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // most power saving
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  // ... sleeping here
  sleep_disable();
  ADCSRA |= (1 << ADEN); // turn on ADC
} 

void wait(int t=SLEEP_4S)
{
  // force D13/SPI-SCK LED off and deep sleep for 4s
  SPI.end();
  pinMode(13, OUTPUT);  digitalWrite(13, LOW);
  powerDown(t);
  //SPI.begin();
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
  //SPI.setDataMode(SPI_MODE3);
  lcd.init(SCR_WD, SCR_HT);
}

void setup() 
{
  Serial.begin(9600);
  lcd.init();
  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
  font.setFont(&rre_chicago_20x24);
  font.setScale(1,2); font.setSpacing(3);
  //font.setCR(1);
  font.setColor(WHITE);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
}

void loop()
{
  lcd.fillScreen(RGBto565(120,60,30));
  font.printStr(ALIGN_CENTER,40,"Power");
  font.printStr(ALIGN_CENTER,40+60,"consumption");
  font.printStr(ALIGN_CENTER,40+120,"tests ...");
  delay(2000);

  rainbow();
  font.setColor(BLACK);
  font.printStr(ALIGN_CENTER,65,"Regular");
  font.printStr(ALIGN_CENTER,65+60,"no saving");
  SPI.end();
  pinMode(13, OUTPUT);  digitalWrite(13, LOW);
  delay(4000);
  lcd.init(SCR_WD, SCR_HT);

  rainbow();
  font.setColor(BLACK);
  font.printStr(ALIGN_CENTER,40,"AVR deep sleep");
  font.printStr(ALIGN_CENTER,40+60,"1. BL ON ");
  font.printStr(ALIGN_CENTER,40+120,"2. BL OFF");
  wait();
  digitalWrite(TFT_BL, LOW);
  wait();
  digitalWrite(TFT_BL, HIGH);

  lcd.fillScreen(WHITE);
  wait();
  lcd.fillScreen(BLACK);
  wait();
  lcd.fillScreen(RED);
  wait();
  lcd.fillScreen(GREEN);
  wait();
  lcd.fillScreen(BLUE);
  wait();

  rainbow();
  font.setColor(BLACK);
  font.printStr(ALIGN_CENTER,95,"Idle mode OFF");
  lcd.idleDisplay(false);
  wait();

  rainbow();
  font.printStr(ALIGN_CENTER,95,"Idle mode ON");
  lcd.idleDisplay(true);
  wait();
  lcd.idleDisplay(false);

  rainbow();
  font.setColor(WHITE,BLACK);
  lcd.fillRect(30,87,240-60,60,BLACK);
  font.printStr(ALIGN_CENTER,95,"Invert OFF");
  lcd.invertDisplay(false);
  wait();
  font.printStr(ALIGN_CENTER,95," Invert ON ");
  lcd.invertDisplay(true);
  wait();
  lcd.invertDisplay(false);

  font.setColor(WHITE);
  lcd.fillScreen(RGBto565(180,0,180));
  font.printStr(ALIGN_CENTER,40,"Sleep mode in 2s");
  font.printStr(ALIGN_CENTER,40+60,"1. BL ON ");
  font.printStr(ALIGN_CENTER,40+120,"2. BL OFF");
  wait(SLEEP_2S);
  //lcd.enableDisplay(false); 
  lcd.sleepDisplay(true);
  wait();
  lcd.sleepDisplay(true);
  digitalWrite(TFT_BL, LOW);
  wait();
  digitalWrite(TFT_BL, HIGH);
  lcd.sleepDisplay(false); 
  //lcd.enableDisplay(true);

  lcd.fillScreen(RGBto565(180,0,180));
  font.printStr(6,10,"Display disable");
  font.printStr(6,10+60,"1. BL ON");
  font.printStr(6,10+120,"2. BL OFF");
  font.printStr(6,10+180,"3. BL OFF + Sleep");
  wait(SLEEP_2S);

  lcd.enableDisplay(false);   // display disable, BL ON
  wait();

  lcd.enableDisplay(false);
  digitalWrite(TFT_BL, LOW);    // display disable, BL OFF
  wait();
  lcd.enableDisplay(false);
  lcd.sleepDisplay(true); 
  digitalWrite(TFT_BL, LOW);    // display disable, sleep, BL OFF
  wait();
  lcd.sleepDisplay(false); 
  lcd.enableDisplay(true);
  digitalWrite(TFT_BL, HIGH);

  lcd.fillScreen(RGBto565(180,0,180));
  font.printStr(ALIGN_CENTER,95,"Partial display");
  font.setColor(YELLOW);
  font.printStr(ALIGN_CENTER,6,"Top");
  font.printStr(ALIGN_CENTER,240-50,"Bottom");
  font.setColor(WHITE);
  wait(SLEEP_2S);

  lcd.setPartArea(60*1, 60*3); lcd.partialDisplay(true);
  wait();

  lcd.setPartArea(60*3, 60*1); lcd.partialDisplay(true);
  wait();
  lcd.partialDisplay(false);
}

