int BCD2DEC(int x) { return ((x)>>4)*10+((x)&0xf); }
int DEC2BCD(int x) { return (((x)/10)<<4)+((x)%10); }

#if USEHW==1
#define I2CStart(x)   Wire.beginTransmission(x)
#define I2CStop()     Wire.endTransmission()
#define I2CWrite(x)   Wire.write(x)
#define I2CRead()     Wire.read()
#define I2CReadLast() Wire.read()
#define I2CReq(x,y)   Wire.requestFrom(x,y)
#define I2CReady      while(!Wire.available()) {};
#else
#define I2CStart(x)   i2c_start((x<<1) | I2C_WRITE)
#define I2CStop()     i2c_stop()
#define I2CWrite(x)   i2c_write(x)
#define I2CRead()     i2c_read(false)
#define I2CReadLast() i2c_read(true)
#define I2CReq(x,y)   i2c_rep_start((x<<1) | I2C_READ)
#define I2CReady
#endif

#define DS1307_I2C_ADDRESS 0x68  // same for DS3231
#define DS1307_TIME    0x00
#define DS1307_DOW     0x03
#define DS1307_DATE    0x04
#define DS1307_MEM     0x08

#define DS3231_I2C_ADDRESS 0x68
#define DS3231_CONTROL 0x0e
#define DS3231_STATUS  0x0f
#define DS3231_TEMP    0x11

/*
void setRTCDateTime(struct RTCData *data)
{
  I2CStart(DS1307_I2C_ADDRESS);
  I2CWrite(DS1307_TIME);
  I2CWrite(DEC2BCD(data->second));
  I2CWrite(DEC2BCD(data->minute));
  I2CWrite(DEC2BCD(data->hour));
  I2CWrite(DEC2BCD(data->dayOfWeek));
  I2CWrite(DEC2BCD(data->day));
  I2CWrite(DEC2BCD(data->month));
  I2CWrite(DEC2BCD(data->year)); // year 00..99
  I2CStop();
}
*/

void setRTCTime(struct RTCData *data)
{
  I2CStart(DS1307_I2C_ADDRESS);
  I2CWrite(DS1307_TIME);
  I2CWrite(DEC2BCD(data->second));
  I2CWrite(DEC2BCD(data->minute));
  I2CWrite(DEC2BCD(data->hour));
  I2CStop();
}

void getRTCDateTime(struct RTCData *data)
{
  I2CStart(DS1307_I2C_ADDRESS);
  I2CWrite(DS1307_TIME);
  I2CStop();

  I2CReq(DS1307_I2C_ADDRESS, 7);
  I2CReady;

  data->second    = BCD2DEC(I2CRead() & 0x7f);
  data->minute    = BCD2DEC(I2CRead() & 0x7f);
  data->hour      = BCD2DEC(I2CRead() & 0x3f);
  data->dayOfWeek = I2CRead() & 0x07;
  data->day       = BCD2DEC(I2CRead() & 0x3f);
  data->month     = BCD2DEC(I2CRead() & 0x3f);
  data->year      = BCD2DEC(I2CReadLast() & 0xff);

  I2CStop();
#if DEBUG_RTC==1
  //Serial.print(hour); Serial.print(":"); Serial.print(minute); Serial.print(":"); Serial.println(second);
  //Serial.print(day); Serial.print("-"); Serial.print(month); Serial.print("-"); Serial.println(year);
  //Serial.println(dayOfWeek);
#endif
}

float getDS3231Temp()
{
  I2CStart(DS3231_I2C_ADDRESS);
  I2CWrite(DS3231_TEMP);
  I2CStop();
  I2CReq(DS1307_I2C_ADDRESS, 2);
  I2CReady;
  int msb = I2CRead()<<2;
  int lsb = I2CRead()>>6;
  I2CStop();
  return (msb | lsb)/4.0;
}

void writeRTCReg(byte addr, byte val)
{
  I2CStart(DS3231_I2C_ADDRESS);
  I2CWrite(addr);
  I2CWrite(val);
  I2CStop();
}

// for DS1307 only
void writeRTCMem(byte addr, byte val)
{
  if(addr>56) return;
  I2CStart(DS1307_I2C_ADDRESS);
  I2CWrite(DS1307_MEM + addr);
  I2CWrite(val);
  I2CStop();
}

byte readRTCMem(byte addr)
{
  if(addr>56) return 0;
  I2CStart(DS1307_I2C_ADDRESS);
  I2CWrite(DS1307_MEM + addr);
  I2CStop();
  I2CReq(DS1307_I2C_ADDRESS, 1);
  I2CReady;
  byte v = I2CReadLast();
  I2CStop();
  return v;
}

