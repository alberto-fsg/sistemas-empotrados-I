
/******************************************************************************/
/*                                                                            */
/*                 Area Ingenieria de Sistemas y Automatica                   */
/*           Departamento de Informatica e Ingenieria de Sistemas             */
/*                         Universidad de Zaragoza                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* proyecto  : PRACTICAS SEP - SEI UNIZAR                                     */
/* fichero : clock.h                                                          */
/* version  : 1                                                               */
/* fecha     : 20/12/2021                                                     */
/* autor   : Jose Luis Villarroel                                             */
/* descripcion : Implementacion del modulo de reloj (MSP430)                  */
/*                                                                            */
/******************************************************************************/

/*******************************************************************

MCLK = 8 MHz
SMCLK = 4 MHz
El tick del reloj de sistema se fija en 1 ms

********************************************************************/

/******************************************************************************/
/*                        Modulos usados                                      */
/******************************************************************************/

#include <msp430.h>
#include "clock.h"


/******************************************************************************/
/*                        Variables del modulo                                */
/******************************************************************************/

volatile static unsigned int tick_counter ;

typedef struct{
    unsigned int deadline ;
    unsigned int period ;
    char active ;
    char TO ;
    void (*timer_function)(void);
} Timer_struct ;

volatile static Timer_struct Timer[8] ;


/******************************************************************************/
/*                        Implementacion servicios                            */
/******************************************************************************/

#pragma vector=RTC_VECTOR
__interrupt void Tick(void)
{
    unsigned char i = 0 ;

    if (RTCIV)
    { /* clear RTCIFG */
        tick_counter++;

        for (i=0;i<8;i++)
        if (Timer[i].active)
            if (Timer[i].deadline == tick_counter)
            {
                Timer[i].TO = 1;
                if (Timer[i].period) Timer[i].deadline += Timer[i].period ;
                if (Timer[i].timer_function)
                    (*Timer[i].timer_function)();
            }
    }
    __low_power_mode_off_on_exit() ;
    return;
}


void Init_Clock(void)
{
    unsigned char i = 0 ;

    __disable_interrupt();

    tick_counter = 0;

    RTCMOD = 4 - 1;                                 // tick = 1 ms
                                                    // SMCLK(4MHz)/1000*4
    SYSCFG2 |= RTCCKSEL_0;                          // Select SMCLK as RTC clock
    RTCCTL = RTCSS_1 | RTCSR | RTCPS__1000 | RTCIE; // RTCSS_1: Clock source device specific (SMCLK)
                                                    // RTCSR reset
                                                    // RTCPS_1000: preescaler /1000
                                                    // RTCIE interrupt enabled
    for (i=0;i<8;i++){
        Timer[i].active = 0 ;
        Timer[i].TO = 0 ;
    }

    __enable_interrupt();

    return;
}


unsigned int Get_Time(void)
{

    unsigned int Aux_TC;

    __disable_interrupt();
    Aux_TC = tick_counter;
    __enable_interrupt();
    return Aux_TC;
}

void delay_until(unsigned int T)
{

    while (1)
    {
        __disable_interrupt();
        if (T == tick_counter)
        {
            __enable_interrupt();
            return;
        }
        __low_power_mode_0();
    }
}

void delay(unsigned int T)
{
    delay_until(T+Get_Time()) ;
    return ;
}

Timer_id Set_Timer (unsigned int Ticks, Timer_type tt, void (*p)(void))
{
    unsigned char i = 0 ;

    for (i=0;i<8;i++){
        if (!Timer[i].active){
            Timer[i].deadline = Get_Time() + Ticks;
            if (tt == PERIODIC) Timer[i].period = Ticks ;
            else Timer[i].period = 0 ;
            Timer[i].timer_function = p;
            Timer[i].TO = 0;
            Timer[i].active = 1;
            return i ;
        }
    }

    return 255 ;
}

char Time_Out(Timer_id n)
{
    return Timer[n].TO;
}

char Is_Active (Timer_id n)
{
    return Timer[n].active;
}

void Remove_Timer(Timer_id n)
{
    Timer[n].active = 0;
    Timer[n].TO = 0;
}

void Clear_Timer(Timer_id n)
{
    Timer[n].TO = 0;
}
