/******************************************************************************/
/*                                                                            */
/*                 Area Ingenieria de Sistemas y Automatica                   */
/*           Departamento de Informatica e Ingenieria de Sistemas             */
/*                         Universidad de Zaragoza                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* proyecto  : PRACTICAS SEP - SEI UNIZAR                                     */
/* fichero   : pwm.h                                                          */
/* version   : 1                                                              */
/* fecha     : 16/01/2021                                                     */
/* autor     : Jose Luis Villarroel                                           */
/* descripcion : Especificacion del modulo PWM (MSP430)                       */
/*                                                                            */
/******************************************************************************/

#ifndef pwm_h
#define pwm_h

/******************************************************************************/
/*                        Servicios del modulo                                */
/******************************************************************************/

void Init_PWM (void) ;
   /* Inicializa el TB para generar un PWM (TB3.3) y su negado (TB3.4) */

void Set_Value_10b (unsigned int value) ;
   /* Establece la anchura de pulso 0..1023 */

#endif
