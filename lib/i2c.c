/** 
 * --------------------------------------------------------------------------------------+
 * @desc        Two Wire Interface / I2C Communication
 * --------------------------------------------------------------------------------------+
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       06.09.2020
 * @file        twi.c
 * @tested      AVR Atmega16, ATmega8, Atmega328
 *
 * @depend      twi.h
 * --------------------------------------------------------------------------------------+
 * @usage       Master Transmit Operation
 */

// include libraries
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
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Calculation fclk:
  //
  // fclk = (fcpu)/(16+2*TWBR*4^Prescaler) m16
  // fclk = (fcpu)/(16+2*TWBR*Prescaler) m328p
  // -------------------------------------------------------------------------------------
  // Calculation TWBR:
  // 
  // TWBR = {(fcpu/fclk) - 16 } / (2*4^Prescaler)
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // @param1 value of TWBR (m16) 
  //  fclk = 400kHz; TWBR = 3
  //  fclk = 100kHz; TWBR = 20
  // @param1 value of TWBR (m328p)
  //  fclk = 400kHz; TWBR = 2
  // @param2 value of Prescaler = 1
  //TWI_FREQ (2, 1);

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
  // null status flag
  //TWI_TWSR &= ~0xA8;
  // START
  // -------------------------------------------------------------------------------------
  // request for bus
  //TWI_START();
  //TWI_TWCR |= (1 << 7); TWI_TWCR |= (1 << 6); TWI_TWCR |= (1 << 5);
  
  //TWI_TWCR = (1 << 7) | (1 << 6) | (1 << 5);
  //TWI_TWCR |= (1 << 7);
  //TWI_TWCR |= (1 << 6);
  //TWI_TWCR |= (1 << 5);
  IBCR |= (IBCR_TX_RX_MASK | IBCR_MS_SL_MASK);  // Start condition: master transmit mode
  IBSR |= IBSR_IBIF_MASK; // Reset interrupt bit
  
  // don't wait till flag set because setting start condition doesn't trigger interrupt on hcs12
  //TWI_WAIT_TILL_TWINT_IS_SET();
  // test if start or repeated start acknowledged
  /*if ((TWI_STATUS != TWI_START_ACK) && (TWI_STATUS != TWI_REP_START_ACK)) {
    // return status
    return TWI_STATUS;
  }*/
  // Also don't check ack for the same reason
  /*if (IBSR & IBSR_RXAK_MASK) {
      return IBSR;
  }*/
  // success
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
  // -------------------------------------------------------------------------------------
  // enable
  TWI_ENABLE(); // Set enable in control register, reset interrupt in status register
  //TWI_TWDR = (address << 1);
  IBDR = (address << 1);
  // wait till flag set
  TWI_WAIT_TILL_TWINT_IS_SET();

  // test if SLA with WRITE acknowledged
  /*if (TWI_STATUS != TWI_MT_SLAW_ACK) {
    // return status
    return TWI_STATUS;
  }*/

  if (IBSR & IBSR_RXAK_MASK) {
      return IBSR;
  }

  // success
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
  // -------------------------------------------------------------------------------------
  // enable
  TWI_ENABLE();
  //TWI_TWDR = data;
  IBDR = data;
  // wait till flag set
  TWI_WAIT_TILL_TWINT_IS_SET();

  // test if data acknowledged
  /*if (TWI_STATUS != TWI_MT_DATA_ACK) {
    // return status
    return TWI_STATUS;
  }*/

  if (IBSR & IBSR_RXAK_MASK) {
      return IBSR;
  }

  // success
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
  // -------------------------------------------------------------------------------------
  // enable
  TWI_ENABLE();
  //TWI_TWDR = (address << 1) | 0x01;
  IBDR = (address << 1) | 0x01;
  // wait till flag set
  TWI_WAIT_TILL_TWINT_IS_SET();

  // test if SLA with READ acknowledged
  /*if (TWI_STATUS != TWI_MR_SLAR_ACK) {
    // return status
    return TWI_STATUS;
  }*/

  // Treat read acknowledge same as write ackowledged?
  if (IBSR & IBSR_RXAK_MASK) {
      return IBSR;
  }

  // success
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
  // End TWI
  // -------------------------------------------------------------------------------------
  // send stop sequence
  //TWI_STOP ();
  //IBCR |= IBCR_IBEN_MASK;
  IBCR &= ~IBCR_MS_SL_MASK;
  IBSR |= IBSR_IBIF_MASK;
  // wait for TWINT flag is set
//  TWI_WAIT_TILL_TWINT_IS_SET();
}
