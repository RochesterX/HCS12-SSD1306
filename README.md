# SSD1306 0.96" OLED I2C Driver for HCS12

## SSD1306 Description
Detailed information is available in the [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf).

## Library
This C library is designed to drive a 0.96" OLED display with the SSD1306 driver (both the 128x64 and 128x32 versions) through I2C (IIC).

Version-specific settings:
  - 128x64 version
    - command argument **SSD1306_SET_MUX_RATIO** set to *0x3F* (ssd1306.c)
    - command argument **SSD1306_COM_PIN_CONF** set to *0x12*  (ssd1306.c)
    - **END_PAGE_ADDR** set to 7 (ssd1306.h)
  - 128x32 version
    - command argument **SSD1306_SET_MUX_RATIO** set to *0x1F* (ssd1306.c)
    - command argument **SSD1306_COM_PIN_CONF** set to *0x02* (ssd1306.c)
    - **END_PAGE_ADDR** set to 3 (ssd1306.h)

### Versions
- 1.0 - basic functions. The first publication.
- [2.0](https://github.com/Matiasus/SSD1306/tree/v2.0.0) - more changes: 
  - rebuild to 'cacheMemLcd' array approach. It means that every request is stored in 'cacheMemLcd' array (RAM) and by the [SSD1306_UpdateScreen (uint8_t)](#ssd1306_updatescreen) function is printed on the display.
  - added new function -> [SSD1306_DrawLine (uint8_t, uint8_t, uint8_t, uint8_t)](#ssd1306_drawline). Possible depicted any line (horizontal, vertical, with slope).
  - possible to use for more than 1 display (not tested). 
  - **!!!** ~1kB RAM memory consumption.
- [3.0](https://github.com/Matiasus/SSD1306/tree/v3.0.1) - simplified alphanumeric version
  - displaying alphanumeric characters
  - for **one display** applicable
  - **only few RAM bytes** consumption
  - **!!!** no graphic functions like drawLine
- [3.1](https://github.com/Matiasus/SSD1306/tree/v3.1.0) - simplified version with draw lines
  - displaying alphanumeric characters with graphic functions like drawLine vertical & horizontal
  - for **one display** applicable
  - **only few RAM bytes** consumption
  - horizontal scroll function added
- [HCS12 Port](https://github.com/RochesterX/HCS12-SSD1306)
  - Ported `twi.c` and `twi.h` to `i2c.c` and `i2c.h` for use with HCS12 chips in CodeWarrior 5.1.

## Dependencies
- [font.h](https://github.com/RochesterX/HCS12-SSD1306/blob/master/lib/font.h)
- [i2c.c](https://github.com/RochesterX/HCS12-SSD1306/blob/master/lib/i2c.c)
- [i2c.h](https://github.com/RochesterX/HCS12-SSD1306/blob/master/lib/i2c.h)

Font.c can be modified according to application requirements with form defined in font.c. Maximal permissible horizontal dimension is 8 bits.

### Usage
This fork has been designed for use with the Motorola/Freescale/NXP HCS12 series of chips.

### Tested
This fork was tested with **_0.96″ 128x64 Adafruit OLED Display (SSD1306 driver)_** and **Dragon12 Light Trainer Rev. D**. The Dragon12 board was equipped with a `MC9S12DG256` and the D-Bug12 bootloader. The software was designed for use with Freescale CodeWarrior 5.1. Communication utilized the I2C pins on Port J via the chip's IIC controller. This hardware configuration was dictated by education material requirements.

## Functions
- [SSD1306_Init (uint8_t)](#ssd1306_init) - Init display
- [SSD1306_ClearScreen (void)](#ssd1306_clearscreen) - Clear screen
- [SSD1306_NormalScreen (uint8_t)](#ssd1306_normalscreen) - Normal screen
- [SSD1306_InverseScreen (uint8_t)](#ssd1306_inversescreen) - Inverse screen
- [SSD1306_SetPosition (uint8_t, uint8_t)](#ssd1306_setposition) - Set position
- [SSD1306_DrawChar (char)](#ssd1306_drawchar) - Draw specific character
- [SSD1306_DrawString (char*)](#ssd1306_drawstring) - Draw specific string
- [SSD1306_UpdateScreen (uint8_t)](#ssd1306_updatescreen) - Update content on display
- [SSD1306_DrawLine (uint8_t, uint8_t, uint8_t, uint8_t)](#ssd1306_drawline) - Draw line

## Acknowledgement
- [Original Project Repo](https://github.com/Matiasus/SSD1306)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)

## Links
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [HCS12 Datasheet](https://docs.rochesterx.dev/Dragon12/HCS12%20%28S12CPU15UG_D%29.pdf)
- [HCS12 IIC Block User Guide](https://docs.rochesterx.dev/Dragon12/modules/I2C%20%20%20%20%20%20%20%28S12IICV2_D%29.pdf)
- [Additional Dragon12/HCS12 Documentation](https://docs.rochesterx.dev/Dragon12/)

## Init OLED Sequence
The OLED display init sequence was defined mainly according to page 64 (next to last page) of the [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf).

### Flowchart
```
// +---------------------------+
// |      Software Reset       |  // not tested yet, @source https://github.com/SmingHub/Sming/issues/501
// +---------------------------+
// |           0xE4            |
// +---------------------------+
//              ||
// +---------------------------+
// |        DISPLAY OFF        |
// +---------------------------+
// |           0xAE            |
// +---------------------------+
//              ||
// +---------------------------+
// |       Set MUX Ratio       |
// +---------------------------+
// |           0xA8            |
// |           0x3F            |
// +---------------------------+
//              ||
// +---------------------------+
// |   Set Memory Addr  Mode   |
// +---------------------------+
// |           0x20            |
// |           0x00            |
// +---------------------------+
//              ||
// +---------------------------+
// |      Set Start Line       |
// +---------------------------+
// |           0x40            |
// +---------------------------+
//              ||
// +---------------------------+
// |    Set Display Offset     |
// +---------------------------+
// |           0xD3            |
// |           0x00            |
// +---------------------------+
//              ||
// +---------------------------+
// |     Set Segment Remap     |
// +---------------------------+
// |       0xA0 or 0xA1        |
// +---------------------------+
//              ||
// +---------------------------+
// |   Set COM Output Scan     |
// |        Direction          |
// +---------------------------+
// |       0xC0 or 0xC8        |
// +---------------------------+
//              ||
// +---------------------------+
// |   Set COM Pins hardware   |
// |       configuration       |
// +---------------------------+
// |           0xDA            |
// |      0x12 for 128x64      |
// |      0x02 for 128x32      |
// +---------------------------+
//              ||
// +---------------------------+
// |   Set Contrast Control    |
// +---------------------------+
// |           0x81            |
// |           0x7F            |
// +---------------------------+
//              ||
// +---------------------------+
// | Disable Entire Display On |
// +---------------------------+
// |           0xA4            |
// +---------------------------+
//              ||
// +---------------------------+
// |    Set Normal Display     |
// +---------------------------+
// |           0xA6            |
// +---------------------------+
//              ||
// +---------------------------+
// |  Set OSC Frequency Fosc   |
// +---------------------------+
// |           0xD5            |
// |           0x80            |
// +---------------------------+
//              ||
// +---------------------------+
// |     Enable charge pump    |
// |         regulator         |
// +---------------------------+
// |           0x8D            |
// |           0x14            |
// +---------------------------+
//              ||
// +---------------------------+
// |     Deactivate Scroll     |
// +---------------------------+
// |           0x2E            |
// +---------------------------+
//              ||
// +---------------------------+
// |        Display On         |
// +---------------------------+
// |           0xAF            |
// +---------------------------+
```
