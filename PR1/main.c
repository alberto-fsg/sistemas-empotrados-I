/******************************************************************
* Area Ingenieria de Sistemas y 
* Automatica y Arquitectura y tecnologia de Computadores
* Sistemas Empotrados 1
*******************************************************************
* nombre fichero : main.c
*******************************************************************
* descripcion    : Practica PR1 comprobación placa
* programador    : JL Villarroel (JLV) & Enrique Torres
* lenguaje       : ANSI C
* fecha          : 8/03/2022
********************************************************************/

/******************************************************************
*                   Modulos usados
*******************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <msp430.h>
#include "InitSystem.h"
#include "display.h"
#include "clock.h"

/******************************************************************
*                   Variables compartidas
*******************************************************************/

static volatile unsigned char pulso_flag = 0;
static volatile unsigned char boton_pulsado = 0xFF;

/******************************************************************
*                   Prototipos funciones
*******************************************************************/
void Init_GPIO (void) ;

/******************************************************************
*                   Main y funciones
*******************************************************************/

typedef struct {
    volatile unsigned char* PIN[7];
    unsigned char BIT[7];
} LEDS; // tipo de datos para manejar los LEDs en bucles

typedef struct {
    volatile unsigned char* PIN[6];
    unsigned char BIT[6];
    unsigned char antes_pulsado[6];
} PULSADORES; // tipo de datos para manejar los pulsadores en bucles

static LEDS leds;
static PULSADORES pulsadores;

void iniciar_leds(void) {
    leds.PIN[0] = &P2OUT;
    leds.PIN[1] = &P2OUT;
    leds.PIN[2] = &P2OUT;
    leds.PIN[3] = &P4OUT;
    leds.PIN[4] = &P4OUT;
    leds.PIN[5] = &P4OUT;
    leds.PIN[6] = &P4OUT;

    leds.BIT[0] = BIT0;
    leds.BIT[1] = BIT1;
    leds.BIT[2] = BIT2;
    leds.BIT[3] = BIT4;
    leds.BIT[4] = BIT5;
    leds.BIT[5] = BIT6;
    leds.BIT[6] = BIT7;
}

void iniciar_pulsadores (void) {
    unsigned char j;
    for(j = 0; j < 6; ++j) {
        pulsadores.PIN[j] = &P1IN;
    }

    pulsadores.BIT[0] = BIT1;
    pulsadores.BIT[1] = BIT2;
    pulsadores.BIT[2] = BIT3;
    pulsadores.BIT[3] = BIT4;
    pulsadores.BIT[4] = BIT5;
    pulsadores.BIT[5] = BIT6;

    for(j = 0; j < 6; ++j) {
        pulsadores.antes_pulsado[j] = 0;
    }
}


void prueba_leds (void) {
    unsigned char i;
    for (i = 0; i < 7; i ++) {
        *leds.PIN[i] |= leds.BIT[i];
        delay_until(Get_Time() + 1000);
    }
}


void prueba_pulsadores (void) {
    unsigned char j;

    while(1) {
        for (j = 0; j < 6; j ++) {
            if (*pulsadores.PIN[j] & pulsadores.BIT[j])
                *leds.PIN[j] |= leds.BIT[j] ;
            else *leds.PIN[j] &= ~leds.BIT[j] ;

            if ((*pulsadores.PIN[j] & pulsadores.BIT[j]) && !pulsadores.antes_pulsado[j]) {//detector de flancos
                pulsadores.antes_pulsado[j] = 1 ;
            }
            else if (!(*pulsadores.PIN[j] & pulsadores.BIT[j])) pulsadores.antes_pulsado[j] = 0 ;
        }
    }
}

void prueba_seven_segment_display (void) {
    static unsigned char k;
    static unsigned char d1=0, d2=0, d3=0;
    while(1)
    {
        for(k = 0; k < 6; ++k) {
            if ((*pulsadores.PIN[k] & pulsadores.BIT[k]) && !pulsadores.antes_pulsado[k]) {//detector de flancos
                pulsadores.antes_pulsado[k] = 1 ;
                if(k == 0) d3 = (d3 == 0) ? 9 : d3 - 1;
                else if(k == 1) d3 = (d3 == 9) ? 0 : d3 + 1;
                else if(k == 2) d2 = (d2 == 0) ? 9 : d2 - 1;
                else if(k == 3) d2 = (d2 == 9) ? 0 : d2 + 1;
                else if(k == 4) d1 = (d1 == 0) ? 9 : d1 - 1;
                else if(k == 5) d1 = (d1 == 9) ? 0 : d1 + 1;
            }
            else if (!(*pulsadores.PIN[k] & pulsadores.BIT[k])) pulsadores.antes_pulsado[k] = 0 ;

            display (0, d1) ;
            display (1, d2) ;
            display (2, d3) ;

            if (pulso_flag > 0){
                pulso_flag = 0;     // OjO, secion critica?
                //...
            }
        }
    }
}

