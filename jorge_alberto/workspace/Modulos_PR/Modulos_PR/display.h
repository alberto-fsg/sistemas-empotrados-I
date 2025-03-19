/******************************************************************************/
/*                                                                            */
/*                 Area Ingenieria de Sistemas y Automatica                   */
/*           Departamento de Informatica e Ingenieria de Sistemas             */
/*                         Universidad de Zaragoza                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* proyecto  : PRACTICAS SEP - SEI UNIZAR                                     */
/* fichero   : display.h                                                      */
/* version   : 2                                                              */
/* fecha     : 19/02/2022                                                     */
/* autor     : Jose Luis Villarroel                                           */
/* descripcion : Modulo visualizacion 7 segmentos (MSP430)                    */
/*                                                                            */
/******************************************************************************/


#ifndef DISPLAY_H_
#define DISPLAY_H_


/******************************************************************************/
/*                        Servicios del modulo                                */
/******************************************************************************/

void Init_Display (void) ;

void display (unsigned char N_Display, unsigned char Value) ;


#endif /* DISPLAY_H_ */
