/******************************************************************************/
/*                                                                            */
/* project  : PRACTICAS SE UNIZAR                                             */
/* filename : servos.h                                                        */
/* version  : 3                                                               */
/* date     : 16/09/2016                                                      */
/* author   : Jose Luis Villarroel                                            */
/* description : Servocontrol of LEGO robot motors                            */
/*                                                                            */
/******************************************************************************/

#ifndef SERVOS_H_
#define SERVOS_H_

/******************************************************************************/
/*                        Exported functions                                  */
/******************************************************************************/

void Init_Servos (unsigned int T) ; // sampling period in ms

float velocity (void) ;  // returns rad/s

void action (float U) ;  // action in Volts

float R (float Wref, float W) ;
// Returns the action in volts, left motor
//   Wref_i : angular velocity reference in rad/s
//   W_i : actual angular velocity rad/s


#endif /* SERVOS_H_ */