void prueba_seven_segment_display_interrupcion(void) {
    static unsigned char d1 = 0, d2 = 0, d3 = 0;
    while (1) {
        if (boton_pulsado != 0xFF) {
            unsigned char btn = boton_pulsado;
            boton_pulsado = 0xFF;
            if (btn == 0)
                d3 = (d3 == 0) ? 9 : d3 - 1;
            else if (btn == 1)
                d3 = (d3 == 9) ? 0 : d3 + 1;
            else if (btn == 2)
                d2 = (d2 == 0) ? 9 : d2 - 1;
            else if (btn == 3)
                d2 = (d2 == 9) ? 0 : d2 + 1;
            else if (btn == 4)
                d1 = (d1 == 0) ? 9 : d1 - 1;
            else if (btn == 5)
                d1 = (d1 == 9) ? 0 : d1 + 1;
        }
        display(0, d1);
        display(1, d2);
        display(2, d3);
    }
}

int main(void)
{
    static unsigned char antes_pulsado = 0, j = 0 ;

    Stop_Watchdog () ;                  // Stop watchdog timer

    Init_CS () ;                        // MCLK = 8 MHz, SMCLK = 4 MHz
    Init_GPIO () ;
    Init_Display () ;

    P1IE |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6;  // Int enabled para todos los botones
    P1IES |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6; // Int raising edge para todos los botones

    Init_Clock ();
    iniciar_leds ();
    iniciar_pulsadores ();

    prueba_leds ();
    // prueba_pulsadores ();
    // prueba_seven_segment_display();
    // prueba_seven_segment_display_interrupcion();

    while(1)
    {
        if (P1IN & BIT1)
            P2OUT |= BIT0 ;
        else P2OUT &= ~BIT0 ;

        if ((P1IN & BIT1) && !antes_pulsado) {//detector de flancos
            antes_pulsado = 1 ;
            j = (j + 1)%10 ;
        }
        else if (!(P1IN & BIT1)) antes_pulsado = 0 ;

        display (0, j) ;

        if (pulso_flag > 0){
            pulso_flag = 0;     // OjO, secion critica?
            //...
        }
    }
}

void Init_GPIO (void) {
    // P2.6,P2.7: XIN, XOUT, reloj externo
    // P1.0 salida, led de la tarjeta
    // P6.6 salida, led de la tarjeta
    // P1.1 .. P1.6 entradas, pulsadores del ascensor
    // P3 control display 7 segmentos
    // P6.0 y P6.1 --> entradas encoder
    // P6.2 y P6.3 --> salidas PWM
    // resto: sin uso

    P1DIR |= BIT0 ;                                          // led de la tarjeta
    // P1.1 - P1.6 entradas para pulsadores
    P1REN |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 ;       // pull-down/pull-up
    P1OUT |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 ;       // pull-up
    P1DIR |= BIT7 ;                                          // sin uso
    // P2.0 - P2.2 salidas para led
    P2DIR |= BIT0 | BIT1 | BIT2  ;                           // salida led
    P2DIR |= BIT3 | BIT4 | BIT5 ;                            // sin uso
    P2OUT = 0x00 ;                                           // sin uso
    // P3 control display 7 segmentos
    P3DIR |= BIT0 | BIT1 | BIT2 ;                            // D0, D1, D2
    P3DIR |= BIT3 | BIT4 | BIT5 | BIT6 ;                     // A, B, C, D
    P3DIR |= BIT7 ;                                          // sin uso
    P3OUT = 0x00 ;
    // P4.5 - P4.7 salidas para led
    P4DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 ;              // sin uso
    P4DIR |= BIT5 | BIT6 | BIT7 ;                            // salida led
    P4OUT = 0x00 ;
    // P5.2 - P5.3 ADC
    P5DIR |= BIT0 | BIT1 | BIT4 ;                            // sin uso
    P5OUT = 0x00 ;
    // P6.0 - P6.1 entradas encoder
    // P6.2 - P6.3 salidas PWM
    P6DIR |= BIT2 | BIT3 ;                                   // PWM
    P6DIR |= BIT4 | BIT5 ;                                   // sin uso
    P6DIR |= BIT6 ;                                          // led de la tarjeta
    P6OUT = 0x00 ;


    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
}

#pragma vector = PORT1_VECTOR
__interrupt void RSI_Pulso(void) {
    unsigned char k;
    for (k = 0; k < 6; k++) {
        if (P1IFG & pulsadores.BIT[k]) {
            if (!(*pulsadores.PIN[k] & pulsadores.BIT[k]))
                boton_pulsado = k;
        }
    }
    P1IFG &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6);
    return;
}
