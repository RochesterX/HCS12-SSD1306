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

void LCD_SendByte(unsigned char data, unsigned char isChar) {
    // Set character/data
    if (isChar) {
        PORTK |= PORTK_BIT0_MASK;
    } else {
        PORTK &= ~PORTK_BIT0_MASK;
    }

    // Set write
    PORTK &= ~PORTK_BIT7_MASK;

    // Send data
    PORTK &= 0b11000011;                // Clear data range
    PORTK |= (data >> 2) & 0b00111100;  // Set upper nibble
    PORTK |= 0b00000010;                // Strobe high
    delay_us(3);                        // Wait
    PORTK &= ~0b00000010;               // Strobe low

    PORTK &= 0b11000011;                // Clear data range
    PORTK |= (data << 2) & 0b00111100;  // Set lower nibble
    PORTK |= 0b00000010;                // Strobe high
    delay_us(3);                        // Wait
    PORTK &= ~0b00000010;               // Strobe low
    
    // LCD processing time
    delay_us(50);
}

void LCD_WriteLine(const char* string) {
    while (*string != '\0') {
        LCD_SendByte(*(string++), 1);
    }
}

void LCD_SendCommands(const char* string) {
    while (*string != '\0') {
        LCD_SendByte(*(string++), 0);
    }
}

void LCD_Init() {
    delay_us(100000);

    LCD_SendByte(0x28, 0);
    LCD_SendByte(0x0F, 0);
    LCD_SendByte(0x06, 0);
    LCD_SendByte(0x01, 0);

    delay_us(2000);
}

void ByteToString(unsigned char value, char *str) {
    static const char hex_digits[] = "0123456789ABCDEF";

    str[0] = hex_digits[(value >> 4) & 0x0F]; // Upper nibble
    str[1] = hex_digits[value & 0x0F];        // Lower nibble
    str[2] = '\0';                             // Null terminator
}

void CheckStatus(unsigned char status) {
    char hexBuffer[3];
    if (status != SSD1306_SUCCESS) {
        LCD_Init();
        ByteToString(status, hexBuffer);
        LCD_WriteLine("Error: ");
        LCD_WriteLine(hexBuffer);

        for(;;) {
            _FEED_COP(); /* feeds the dog */
        } /* loop forever */
    }
}


void main(void) {
    char status;
    EnableInterrupts;

  // LCD INIT
  // -------------------------------------------------------------------------------------
  status = SSD1306_Init (SSD1306_ADDR);                                    // 0x3C
    CheckStatus(status);


  // DRAWING
  // -------------------------------------------------------------------------------------
  SSD1306_ClearScreen ();                                         // clear screen
  status = SSD1306_DrawLine (0, MAX_X, 4, 4);                              // draw line
    CheckStatus(status);
  SSD1306_SetPosition (7, 1);                                     // set position
  SSD1306_DrawString ("SSD1306 OLED DRIVER");                     // draw string
  status = SSD1306_DrawLine (0, MAX_X, 18, 18);                            // draw line
    CheckStatus(status);
  SSD1306_SetPosition (40, 3);                                    // set position
  SSD1306_DrawString ("Trevor Maze");                                // draw string
  SSD1306_SetPosition (53, 5);                                    // set position
  SSD1306_DrawString ("2026");                                    // draw string
  status = SSD1306_UpdateScreen (SSD1306_ADDR);                            // update
    CheckStatus(status);

  delay_us(1000000);
  status = SSD1306_InverseScreen (SSD1306_ADDR);
    CheckStatus(status);

  delay_us(1000000);
  status = SSD1306_NormalScreen (SSD1306_ADDR);
    CheckStatus(status);


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
} 

