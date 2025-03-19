/******************************************************************************/
/*                                                                            */
/*                 Area Ingenieria de Sistemas y Automatica                   */
/*           Departamento de Informatica e Ingenieria de Sistemas             */
/*                         Universidad de Zaragoza                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* proyecto  : PRACTICAS SEP - SEI UNIZAR                                     */
/* fichero   : display.c                                                      */
/* version   : 2                                                              */
/* fecha     : 19/02/2022                                                     */
/* autor     : Jose Luis Villarroel                                           */
/* descripcion : Implementacion modulo visualizacion 7 segmentos (MSP430)     */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                        Modulos usados                                      */
/******************************************************************************/

#include <msp430.h>
#include "display.h"

/******************************************************************************/
/*                        Implementacion servicios                            */
/******************************************************************************/

void Init_Display (void) {

    P3DIR |= BIT0 | BIT1 | BIT2 ;                            // D0, D1, D2
    P3DIR |= BIT4 | BIT5 | BIT6 | BIT7 ;                     // A, B, C, D
    P3DIR |= BIT3 ;                                          // sin uso
    P3OUT = 0x00 ;

} ;

void display (unsigned char N_Display, unsigned char Value) {

  P3OUT = (~(0x1 << N_Display)) & 0x07 | ((Value&0x0F) << 3) ;
  return ;

}

