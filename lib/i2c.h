#ifndef __TWI_H__
#define __TWI_H__

  // Includes
  #include <hidef.h>            // CodeWarrior: Common defines and macros
  #include "derivative.h"       // CodeWarrior: Derivative-specific definitions


  // Success
  #ifndef SUCCESS
    #define SUCCESS             0
  #endif

  // Error
  #ifndef ERROR
    #define ERROR               1
  #endif 


  // TWI enable
  #define TWI_ENABLE()                  { IBSR = IBSR_IBIF_MASK; }

  // TWI test if TWINT Flag is set
  #define TWI_WAIT_TILL_TWINT_IS_SET()  { while (!(IBSR & IBSR_IBIF_MASK)); }

  
  /**
   * @desc    TWI init
   *
   * @param   void
   *
   * @return  void
   */
  void TWI_Init (void);

  /**
   * @desc    TWI MT Start
   *
   * @param   void
   *
   * @return  char
   */
  char TWI_MT_Start (void);

  /**
   * @desc    TWI Send SLAW
   *
   * @param   void
   *
   * @return  unsigned char
   */
  char TWI_MT_Send_SLAW (char);

  /**
   * @desc    TWI Send data
   *
   * @param   char
   *
   * @return  char
   */
  char TWI_MT_Send_Data (char);

  /**
   * @desc    TWI Send SLAR
   *
   * @param   void
   *
   * @return  unsigned char
   */
  char TWI_MR_Send_SLAR (char);

  /**
   * @desc    TWI stop
   *
   * @param   void
   *
   * @return  void
   */
  void TWI_Stop (void);
  
#endif
