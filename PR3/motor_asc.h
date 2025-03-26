/******************************************************************************/
/*                                                                            */
/*                 Area Ingenieria de Sistemas y Automatica                   */
/*           Departamento de Informatica e Ingenieria de Sistemas             */
/*                         Universidad de Zaragoza                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* proyecto  : PRACTICAS SEP - SEI UNIZAR                                     */
/* fichero   : motor_asc.h                                                    */
/* version   : 1                                                              */
/* fecha     : 20/12/2021                                                     */
/* autor     : Jose Luis Villarroel                                           */
/* descripcion : Especificacion del modulo del motor del ascensor (MSP430)    */
/*                                                                            */
/******************************************************************************/

#ifndef MOTOR_ASC_H_
#define MOTOR_ASC_H_

/******************************************************************************/
/*                        Tipos de dato                                       */
/******************************************************************************/

typedef enum {SUBIR, BAJAR, PARAR} ORDEN ;


/******************************************************************************/
/*                        Servicios del modulo                                */
/******************************************************************************/

void Init_motor_asc (void) ;

void Orden_motor_asc (ORDEN O) ;

#endif /* MOTOR_ASC_H_ */
