// ST7789 library example
// Analog watch/clock, AVR + DS1307/DS3231 RTC module
// (c) 2020 Pawel A. Hernik
// YT videos:
// https://youtu.be/jFGDFuLhdMc 
// https://youtu.be/35Z0enhEYqM 
// https://youtu.be/Xr-dxPhePhY

/*
 ST7789 240x240 IPS (without CS pin) connections (only 6 wires required):

 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> D13/PA5/SCK
 #04 SDA -> D11/PA7/MOSI
 #05 RES -> D9 /PA0 or any digital
 #06 DC  -> D10/PA1 or any digital
 #07 BLK -> NC
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#if (__STM32F1__) // bluepill
#define TFT_DC    PA1
#define TFT_RST   PA0
#define BUTTON    PB9
//#include <Arduino_ST7789_STM.h>
#else
#define TFT_DC    7
#define TFT_RST   8
#define BUTTON    3
#include <Arduino_ST7789_Fast.h>
#endif

#define SCR_WD 240
#define SCR_HT 240
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

//#include "sansmains.h"
//#include "smartq02.h"
//#include "smartq04.h"
//#include "smartq09.h"
//#include "smartq07.h"
#include "smartq08.h"
const unsigned char *clockface = dial2bit;

//#include "small4x6_font.h"

struct RTCData {
  int hour,minute,second;
  int year,month,day,dayOfWeek;
};

RTCData cur;

#define USEHW 1
#include <Wire.h>
#include "rtc.h"

uint8_t txt2num(const char* p) 
{
  return 10*(*p-'0') + *(p+1)-'0';
}

// configuration

const uint16_t cx = SCR_WD/2, cy = SCR_HT/2;  // clock center
const uint16_t selHandCol1 = RGBto565(250,250,0);
const uint16_t selHandCol2 = RGBto565(180,180,0);
const uint16_t mHandCol1 = RGBto565(220,220,220);
const uint16_t mHandCol2 = RGBto565(150,150,150);
const uint16_t hHandCol1 = RGBto565(220,220,220);
const uint16_t hHandCol2 = RGBto565(150,150,150);
const uint16_t sHandCol1 = RGBto565(250,80,80);
const uint16_t sHandCol2 = RGBto565(200,0,0);
int hHandL = 25*2, hHandW = 3*2;
int mHandL = 36*2, mHandW = 3*2;
int sHandL = 44*2, sHandW = 2*2;


int sDeg,mDeg,hDeg;
int sDegOld,mDegOld,hDegOld;
unsigned long styleTime, ms;
uint8_t hh = txt2num(__TIME__+0);
uint8_t mm = txt2num(__TIME__+3);
uint8_t ss = txt2num(__TIME__+6);
uint8_t start = 1;
int setMode = 0;

// ----------------------------------------------------------------

uint16_t palette[4];
uint16_t line[SCR_WD+4];

void imgLineH(int x, int y, int w)
{
  uint8_t v,*img = (uint8_t*)clockface+4*2+6+y*SCR_WD/4+x/4;
  int ww = (x&3)?w+4:w;
  for(int i=0;i<ww;i+=4) {
    v = pgm_read_byte(img++);
    line[i+0] = palette[(v>>6)&3];
    line[i+1] = palette[(v>>4)&3];
    line[i+2] = palette[(v>>2)&3];
    line[i+3] = palette[v&3];
  }
  lcd.drawImage(x,y,w,1,line+(x&3));
}

void imgRect(int x, int y, int w, int h)
{
  for(int i=y;i<y+h;i++) imgLineH(x,i,w);
}
// ----------------------------------------------------------------
#define swap(a, b) { int16_t t = a; a = b; b = t; }

void imgTriangle(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3, uint16_t c=0)
{
  int16_t t1x,t2x,y,minx,maxx,t1xp,t2xp;
  bool changed1 = false,changed2 = false;
  int16_t signx1,signx2,dx1,dy1,dx2,dy2;
  uint16_t e1,e2;

  if (y1>y2) { swap(y1,y2); swap(x1,x2); }
  if (y1>y3) { swap(y1,y3); swap(x1,x3); }
  if (y2>y3) { swap(y2,y3); swap(x2,x3); }

  t1x=t2x=x1; y=y1;   // Starting points

  dx1 = x2 - x1; if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
  dy1 = y2 - y1;
 
  dx2 = x3 - x1; if(dx2<0) { dx2=-dx2; signx2=-1; } else signx2=1;
  dy2 = y3 - y1;
  
  if (dy1 > dx1) { swap(dx1,dy1); changed1 = true; }
  if (dy2 > dx2) { swap(dy2,dx2); changed2 = true; }
  
  e2 = dx2>>1;
  if(y1==y2) goto next;  // Flat top, just process the second half
  e1 = dx1>>1;
  
  for (uint16_t i = 0; i < dx1;) {
    t1xp=0; t2xp=0;
    if(t1x<t2x) { minx=t1x; maxx=t2x; }
    else    { minx=t2x; maxx=t1x; }
    // process first line until y value is about to change
    while(i<dx1) {
      i++;      
      e1 += dy1;
      while (e1 >= dx1) {
        e1 -= dx1;
               if (changed1) t1xp=signx1;//t1x += signx1;
        else          goto next1;
      }
      if (changed1) break;
      else t1x += signx1;
    }
  // Move line
  next1:
    // process second line until y value is about to change
    while (1) {
      e2 += dy2;    
      while (e2 >= dx2) {
        e2 -= dx2;
        if (changed2) t2xp=signx2;//t2x += signx2;
        else          goto next2;
      }
      if (changed2)     break;
      else              t2x += signx2;
    }
  next2:
    if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
    if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
    
    // line from min to max points found on the y
    if(c) lcd.drawFastHLine(minx, y, maxx-minx, c); else imgLineH(minx, y, maxx-minx); 
    // increase y
    if(!changed1) t1x += signx1;
    t1x+=t1xp;
    if(!changed2) t2x += signx2;
    t2x+=t2xp;
    y += 1;
    if(y==y2) break;
    
   }
  next:
  
  // Second half
  dx1 = x3 - x2; if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
  dy1 = y3 - y2;
  t1x=x2;
 
  if (dy1 > dx1) { swap(dy1,dx1); changed1 = true; } else changed1=false;
  
  e1 = dx1>>1;
  
  for (uint16_t i = 0; i<=dx1; i++) {
    t1xp=0; t2xp=0;
    if(t1x<t2x) { minx=t1x; maxx=t2x; }
    else    { minx=t2x; maxx=t1x; }
      // process first line until y value is about to change
    while(i<dx1) {
        e1 += dy1;
          while (e1 >= dx1) {
        e1 -= dx1;
                if (changed1) { t1xp=signx1; break; }//t1x += signx1;
        else          goto next3;
      }
      if (changed1) break;
      else          t1x += signx1;
      if(i<dx1) i++;
    }
  next3:
    // process second line until y value is about to change
    while (t2x!=x3) {
      e2 += dy2;
          while (e2 >= dx2) {
        e2 -= dx2;
        if(changed2) t2xp=signx2;
        else          goto next4;
      }
      if (changed2)     break;
      else              t2x += signx2;
    }        
  next4:
    if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
    if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
    // line from min to max points found on the y
    if(c) lcd.drawFastHLine(minx, y, maxx-minx, c); else imgLineH(minx, y, maxx-minx); 
    // increase y
    if(!changed1) t1x += signx1;
    t1x+=t1xp;
    if(!changed2) t2x += signx2;
    t2x+=t2xp;
      y += 1;
    if(y>y3) return;
  }
}

// ------------------------------------------------
#define MAXSIN 255
const uint8_t sinTab[91] PROGMEM = {
0,4,8,13,17,22,26,31,35,39,44,48,53,57,61,65,70,74,78,83,87,91,95,99,103,107,111,115,119,123,
127,131,135,138,142,146,149,153,156,160,163,167,170,173,177,180,183,186,189,192,195,198,200,203,206,208,211,213,216,218,
220,223,225,227,229,231,232,234,236,238,239,241,242,243,245,246,247,248,249,250,251,251,252,253,253,254,254,254,254,254,
255
};

int fastSin(int i)
{
  while(i<0) i+=360;
  while(i>=360) i-=360;
  if(i<90)  return(pgm_read_byte(&sinTab[i])); else
  if(i<180) return(pgm_read_byte(&sinTab[180-i])); else
  if(i<270) return(-pgm_read_byte(&sinTab[i-180])); else
            return(-pgm_read_byte(&sinTab[360-i]));
}

int fastCos(int i)
{
  return fastSin(i+90);
}

// ------------------------------------------------

int px[8],py[8];

void drawHand(int deg, int w, int l, int col1=0, int col2=0)
{
  int i,num = 4;
  px[0]= 0, py[0]= l;
  px[1]=-w-1, py[1]= 0;
  px[2]= w+1, py[2]= 0;
  px[3]= 0, py[3]=-15;
  int x[5],y[5];
  int cc = fastCos(deg+180);
  int ss = fastSin(deg+180);
  for(i=0;i<num;i++) {
    x[i] = px[i]*cc - py[i]*ss;
    y[i] = px[i]*ss + py[i]*cc;
    x[i] = cx + (x[i]+(x[i]>0?MAXSIN/2:-MAXSIN/2))/MAXSIN;
    y[i] = cy + (y[i]+(y[i]>0?MAXSIN/2:-MAXSIN/2))/MAXSIN;
  }
  imgTriangle(x[0],y[0], x[1],y[1], x[3],y[3], col2);
  imgTriangle(x[2],y[2], x[3],y[3], x[0],y[0], col1);
}

void drawHandS(int deg, int w, int l, int col1=0, int col2=0)
{
  int i,num = 8;
  px[0]=-w+3, py[0]= l;
  px[1]=-w+3, py[1]=-20;
  px[2]= w-3, py[2]= l;
  px[3]= w-3, py[3]=-20;
  px[4]=-w+1, py[4]=-40;
  px[5]=-w+1, py[5]=-15;
  px[6]= w-1, py[6]=-40;
  px[7]= w-1, py[7]=-15;
  int x[8],y[8];
  int cc = fastCos(deg+180);
  int ss = fastSin(deg+180);
  for(i=0;i<num;i++) {
    x[i] = px[i]*cc - py[i]*ss;
    y[i] = px[i]*ss + py[i]*cc;
    x[i] = cx + (x[i]+(x[i]>0?MAXSIN/2:-MAXSIN/2))/MAXSIN;
    y[i] = cy + (y[i]+(y[i]>0?MAXSIN/2:-MAXSIN/2))/MAXSIN;
  }
  imgTriangle(x[0],y[0], x[1],y[1], x[3],y[3], col1);
  imgTriangle(x[2],y[2], x[3],y[3], x[0],y[0], col1);
  imgTriangle(x[4],y[4], x[5],y[5], x[7],y[7], col1);
  imgTriangle(x[6],y[6], x[7],y[7], x[4],y[4], col1);
}

void clockUpdate() 
{
  if(millis()-ms<1000 && !start) return;
  ms = millis();
  if(setMode==0) {
    //getRTCDateTime(&cur);
    if(1) {
    if(++cur.second>59) {
      cur.second = 0;
      if(++cur.minute>59) {
        cur.minute = 0;
        if(++cur.hour>11) {
          cur.hour = 0;
        }
      }
    }
    }
    ss = cur.second;
    mm = cur.minute;
    hh = cur.hour;
  }

  sDeg = ss*6;

  if(ss==0 || start) {
    start = 0;
    mDeg = mm*6+sDeg/60;
    hDeg = hh*30+mDeg/12;
    drawHand(hDegOld,hHandW,hHandL);
    drawHand(mDegOld,mHandW,mHandL);
    mDegOld = mDeg;
    hDegOld = hDeg;
  }
  
  drawHandS(sDegOld,sHandW,sHandL);
  if(setMode==1)
    drawHand(hDeg,hHandW,hHandL,selHandCol1,selHandCol2);
  else
    drawHand(hDeg,hHandW,hHandL,hHandCol1,hHandCol2);
  if(setMode==2)
    drawHand(mDeg,mHandW,mHandL,selHandCol1,selHandCol2);
  else
    drawHand(mDeg,mHandW,mHandL,mHandCol1,mHandCol2);
  if(setMode==0)
    drawHandS(sDeg,sHandW,sHandL,sHandCol1,sHandCol2);
  sDegOld = sDeg;

  lcd.fillCircle(cx,cy, 4, RGBto565(40,40,40));
/*
  lcd.setTextSize(3); lcd.setTextColor(WHITE,BLACK);
  lcd.setCursor(0,0);
  lcd.print(hh); lcd.print(":"); lcd.print(mm); lcd.print(":"); lcd.print(ss);
  lcd.setCursor(0,30);
  lcd.print(cur.day); lcd.print("."); lcd.print(cur.month); lcd.print("."); lcd.print(cur.year+2000);
*/
}

