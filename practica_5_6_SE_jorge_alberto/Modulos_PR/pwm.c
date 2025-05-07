/******************************************************************************/
/*                                                                            */
/*                 Area Ingenieria de Sistemas y Automatica                   */
/*           Departamento de Informatica e Ingenieria de Sistemas             */
/*                         Universidad de Zaragoza                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* proyecto  : PRACTICAS SEP - SEI UNIZAR                                     */
/* fichero   : pwm.c                                                          */
/* version   : 1                                                              */
/* fecha     : 16/01/2021                                                     */
/* autor     : Jose Luis Villarroel                                           */
/* descripcion : Implementacion del modulo PWM (MSP430)                       */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                        Modulos usados                                      */
/******************************************************************************/

#include <msp430.h>
#include "pwm.h"

/******************************************************************************/
/*                        Implementacion servicios                            */
/******************************************************************************/

void Init_PWM(void){
  
    P6DIR |= BIT2 | BIT3;                  // P6.2 and P6.3 output
    P6SEL0 |= BIT2 | BIT3;                 // P6.2 -> TB3.3, P6.3 -> TB3.4

    TB3CCR0 = 0x03FF ;                     // 10 bits PWM
    TB3CCTL3 = OUTMOD_6;                   // TBCCR3 toggle/set
    TB3CCR3 = 0x01FF ;                     // TB3.3 50%
    TB3CCTL4 = OUTMOD_2;                   // TBCCR4 toggle/reset
    TB3CCR4 = 0x01FF ;                     // TB3.4 50%

    TB3CTL = TBSSEL_2 | MC_1;              // SMCLK, up mode


}



void Set_Value_10b (unsigned int value)
{
   if (value > 0x03FF) value = 0x03FF ;
   TB3CCR3 = value ;
   TB3CCR4 = value ;
}
 
 
