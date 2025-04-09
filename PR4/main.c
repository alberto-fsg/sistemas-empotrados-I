/******************************************************************************/
/*                                                                            */
/*                 Area Ingenieria de Sistemas y Automatica                   */
/*           Departamento de Informatica e Ingenieria de Sistemas             */
/*                         Universidad de Zaragoza                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* proyecto  : PRACTICAS SEP - SEI UNIZAR                                     */
/* fichero   : main.c                                                          */
/* version   : 1                                                              */
/* fecha     : 22/02/2022                                                     */
/* autor     : Jose Luis Villarroel                                           */
/* descripcion : Practica PR4 (motor) (MSP430)                             */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                        Modulos usados                                      */
/******************************************************************************/

#include <msp430.h>
#include "InitSystem.h"
#include "clock.h"
#include "pwm.h"
#include "encoder.h"
#include "servos.h"
#include "ad.h"
#include "wcet.h"


/******************************************************************************/
/*                        Variables globales                                  */
/******************************************************************************/

unsigned int siguiente, periodo = 100 ;

float W, Wref = -1;

Timer_id Ti ;

float acum = 0;
unsigned int n_cum = 0;

float media_1 = 0, media_2 = 0, media_3 = 0;

int iter = 0;
float buffer[150];

// Variables globales de la practica

/******************************************************************************/
/*                        Prototipos funciones                                */
/******************************************************************************/

void Init_GPIO (void) ;

/******************************************************************************/
/*                        Main y funciones                                    */
/******************************************************************************/

float transformacion_lineal(int input) {
    return (input / 4095.0f) * 8.0f - 4.0f;
}

void actualizar_var(void) {
    switch((int)Wref) {
        case -1:
            Wref = 1.0f;

            media_1 = acum / n_cum;
            acum = 0; n_cum = 0;

            break;
        case 1:
            Wref = 3.0f;

            media_2 = acum / n_cum;
            acum = 0; n_cum = 0;

            break;
        default:
            Remove_Timer(Ti);
            Wref = 0.0f;

            media_3 = acum / n_cum;
            acum = 0; n_cum = 0;
            break;
    }
}

int main(void)
{

    periodo = 100;

    Stop_Watchdog () ;                  // Stop watchdog timer

    Init_CS () ;                        // MCLK = 8 MHz, SMCLK = 4 MHz
    Init_Clock () ;
    Init_GPIO () ;

    Init_PWM () ;
    Init_Encoder () ;
    Init_AD () ;

    Init_WCET();

    Init_Servos(periodo) ;

    siguiente = Get_Time () ;

    // PARTE 3
    Ti = Set_Timer(5000, PERIODIC, actualizar_var);

    while(1)
    {

        // PARTE 1
        //Set_Value_10b(Read_Value_Int_1());

        // PARTE 2
        /*Wref = transformacion_lineal(Read_Value_Int_1());
        W = velocity();
        R(Wref, W);*/

        // PARTE 3
        Start_measurement();

        W = velocity();
        R(Wref, W);

        buffer[iter] = W;
        acum += W;
        n_cum ++;
        iter++;

        End_measurement();

        // PERIÓDICO
        siguiente += periodo ;
        delay_until (siguiente) ;
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


