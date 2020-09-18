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

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

// ------------------------------------------------
unsigned long FillScreenTest()
{
  unsigned long start = millis();
  for(int i=0;i<5;i++) {
    tft.fillScreen(RED);
    tft.fillScreen(GREEN);
    tft.fillScreen(BLUE);
    tft.fillScreen(YELLOW);
  }
  return millis()-start;
}

// ------------------------------------------------
unsigned long ClearScreenTest()
{
  unsigned long start = millis();
  for(int i=0;i<5*4;i++) 
    tft.fillScreen(BLACK);
  return millis()-start;
}

// ------------------------------------------------
const uint16_t imgF[] PROGMEM = {0xF800,0xF840,0xF8A0,0xF900,0xF960,0xF9C0,0xFA20,0xFA80,0xFAE0,0xFB40,0xFBA0,0xFC00,0xFC60,0xFCC0,0xFD20,0xFD80,0xFDE0,0xFE40,0xFEA0,0xFF00,0xFF60,0xFFC0,0xFFE0,0xEFE0,0xE7E0,0xD7E0,0xCFE0,0xBFE0,0xB7E0,0xA7E0,0x9FE0,0x8FE0,0x87E0,0x77E0,0x6FE0,0x5FE0,0x57E0,0x47E0,0x3FE0,0x2FE0,0x27E0,0x17E0,0xFE0,0x7E0,0x7E1,0x7E3,0x7E4,0x7E6,0x7E7,0x7E9,0x7EA,0x7EC,0x7ED,0x7EF,0x7F0,0x7F2,0x7F3,0x7F5,0x7F6,0x7F8,0x7F9,0x7FB,0x7FC,0x7FE,0x7FF,0x79F,0x73F,0x6DF,0x67F,0x61F,0x5BF,0x55F,0x4FF,0x49F,0x43F,0x3DF,0x37F,0x31F,0x2BF,0x25F,0x1FF,0x19F,0x13F,0xDF,0x7F,0x1F,0x81F,0x101F,0x201F,0x281F,0x381F,0x401F,0x501F,0x581F,0x681F,0x701F,0x801F,0x881F,0x981F,0xA01F,0xB01F,0xB81F,0xC81F,0xD01F,0xE01F,0xE81F,0xF81F,0xF81F,0xF81D,0xF81C,0xF81A,0xF819,0xF817,0xF816,0xF814,0xF813,0xF811,0xF810,0xF80E,0xF80D,0xF80B,0xF80A,0xF808,0xF807,0xF805,0xF804,0xF802,0xF801,
                                 0xF800,0xF840,0xF8A0,0xF900,0xF960,0xF9C0,0xFA20,0xFA80,0xFAE0,0xFB40,0xFBA0,0xFC00,0xFC60,0xFCC0,0xFD20,0xFD80,0xFDE0,0xFE40,0xFEA0,0xFF00,0xFF60,0xFFC0,0xFFE0,0xEFE0,0xE7E0,0xD7E0,0xCFE0,0xBFE0,0xB7E0,0xA7E0,0x9FE0,0x8FE0,0x87E0,0x77E0,0x6FE0,0x5FE0,0x57E0,0x47E0,0x3FE0,0x2FE0,0x27E0,0x17E0,0xFE0,0x7E0,0x7E1,0x7E3,0x7E4,0x7E6,0x7E7,0x7E9,0x7EA,0x7EC,0x7ED,0x7EF,0x7F0,0x7F2,0x7F3,0x7F5,0x7F6,0x7F8,0x7F9,0x7FB,0x7FC,0x7FE,0x7FF,0x79F,0x73F,0x6DF,0x67F,0x61F,0x5BF,0x55F,0x4FF,0x49F,0x43F,0x3DF,0x37F,0x31F,0x2BF,0x25F,0x1FF,0x19F,0x13F,0xDF,0x7F,0x1F,0x81F,0x101F,0x201F,0x281F,0x381F,0x401F,0x501F,0x581F,0x681F,0x701F,0x801F,0x881F,0x981F,0xA01F,0xB01F,0xB81F,0xC81F,0xD01F,0xE01F,0xE81F,0xF81F,0xF81F,0xF81D,0xF81C,0xF81A,0xF819,0xF817,0xF816,0xF814,0xF813,0xF811,0xF810,0xF80E,0xF80D,0xF80B,0xF80A,0xF808,0xF807,0xF805,0xF804,0xF802,0xF801};
