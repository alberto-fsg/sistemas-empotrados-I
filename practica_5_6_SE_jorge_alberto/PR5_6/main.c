#include <msp430.h>
#include "InitSystem.h"
#include "clock.h"
#include "pwm.h"
#include "encoder.h"
#include "servos.h"
#include "ad.h"

#include <msp430.h>
#include "InitSystem.h"
#include "clock.h"
#include "display.h"

#define START  BIT1
#define S1     BIT5
#define S2     BIT6

static Timer_id ti; // Timer entre estados temporizados durante el lavado
static unsigned char current_display = 0; // Para multiplexar el display

static enum {
    P1,
    P1_5,
    P1_4,
    P1_3,
    P1_2,
    P1_1,

    P2,
    P2_4,
    P2_3,
    P2_2,
    P2_1,
} ESTADO_LAVADORA = P1;

float transformacion_lineal(float input) {
    return (input / 4095.0f) * 8.0f - 4.0f;
}

static unsigned char LeerEntrada(void) {
  return (P1IN)&0x7E;
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
    // P1.1 - P1.7 entradas para pulsadores
    P1REN |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 ;// pull-down/pull-up
    P1OUT |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 ;       // pull-up
    P1OUT &= ~BIT7 ;                                         // pull-down
    // P2.0 - P2.2 salidas para led
    P2DIR |= BIT0 | BIT1 | BIT2  ;                           // salida led
    P2DIR |= BIT3 | BIT4 | BIT5 ;                            // sin uso
    P2OUT = 0x00 ;                                           // sin uso
    // P3 control display 7 segmentos
    P3DIR |= BIT0 | BIT1 | BIT2 ;                            // D0, D1, D2
    P3DIR |= BIT3 | BIT4 | BIT5 | BIT6 ;                     // A, B, C, D
    P3REN |= BIT7 ;                                          // pull-down/pull-up
    P3OUT &= ~BIT7 ;                                         // pull-down
    P3OUT = 0x00 ;
    // P4.5 - P4.7 salidas para led
    P4DIR |= BIT0 | BIT1 | BIT2 | BIT3  ;                    // sin uso
    P4DIR |= BIT4 | BIT5 | BIT6 | BIT7 ;                     // salida led
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

 void visualizador(void) {
    P4OUT ^= BIT5; // Subida onda cuadrada tarea visualización

    switch(ESTADO_LAVADORA) {

    case P1:
        if (current_display == 0) {
            display(0, 1);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 0);
            current_display = 0;
        }
        break;

    case P1_5:
        if (current_display == 0) {
            display(0, 1);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 5);
            current_display = 0;
        }
        break;

    case P1_4:
        if (current_display == 0) {
            display(0, 1);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 4);
            current_display = 0;
        }
        break;

    case P1_3:
        if (current_display == 0) {
            display(0, 1);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 3);
            current_display = 0;
        }
        break;

    case P1_2:
        if (current_display == 0) {
            display(0, 1);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 2);
            current_display = 0;
        }
        break;

    case P1_1:
        if (current_display == 0) {
            display(0, 1);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 1);
            current_display = 0;
        }
        break;

    case P2:
        if (current_display == 0) {
            display(0, 2);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 0);
            current_display = 0;
        }
        break;

    case P2_4:
        if (current_display == 0) {
            display(0, 2);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 4);
            current_display = 0;
        }
        break;

    case P2_3:
        if (current_display == 0) {
            display(0, 2);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 3);
            current_display = 0;
        }
        break;

    case P2_2:
        if (current_display == 0) {
            display(0, 2);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 2);
            current_display = 0;
        }
        break;

    case P2_1:
        if (current_display == 0) {
            display(0, 2);
            current_display = 1;
        }
        else if (current_display == 1) {
            display(1, 1);
            current_display = 0;
        }
        break;

    default:
        break;
    }

    P4OUT ^= BIT5; // Bajada onda cuadrada tarea visualización
}

static unsigned char entrada;

