/******************************************************************************/
/*                                                                            */
/* project  : PRACTICAS SE UNIZAR                                             */
/* filename : wcet.c                                                          */
/* version  : 1                                                               */
/* date     : 30/04/2024                                                      */
/* author   : Jose Luis Villarroel                                            */
/* description : Modulo medida WCET                                           */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                        Modulos usados                                      */
/******************************************************************************/

#include <msp430.h>
#include "InitSystem.h"
#include "wcet.h"

/******************************************************************************/
/*                        Variables del modulo                                */
/******************************************************************************/

 unsigned long execution_time_summation ;

 unsigned int current_start_time ;
 unsigned int current_stop_time ;
 unsigned int current_execution_time ;
 unsigned int max_execution_time ;
 unsigned int mean_execution_time ;
 unsigned int n_executions ;



/******************************************************************************/
/*                        Exported functions                                  */
/******************************************************************************/

void Init_WCET (void) {

    TB0CTL = TBSSEL_2 | ID_2 | MC_2;    // TBSEL_2 --> SMCLK (4MHz)
                                        // ID_3 --> PS = /4  (1MHz)
                                        // MC_2 --> continous mode 16b

    current_execution_time = 0 ;
    max_execution_time = 0 ;
    mean_execution_time = 0 ;
    n_executions = 0 ;

}

void Start_measurement (void) {
    current_start_time = TB0R ;
}
void End_measurement (void) {
    current_stop_time = TB0R ;
    current_execution_time = current_stop_time - current_start_time ;
    execution_time_summation += current_execution_time ;
    n_executions++ ;
    mean_execution_time = execution_time_summation/n_executions ;
    if (current_execution_time > max_execution_time)  max_execution_time = current_execution_time ;
}

// unsigned int Get_WCET (void) ;
// unsigned int Get_meanC (void) ;