uint16_t img[SCR_WD+16];
unsigned long DrawImageTest()
{
  for(int i=0;i<SCR_WD+16;i++) img[i] = tft.rgbWheel(500L*i/SCR_WD);
  unsigned long start = millis();
  for(int i=0;i<5*4;i++) for(int y=0;y<SCR_HT;y++) tft.drawImage(0,y,SCR_WD,1,img+(((y>>2)+i)&0xf));
  return millis()-start;
}

// ------------------------------------------------ 
unsigned long DrawImageFTest()
{
  unsigned long start = millis();
  for(int i=0;i<5*4;i++) for(int y=0;y<SCR_HT;y++) tft.drawImageF(0,y,SCR_WD,1,imgF+(((y>>2)+i)&0xf));
  return millis()-start;
}
// ------------------------------------------------
// original Adafruit
unsigned long orig[14]={ 5872, 5872, 1468292,394696,2938532,132952,121996,4068208,925276,1319512, 1278096,1930524,602080,4649200 };
// pre 18.09.2020 Fast
//unsigned long orig[14]={2645,3577,661616,114064,579036,56216,49500,1830864,222936,259340,175460,903348,138264,1853300};

unsigned long res[14];
void result(int i)
{
  Serial.print(res[i]);
  if(res[i]<1000000) Serial.print("\t");
  Serial.print("\t\t\t");
  Serial.print(100*orig[i]/res[i]);
  Serial.println("%");
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println(F("IPS 240x240 ST7789"));
  //tft.reset();
  //tft.init(SCR_WD, SCR_HT);
  tft.begin();
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.println("IPS 240x240 ST7789");
  tft.println("Library Benchmark");
  tft.println("starts in 3s ...");
  delay(3000);

  Serial.println(F("Benchmark                Time (microseconds)"));

  res[0]=FillScreenTest();
  Serial.print(F("FillScreen Mbps          "));
  Serial.println(String(res[0])+"ms  "+String(1000*20.0/res[0])+"fps  "+String(20.0*SCR_WD*SCR_HT*16/res[0]/1000.0)+" Mbps\t"+100*orig[0]/res[0]+"%");
//for(int i=0;i<100;i++) tft.fillRect(i,i,SCR_WD-i*2,SCR_HT-i*2,tft.rgbWheel(i*10)); delay(1000);

  res[1]=ClearScreenTest();
  Serial.print(F("ClearScreen Mbps         "));
  Serial.println(String(res[1])+"ms  "+String(1000*20.0/res[1])+"fps  "+String(20.0*SCR_WD*SCR_HT*16/res[1]/1000.0)+" Mbps\t"+100*orig[1]/res[1]+"%");

  res[1]=DrawImageTest();
  Serial.print(F("DrawImage Mbps           "));
  Serial.println(String(res[1])+"ms  "+String(1000*20.0/res[1])+"fps  "+String(20.0*SCR_WD*SCR_HT*16/res[1]/1000.0)+" Mbps\t"+100*orig[1]/res[1]+"%"); 

  res[1]=DrawImageFTest();
  Serial.print(F("DrawImageF Mbps          "));
  Serial.println(String(res[1])+"ms  "+String(1000*20.0/res[1])+"fps  "+String(20.0*SCR_WD*SCR_HT*16/res[1]/1000.0)+" Mbps\t"+100*orig[1]/res[1]+"%"); 

  res[2]=testFillScreen();
  Serial.print(F("Screen fill              "));
  result(2);
  delay(500);

  res[3]=testText();
  Serial.print(F("Text                     "));
  result(3);
  delay(3000);

  res[4]=testLines(CYAN);
  Serial.print(F("Lines                    "));
  result(4);
  delay(500);

  res[5]=testFastLines(RED, BLUE);
  Serial.print(F("Horiz/Vert Lines         "));
  result(5);
  delay(500);

  res[6]=testRects(GREEN);
  Serial.print(F("Rectangles (outline)     "));
  result(6);
  delay(500);

  res[7]=testFilledRects(YELLOW, MAGENTA);
  Serial.print(F("Rectangles (filled)      "));
  result(7);
  delay(500);

  res[8]=testFilledCircles(10, MAGENTA);
  Serial.print(F("Circles (filled)         "));
  result(8);

  res[9]=testCircles(10, WHITE);
  Serial.print(F("Circles (outline)        "));
  result(9);
  delay(500);

  res[10]=testTriangles();
  Serial.print(F("Triangles (outline)      "));
  result(10);
  delay(500);

  res[11]=testFilledTriangles();
  Serial.print(F("Triangles (filled)       "));
  result(11);
  delay(500);

  res[12]=testRoundRects();
  Serial.print(F("Rounded rects (outline)  "));
  result(12);
  delay(500);

  res[13]=testFilledRoundRects();
  Serial.print(F("Rounded rects (filled)   "));
  result(13);
  delay(500);

  Serial.println(F("Done!"));
  Serial.println(F("Results:"));
  for(int i=0;i<14;i++) { Serial.print(res[i]); Serial.print(","); }
  Serial.println();

  int c1=YELLOW, c2=WHITE;
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println("RESULTS:");

  tft.setTextSize(1);
  tft.println();
  tft.setTextColor(GREEN);
  tft.println(F("Benchmark               Time (us)"));
  tft.setTextColor(c1); tft.print(F("FillScreen Mbps         "));
  tft.setTextColor(c2); tft.println(String(res[0])+"ms "+String(20.0*SCR_WD*SCR_HT*16/res[0]/1000.0)+" Mbps");
  //tft.setTextColor(c1); tft.print(F("ClearScreen Mbps        "));
  //tft.setTextColor(c2); tft.print(String(res[1])+"ms "+String(20.0*SCR_WD*SCR_HT*16/res[1]/1000.0)+" Mbps");
  tft.setTextColor(c1); tft.print(F("DrawImageF Mbps         "));
  tft.setTextColor(c2); tft.println(String(res[1])+"ms "+String(20.0*SCR_WD*SCR_HT*16/res[1]/1000.0)+" Mbps");

  tft.setTextColor(c1); tft.print(F("Screen fill             "));
  tft.setTextColor(c2); tft.println(res[3]);
  tft.setTextColor(c1); tft.print(F("Text                    "));
  tft.setTextColor(c2); tft.println(res[4]);
  tft.setTextColor(c1); tft.print(F("Lines                   "));
  tft.setTextColor(c2); tft.println(res[5]);
  tft.setTextColor(c1); tft.print(F("Horiz/Vert Lines        "));
  tft.setTextColor(c2); tft.println(res[6]);
  tft.setTextColor(c1); tft.print(F("Rectangles (outline)    "));
  tft.setTextColor(c2); tft.println(res[8]);
  tft.setTextColor(c1); tft.print(F("Rectangles (filled)     "));
  tft.setTextColor(c2); tft.println(res[9]);
  tft.setTextColor(c1); tft.print(F("Circles (filled)        "));
  tft.setTextColor(c2); tft.println(res[10]);
  tft.setTextColor(c1); tft.print(F("Circles (outline)       "));
  tft.setTextColor(c2); tft.println(res[11]);
  tft.setTextColor(c1); tft.print(F("Triangles (outline)     "));
  tft.setTextColor(c2); tft.println(res[12]);
  tft.setTextColor(c1); tft.print(F("Triangles (filled)      "));
  tft.setTextColor(c2); tft.println(res[13]);
  tft.setTextColor(c1); tft.print(F("Rounded rects (outline) "));
  tft.setTextColor(c2); tft.println(res[14]);
  tft.setTextColor(c1); tft.print(F("Rounded rects (filled)  "));
  tft.setTextColor(c2); tft.println(res[15]);
  tft.setTextColor(RED); tft.println(F("Done!"));
}

