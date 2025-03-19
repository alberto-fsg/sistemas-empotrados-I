/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados
*******************************************************************
* nombre fichero : encoder.h 
*******************************************************************
* descripcion    : Fichero de cabecera modulo encoder
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C
* fecha          : 17/01/2021
********************************************************************/

#ifndef encoder_h
#define encoder_h

void Init_Encoder (void) ;
   /* Inicializa el modulo encoder basado en el TB3 */

unsigned int Get_Counter (void) ;
   /* Devuelve el contado del encoder. 1 pulso = 0.5 grados */

#endif
