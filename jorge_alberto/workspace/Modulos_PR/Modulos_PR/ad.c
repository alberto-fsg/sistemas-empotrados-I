/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados
*******************************************************************
* nombre fichero : ad.c 
*******************************************************************
* descripcion    : Implementacion modulo conversor analogico -
*                  digital
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C
* fecha          : 18/01/2021
********************************************************************/


#include <msp430.h>
#include "ad.h"

#define A 0.0048876

void Init_AD (void)
{
    P5SEL0 |= BIT2 | BIT3 ;                // P5.2 -> A10, P5.3 -> A11
    P5SEL1 |= BIT2 | BIT3 ;

    ADCCTL0 |= ADCSHT_2 | ADCON;           // ADC ON, S&H=16 ADC clks

    ADCCTL1 |= ADCSSEL_3 | ADCDIV_3 | ADCSHP ;     // ADC CLK = SMCLK / 4, pulse mode

    ADCCTL2 &= ~ADCRES;                    // clear ADCRES in ADCCTL
    ADCCTL2 |= ADCRES_2;                   // 12-bit conversion

}

unsigned int Read_Value_Int_1 (void) 
{
    ADCMCTL0 |= ADCINCH_10 ;               // A10 ADC input select
    ADCCTL0 |= ADCENC | ADCSC;             // Sampling and conversion start
    while(!(ADCIFG&ADCIFG0));              // Espera fin conversion
    return ADCMEM0 ;                       // Lectura dato, borrado flag
}

unsigned int Read_Value_Int_2 (void) 
{
    ADCMCTL0 |= ADCINCH_11 ;               // A11 ADC input select
    ADCCTL0 |= ADCENC | ADCSC;             // Sampling and conversion start
    while(!(ADCIFG&ADCIFG0));              // Espera fin conversion
    return ADCMEM0 ;                       // Lectura dato, borrado flag
}