/*
Optimizations from 18.9.2020

-- IPS 240x240 ST7789, back to old setAddrWindow() with writeSPI, flash: 28,550
Benchmark                Time (microseconds)
FillScreen Mbps          2609ms  7.67fps  7.06 Mbps 225%
ClearScreen Mbps         2609ms  7.67fps  7.06 Mbps 225%
DrawImage Mbps           2740ms  7.30fps  6.73 Mbps 214%
DrawImageF Mbps          3579ms  5.59fps  5.15 Mbps 164%
Screen fill              652528       225%
Text                     109748       359%
Lines                    572992       512%
Horiz/Vert Lines         54696        243%
Rectangles (outline)     47916        254%
Rectangles (filled)      1805552      225%
Circles (filled)         208212       444%
Circles (outline)        255744       515%
Triangles (outline)      173468       736%
Triangles (filled)       880160       219%
Rounded rects (outline)  135920       442%
Rounded rects (filled)   1819660      255%
Done!
Results:
2609,3579,652528,109748,572992,54696,47916,1805552,208212,255744,173468,880160,135920,1819660,


-- IPS 240x240 ST7789 + back to old drawPixel, flash: 28,456
Benchmark                Time (microseconds)
FillScreen Mbps          2609ms  7.67fps  7.06 Mbps 225%
ClearScreen Mbps         2609ms  7.67fps  7.06 Mbps 225%
DrawImage Mbps           2750ms  7.27fps  6.70 Mbps 213%
DrawImageF Mbps          3588ms  5.57fps  5.14 Mbps 163%
Screen fill              652540       225%
Text                     114004       346%
Lines                    609216       482%
Horiz/Vert Lines         54884        242%
Rectangles (outline)     48216        253%
Rectangles (filled)      1805640      225%
Circles (filled)         213896       432%
Circles (outline)        272036       485%
Triangles (outline)      184020       694%
Triangles (filled)       885728       217%
Rounded rects (outline)  142432       422%
Rounded rects (filled)   1821976      255%
Done!
Results:
2609,3588,652540,114004,609216,54884,48216,1805640,213896,272036,184020,885728,142432,1821976,


-- IPS 240x240 ST7789 - all previous + no _x/ystart in setAddrWindow(), flash: 28,462
Benchmark                Time (microseconds)
FillScreen Mbps          2609ms  7.67fps  7.06 Mbps  225%
ClearScreen Mbps         2609ms  7.67fps  7.06 Mbps 225%
DrawImage Mbps           2749ms  7.28fps  6.70 Mbps 213%
DrawImageF Mbps          3588ms  5.57fps  5.14 Mbps 163%
Screen fill              652536       225%
Text                     115248       342%
Lines                    622492       472%
Horiz/Vert Lines         54876        242%
Rectangles (outline)     48216        253%
Rectangles (filled)      1805608      225%
Circles (filled)         213896       432%
Circles (outline)        278020       474%
Triangles (outline)      187872       680%
Triangles (filled)       885744       217%
Rounded rects (outline)  144708       416%
Rounded rects (filled)   1821980      255%
Done!
Results:
2609,3588,652536,115248,622492,54876,48216,1805608,213896,278020,187872,885744,144708,1821980,

-- IPS 240x240 ST7789 optimized write and copy, flash 28,506
Benchmark                Time (microseconds)
FillScreen Mbps          2609ms  7.67fps  7.06 Mbps  225%
ClearScreen Mbps         2608ms  7.67fps  7.07 Mbps 225%
DrawImage Mbps           2751ms  7.27fps  6.70 Mbps 213%
DrawImageF Mbps          3592ms  5.57fps  5.13 Mbps 163%
Screen fill              652552       225%
Text                     117240       336% (---)
Lines                    639400       459% (---)
Horiz/Vert Lines         54968        241%
Rectangles (outline)     48364        252%
Rectangles (filled)      1805680      225%
Circles (filled)         216544       427%
Circles (outline)        285624       461%
Triangles (outline)      192788       662%
Triangles (filled)       888328       217%
Rounded rects (outline)  147748       407% (---)
Rounded rects (filled)   1823064      255%
Done!
Results:
2609,3592,652552,117240,639400,54968,48364,1805680,216544,285624,192788,888328,147748,1823064,

-- IPS 240x240 ST7789 Fast original before 18.9.20 changes, flash: 29,356 bytes
Benchmark                Time (microseconds)
FillScreen Mbps          2645ms  7.56fps  6.97 Mbps  222%
ClearScreen Mbps         2645ms  7.56fps  6.97 Mbps 222%
DrawImage Mbps           3026ms  6.61fps  6.09 Mbps 194%
DrawImageF Mbps          3577ms  5.59fps  5.15 Mbps 164%
Screen fill              661616       221%
Text                     114064       346%
Lines                    579036       507%
Horiz/Vert Lines         56216        236%
Rectangles (outline)     49500        246%
Rectangles (filled)      1830864      222%
Circles (filled)         222936       415%
Circles (outline)        259340       508%
Triangles (outline)      175460       728%
Triangles (filled)       903348       213%
Rounded rects (outline)  138264       435%
Rounded rects (filled)   1853300      250%
Done!
Results:
2645,3577,661616,114064,579036,56216,49500,1830864,222936,259340,175460,903348,138264,1853300,

-- IPS 240x240 ST7789 in COMPATIBILITY mode
Benchmark                Time (microseconds)
FillScreen Mbps          5361ms  3.73fps  3.44 Mbps  109%
ClearScreen Mbps         5362ms  3.73fps  3.44 Mbps 109%
DrawImage Mbps           5665ms  3.53fps  3.25 Mbps 103%
DrawImageF Mbps          5706ms  3.51fps  3.23 Mbps 102%
Screen fill              1340716      109%
Text                     244268       161%
Lines                    1652188      177%
Horiz/Vert Lines         114840       115%
Rectangles (outline)     101892       119%
Rectangles (filled)      3711592      109%
Circles (filled)         487852       189%
Circles (outline)        741392       177%
Triangles (outline)      494348       258%
Triangles (filled)       1614292      119%
Rounded rects (outline)  363292       165%
Rounded rects (filled)   3761392      123%
Done!
Results:
5361,5706,1340716,244268,1652188,114840,101892,3711592,487852,741392,494348,1614292,363292,3761392,

*/

