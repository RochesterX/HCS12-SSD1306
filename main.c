#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "ssd1306.h"

void delay_us(unsigned long long us) {
    unsigned long long i;
    while (us--) {
        for (i = 0; i < 3; i++) {
            asm("nop");
        }
    }
}

void main(void) {
    char status;
    EnableInterrupts;

    // LCD Initialization
    status = SSD1306_Init(SSD1306_ADDR);                           // 0x3C
    CheckStatus(status);


    // Draw commands
    SSD1306_ClearScreen();                                         // clear screen
    SSD1306_DrawLine(0, MAX_X, 4, 4);                              // draw line
    SSD1306_SetPosition(7, 1);                                     // set position
    SSD1306_DrawString("SSD1306 OLED DRIVER");                     // draw string
    SSD1306_DrawLine(0, MAX_X, 18, 18);                            // draw line
    SSD1306_SetPosition(40, 3);                                    // set position
    SSD1306_DrawString("Trevor Maze");                             // draw string
    SSD1306_SetPosition(53, 5);                                    // set position
    SSD1306_DrawString("2026");                                    // draw string
    SSD1306_UpdateScreen(SSD1306_ADDR);                            // update

    delay_us(1000000);
    SSD1306_InverseScreen(SSD1306_ADDR);

    delay_us(1000000);
    SSD1306_NormalScreen(SSD1306_ADDR);


  for(;;) {
      _FEED_COP();
  }
} 