void automata (void) {
    P4OUT ^= BIT6; // Subida onda cuadrada tarea control discreto

    entrada = LeerEntrada(); // Asumo lectura destructiva

    switch(ESTADO_LAVADORA) {

    case P1:
        P2OUT &= ~BIT2; // Apagar led 2.2
        P2OUT |= BIT1; // Encender led 2.1
        if (entrada & S2) {
            ESTADO_LAVADORA = P2;
        }
        else if (entrada & START) {
            P2OUT &= ~BIT1; // Apagar led 2.1
            P2OUT |= BIT0; // Encender led 2.0
            ESTADO_LAVADORA = P1_5;
            Remove_Timer(ti);
            ti = Set_Timer(4000, ONE_SHOT, ((void*)0));
        }
        break;

    case P1_5:
        if (Time_Out(ti)) {
            ESTADO_LAVADORA = P1_4;
            Remove_Timer(ti);
            ti = Set_Timer(4000, ONE_SHOT, ((void*)0));
        }
        break;

    case P1_4:
        if (Time_Out(ti)) {
            ESTADO_LAVADORA = P1_3;
            Remove_Timer(ti);
            ti = Set_Timer(4000, ONE_SHOT, ((void*)0));
        }
        break;

    case P1_3:
        if (Time_Out(ti)) {
            ESTADO_LAVADORA = P1_2;
            Remove_Timer(ti);
            ti = Set_Timer(4000, ONE_SHOT, ((void*)0));
        }
        break;

    case P1_2:
        if (Time_Out(ti)) {
            ESTADO_LAVADORA = P1_1;
            Remove_Timer(ti);
            ti = Set_Timer(4000, ONE_SHOT, ((void*)0));
        }
        break;

    case P1_1:
        if (Time_Out(ti)) {
            P2OUT &= ~BIT0; // Apagar led 2.0
            ESTADO_LAVADORA = P1;
            Remove_Timer(ti);
        }
        break;

    case P2:
        P2OUT &= ~BIT1; // Apagar led 2.1
        P2OUT |= BIT2; // Encender led 2.2
        if (entrada & S1) {
            ESTADO_LAVADORA = P1;
        }
        else if (entrada & START) {
            P2OUT &= ~BIT2; // Apagar led 2.2
            P2OUT |= BIT0; // Encender led 2.0
            ESTADO_LAVADORA = P2_4;
            Remove_Timer(ti);
            ti = Set_Timer(2000, ONE_SHOT, ((void*)0));
        }
        break;

    case P2_4:
        if (Time_Out(ti)) {
            ESTADO_LAVADORA = P2_3;
            Remove_Timer(ti);
            ti = Set_Timer(4000, ONE_SHOT, ((void*)0));
        }
        else {
            P2OUT |= BIT0 ;
        }
        break;

    case P2_3:
        if (Time_Out(ti)) {
            ESTADO_LAVADORA = P2_2;
            Remove_Timer(ti);
            ti = Set_Timer(2000, ONE_SHOT, ((void*)0));
        }
        break;

    case P2_2:
        if (Time_Out(ti)) {
            ESTADO_LAVADORA = P2_1;
            Remove_Timer(ti);
            ti = Set_Timer(6000, ONE_SHOT, ((void*)0));
        }
        break;

    case P2_1:
        if (Time_Out(ti)) {
            P2OUT &= ~BIT0; // Apagar led 2.0
            ESTADO_LAVADORA = P2;
            Remove_Timer(ti);
        }
        break;

    default:
        break;
    }

    P4OUT ^= BIT6; // Bajada onda cuadrada tarea control discreto
}

float W, Wref = -1;

void motor (void) {
    P4OUT ^= BIT7; // Subida onda cuadrada tarea control motor

   switch(ESTADO_LAVADORA) {
        case P1:
            Wref = 0.f;
            break;

        case P1_5:
            Wref = 1.f;
            break;

        case P1_4:
            Wref = -1.f;
            break;

        case P1_3:
            Wref = 1.f;
            break;

        case P1_2:
            Wref = 0.f;
            break;

        case P1_1:
            Wref = 3.f;
            break;

        case P2:
            Wref = 0.f;
            break;

        case P2_4:
            Wref = -0.5f;
            break;

        case P2_3:
            Wref = 1.f;
            break;

        case P2_2:
            Wref = 0.f;
            break;

        case P2_1:
            Wref = 3.f;
            break;

        default:
            break;
        }

    W = velocity();
    R(Wref, W);

    P4OUT ^= BIT7; // Bajada onda cuadrada tarea control motor
}

int main(void) {
    Stop_Watchdog();    // Stop watchdog timer

    Init_CS();          // MCLK = 8 MHz, SMCLK = 4 MHz
    Init_GPIO();
    Init_Display();
    Init_Clock();

    Init_PWM () ;
    Init_Encoder () ;
    Init_AD () ;
    Init_Servos(100) ;

    unsigned int siguiente = Get_Time();
    unsigned char marco = 0;
    while (1) {
        switch (marco) {
        case 0:
            visualizador();
            automata();
            motor();
            marco ++;
            break;
        case 5:
            visualizador();
            automata();
            marco ++;
            break;
        case 9:
            visualizador();
            marco = 0;
            break;
        default:
            visualizador();
            marco ++;
            break;
        }
        siguiente += 10;
        delay_until(siguiente);
    }
}