/*
IPS ST7789 original:
Benchmark                Time (microseconds)
FillScreen               5872ms 3.41fps
ClearScreen              5872ms 3.41fps
Screen fill              1468292
Text                     394696
Lines                    2938532
Horiz/Vert Lines         132952
Rectangles (outline)     121996
Rectangles (filled)      4068208
Circles (filled)         925276
Circles (outline)        1319512
Triangles (outline)      1278096
Triangles (filled)       1930524
Rounded rects (outline)  602080
Rounded rects (filled)   4649200
Done!


IPS 240x240 ST7789 optimized - FINAL
Benchmark                Time (microseconds)
FillScreen               2645ms 7.56fps
ClearScreen              2645ms 7.56fps
Screen fill              661488
Text                     108816
Lines                    563888
Horiz/Vert Lines         56192
Rectangles (outline)     49128
Rectangles (filled)      1830804
Circles (filled)         270728
Circles (outline)        250940
Triangles (outline)      245104
Triangles (filled)       897880
Rounded rects (outline)  135416
Rounded rects (filled)   2030816
Done!

Fill speed:
240*240*2 = 115 200b  *20 *8bit/2.645 ->6.9Mbps (!)


IPS 240x240 ST7789
Benchmark                Time (microseconds)
FillScreen Mbps          2645ms  7.56fps  6.97 Mbps  222%
ClearScreen Mbps         2645ms  7.56fps  6.97 Mbps 222%
Screen fill              661624       221%
Text                     109496       360%
Lines                    563888       521%
Horiz/Vert Lines         56216        236%
Rectangles (outline)     49168        248%
Rectangles (filled)      1830880      222%
Circles (filled)         271772       340%
Circles (outline)        251000       525%
Triangles (outline)      245104       521%
Triangles (filled)       898632       214%
Rounded rects (outline)  135368       444%
Rounded rects (filled)   2031308      228%
Done!

*/
// ------------------------------------------------

void loop(void) 
{
}

// ------------------------------------------------

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  return micros() - start;
}

// ------------------------------------------------
unsigned long testText() {
  tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

// ------------------------------------------------
unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

// ------------------------------------------------
unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

// ------------------------------------------------
unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

// ------------------------------------------------
unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

// ------------------------------------------------
unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

// ------------------------------------------------
unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

// ------------------------------------------------
unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}

// ------------------------------------------------
unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}

// ------------------------------------------------
unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

// ------------------------------------------------
unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
  }

  return micros() - start;
}
// ------------------------------------------------

