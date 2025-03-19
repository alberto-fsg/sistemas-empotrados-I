/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programables
*******************************************************************
* nombre fichero : clock.h 
*******************************************************************
* descripcion    : Fichero de cabecera modulo reloj de sistema
*                  Tick del reloj 1 ms
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CCS
* fecha          : 14/01/2021
********************************************************************/

#ifndef clock_h
#define clock_h

#define PERIODIC 1
#define ONE_SHOT 0

typedef unsigned char Timer_id ;
   /* Indice de timer. Puede haber 0..7 */
typedef unsigned char Timer_type ;
   /* PERIODIC or ONE_SHOT */

void Init_Clock (void) ;
   /* Instalacion del reloj y puesta a cero*/
   
unsigned int Get_Time (void) ;
   /* Obtención de la cuenta del reloj*/
   
void delay_until(unsigned int T);
    /* Espera bloqueante hasta que la cuenta del reloj llegue
       a T */
       
Timer_id Set_Timer (unsigned int Ticks, Timer_type tt, void (*p)(void)) ;
    /* Arranque de un timer avisa en t+Ticks y ejecuta p */

char Time_Out (Timer_id n) ;
    /* Consulta si el timer ha expirado */

char Is_Active (Timer_id n) ;
/* Consulta si el timer esta activo */

void Remove_Timer (Timer_id n) ;
    /* Elimina un timer */

void Clear_Timer(Timer_id n) ;
    /* Borra la bandera de time out */


#endif
