# Arduino_ST7789_Fast
Fast SPI library for the ST7789 240x240 IPS display

YouTube video: https://youtu.be/GciLKcWQZK4

Significantly optimized for 16MHz AVR Arduino boards (2.5-5x faster than other libraries)

Achieved 6.9Mbps SPI transfer rate (at DIV2/16MHz clock)

## Configuration

Use "define COMPATIBILITY_MODE" - then the library should work on all Arduino compatible boards
Remove above for the best performance on 16MHz AVR

Use "#define CS_ALWAYS_LOW" for LCD boards where CS pin is internally connected to the ground, it gives better performance

## Extra Features
- invertDisplay()
- sleepDisplay()
- enableDisplay()
- idleDisplay() - saves power by limiting colors to 3 bit mode (8 colors)
- resetDisplay() - software reset
- partialDisplay() and setPartArea() - limiting display area for power saving
- setScrollArea() and setScroll(uint16_t vsp) - smooth vertical scrolling
- fast drawImage() from RAM
- fast drawImage() from flash (PROGMEM)

## Connections:

|LCD pin|LCD pin name|Arduino|
|--|--|--|
 |#01| GND| GND|
 |#02| VCC |VCC (3.3V only!)|
 |#03| SCL |D13/SCK|
 |#04| SDA|D11/MOSI|
 |#05| RES|D8 or any digital|
 |#06| DC|D7 or any digital|
 |#07| BLK | NC|
