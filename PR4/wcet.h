/******************************************************************************/
/*                                                                            */
/* project  : PRACTICAS SE UNIZAR                                             */
/* filename : wcet.h                                                          */
/* version  : 1                                                               */
/* date     : 30/04/2024                                                      */
/* author   : Jose Luis Villarroel                                            */
/* description : Modulo medida WCET                                           */
/*                                                                            */
/******************************************************************************/

#ifndef WCET_H_
#define WCET_H_

/******************************************************************************/
/*                        Exported functions                                  */
/******************************************************************************/

void Init_WCET (void) ;

void Start_measurement (void) ;
void End_measurement (void) ;

// unsigned int Get_WCET (void) ;
// unsigned int Get_meanC (void) ;



#endif /* WCET_H_ */