// --------------------------------------------------------------------------
int stateOld = HIGH;
long btDebounce    = 30;
long btDoubleClick = 600;
long btLongClick   = 700;
long btLongerClick = 2000;
long btTime = 0, btTime2 = 0;
int clickCnt = 1;

// 0=idle, 1,2,3=click, -1,-2=longclick
int checkButton()
{
  int state = digitalRead(BUTTON);
  if( state == LOW && stateOld == HIGH ) { btTime = millis(); stateOld = state; return 0; } // button just pressed
  if( state == HIGH && stateOld == LOW ) { // button just released
    stateOld = state;
    if( millis()-btTime >= btDebounce && millis()-btTime < btLongClick ) { 
      if( millis()-btTime2<btDoubleClick ) clickCnt++; else clickCnt=1;
      btTime2 = millis();
      return clickCnt; 
    } 
  }
  if( state == LOW && millis()-btTime >= btLongerClick ) { stateOld = state; return -2; }
  if( state == LOW && millis()-btTime >= btLongClick ) { stateOld = state; return -1; }
  return 0;
}
// --------------------------------------------------------------------------
const char *months[] = {"???", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const char *delim = " :";
char bld[40];

uint8_t str2month(const char * d)
{
  uint8_t i = 13;
  while((--i) && strcmp(months[i], d));
  return i;
}

void setBuildTime()
{
  // Timestamp format: "Mar 3 2019 12:34:56"
  snprintf(bld,40,"%s %s\n", __DATE__, __TIME__);
  char *token = strtok(bld, delim);
  while(token) {
    int m = str2month((const char*)token);
    if(m>0) {
      cur.month = m;
      token = strtok(NULL, delim);  cur.day = atoi(token);
      token = strtok(NULL, delim);  cur.year = atoi(token) - 1970;
      token = strtok(NULL, delim);  cur.hour = atoi(token);
      token = strtok(NULL, delim);  cur.minute = atoi(token);
      token = strtok(NULL, delim);  cur.second = atoi(token);
    }
    token = strtok(NULL, delim);
  }
  //snprintf(bld,40,"Build: %02d-%02d-%02d %02d:%02d:%02d\n",mt.year+1970,mt.month,mt.day,mt.hour,mt.minute,mt.second); Serial.println(bld);
  setRTCTime(&cur);
}

//-----------------------------------------------------------------------------

void setup() 
{
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  Wire.begin();
  Wire.setClock(400000);  // faster
  lcd.init(SCR_WD, SCR_HT);
  //getRTCDateTime(&cur);
  if(cur.year+2000<2020) setBuildTime();  //  <2020 - invalid year
  uint16_t *pal = (uint16_t*)clockface+3;
  for(int i=0;i<4;i++) palette[i]=pgm_read_word(pal++);
  imgRect(0,0,SCR_WD,SCR_HT);
  ms = millis(); 
}

void loop()
{
  /*
  int st = checkButton();
  if(st<0 && setMode==0) setMode=1;
  if(setMode>0) {
    if(st>0) { start=1; if(++setMode>2) setMode=0; }
    if(setMode==1 && st<0) { if(++hh>23) hh=0; start=1; delay(600); }
    if(setMode==2 && st<0) { if(++mm>59) mm=0; start=1; delay(200); }
    cur.hour = hh;
    cur.minute = mm;
    setRTCTime(&cur);
  }*/
  clockUpdate();
}

