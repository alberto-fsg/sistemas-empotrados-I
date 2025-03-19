/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programables
*******************************************************************
* nombre fichero : encoder.c 
*******************************************************************
* descripcion    : Fichero de implementacion modulo encoder
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C
* fecha          : 17/01/2021
********************************************************************/

#include <msp430.h>
//#include "msp430JL.h"
#include "encoder.h"

static unsigned int Counter = 0 ;

unsigned int fs = 0 ;
unsigned int fb = 0 ;

void Init_Encoder (void) {

    P6SEL0 |= BIT0 ;                                  // P6.0 -> TB3.1
//    P6SEL0.bit.B0 = 1 ;                                  // P6.0 -> TB3.1

    TB3CCTL1 = CM_3 | CCIS__CCIA | CCIE | CAP ;       // Capture on both edges
                                                      // Capture CCIA: TB3.1 == P6.0
                                                      // Interrupt enable
 /*                                                     // Capyure mode
    TB3CCTL1.bit.CM = 0b11 ;
    TB3CCTL1.bit.CCIS = 0b00 ;
    TB3CCTL1.bit.CCIE = 0b1 ;
    TB3CCTL1.bit.CAP = 0b1 ;
*/

    if (!(TB3CTL & TBSSEL_2)) TB3CTL = TBSSEL_2 | MC_1;   // SMCLK, up mode
    __enable_interrupt();

}

unsigned int Get_Counter (void) {

   unsigned int Counter_Copy ;
   
   __disable_interrupt();
   Counter_Copy = Counter ;
   __enable_interrupt();
   return Counter_Copy ;

}

#pragma vector = TIMER3_B1_VECTOR
__interrupt void Pulse (void) {

   if (TB3IV) {
   if (P6IN&BIT0) {
       fs ++ ;
      if (P6IN&BIT1) Counter -- ;
      else Counter ++ ;
   } else {
       fb ++ ;
      if (P6IN&BIT1) Counter ++ ;
      else Counter -- ;
   }
   }

   return ;
}
   
