// Includes
#include "i2c.h"

/**
 * @desc    TWI init - initialize frequency
 *
 * @param   void
 *
 * @return  void
 */
void TWI_Init (void)
{
    IBCR = IBCR_IBEN_MASK;
    IBFD = 0b00100000; // 0x20
}

/**
 * @desc    TWI MT Start
 *
 * @param   void
 *
 * @return  char
 */
char TWI_MT_Start (void)
{
  IBCR |= (IBCR_TX_RX_MASK | IBCR_MS_SL_MASK);  // Start condition: master transmit mode
  IBSR |= IBSR_IBIF_MASK;                       // Reset interrupt bit
                                                //
  return SUCCESS;
}

/**
 * @desc    TWI Send address + write
 *
 * @param   char
 *
 * @return  char
 */
char TWI_MT_Send_SLAW (char address)
{
  // SLA+W
  TWI_ENABLE(); // Set enable in control register, reset interrupt in status register

  IBDR = (address << 1);

  TWI_WAIT_TILL_TWINT_IS_SET();

  if (IBSR & IBSR_RXAK_MASK) {
      return IBSR;
  }

  return SUCCESS;
}

/**
 * @desc    TWI Send data
 *
 * @param   char
 *
 * @return  char
 */
char TWI_MT_Send_Data (char data)
{
  // DATA
  TWI_ENABLE();

  IBDR = data;

  TWI_WAIT_TILL_TWINT_IS_SET();

  if (IBSR & IBSR_RXAK_MASK) {
      return IBSR;
  }

  return SUCCESS;
}

/**
 * @desc    TWI Send address + read
 *
 * @param   char
 *
 * @return  char
 */
char TWI_MR_Send_SLAR (char address)
{
  // SLA+R
  TWI_ENABLE();

  IBDR = (address << 1) | 0x01;

  TWI_WAIT_TILL_TWINT_IS_SET();

  if (IBSR & IBSR_RXAK_MASK) {
      return IBSR;
  }

  return SUCCESS;
}

/**
 * @desc    TWI stop
 *
 * @param   void
 *
 * @return  void
 */
void TWI_Stop (void)
{
  // Send stop condition
  IBCR &= ~IBCR_MS_SL_MASK;
  IBSR |= IBSR_IBIF_MASK;
}

