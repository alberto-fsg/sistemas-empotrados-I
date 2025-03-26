/******************************************************************************/
/*                                                                            */
/*                 Area Ingenieria de Sistemas y Automatica                   */
/*           Departamento de Informatica e Ingenieria de Sistemas             */
/*                         Universidad de Zaragoza                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* proyecto  : PRACTICAS SEP - SEI UNIZAR                                     */
/* fichero   : motor_asc.c                                                    */
/* version   : 1                                                              */
/* fecha     : 20/12/2021                                                     */
/* autor     : Jose Luis Villarroel                                           */
/* descripcion : Implementacion del modulo de reloj (MSP430)                  */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                        Modulos usados                                      */
/******************************************************************************/

#include <msp430.h>
#include "pwm.h"
#include "motor_asc.h"

/******************************************************************************/
/*                    Variables locales del modulo                            */
/******************************************************************************/

const unsigned int V_SUBIR = 909 ;        // Subir con alimentacion de 9V: 7V
const unsigned int V_BAJAR = 228 ;        // Bajar con alimentacion de 9V: -5V
const unsigned int V_PARAR = 512 ;        // Parar con alimentacion de 9V: 0V


/******************************************************************************/
/*                        Implementacion servicios                            */
/******************************************************************************/

void Init_motor_asc (void) {
    Init_PWM () ;
}

void Orden_motor_asc(ORDEN O)
{
/*  Manejo fines de carrera recorrido
 *  Descomentar con el ascensor conectado
*/
    /*if ((P1IN & BIT7) || (P3IN & BIT7)) {
        Set_Value_10b(V_PARAR);
        P4OUT &= ~(BIT4 | BIT6 | BIT7) ;
        P4OUT |= BIT4 | BIT6 | BIT7 ;
        return ;
    }*/

    switch (O)
    {
    case SUBIR:
        Set_Value_10b(V_SUBIR);
        P4OUT &= ~(BIT4 | BIT6 | BIT7) ;
        P4OUT |= BIT7 ;
        break;
    case BAJAR:
        Set_Value_10b(V_BAJAR);
        P4OUT &= ~(BIT4 | BIT6 | BIT7) ;
        P4OUT |= BIT6 ;
        break;
    case PARAR:
        Set_Value_10b(V_PARAR);
        P4OUT &= ~(BIT4 | BIT6 | BIT7) ;
        P4OUT |= BIT6 | BIT7 ;
        break;
    default:
        ;
    }
}
