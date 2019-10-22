/*
 * TIMER.h
 *
 *  Created on: Sep 24, 2019
 *      Author: Mohammed
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "TIMER_cnfg.h"
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU   8000000ul
#endif

#define NULL_POINTER ((void*)0)

//------------------- Delay_units Macro_Constants -------------------------------//
#define SECOND		0
#define MILLISECOND 1
#define MICROSECOND 2
//------------------ Another Naming for Delay_units ------------//
#define SEC			0
#define MS			1
#define US			2

/*
//this line is used for testing only 
#define Delay_unit_t uint8	
*/
typedef enum {s=0,ms=1,us=2,sec=0,msec=1,usec=2}Delay_unit_t;


//-------------------  Public_Functions-------------------------------//
ACK TIMER_init(void);
ACK TIMER_ID_init( TIMER_t TIMER_Select );
ACK Time_Delay ( TIMER_t  , double  , Delay_unit_t );
ACK TIMER_Start ( TIMER_t  );
ACK PWM_generate( TIMER_t  , float  ,uint8 );
ACK interrupt_time_delay (TIMER_t );
ACK Update_Timer_TCNT_Register (TIMER_t , double  , Delay_unit_t );
ACK TIMER_Stop (TIMER_t );
ACK Enable_Timer_Interrupt (TIMER_t );
void timer2_delay_ms(uint32 delay_value);
ACK PWM_Channel_generate( TIMER_t TIMER_Select , Channel_t Channel_Select , float Required_duty_cycle , uint8 type );


void Timer0_Interrupt_Handling (void);
void Timer1_Interrupt_Handling (void);
void Timer2_Interrupt_Handling (void);

void Timer0_Set_Callback ( void (*ptr)(void) );
void Timer1_Set_Callback ( void (*ptr)(void) );
void Timer2_Set_Callback ( void (*ptr)(void) );

//----------------- Extern Global Variables -------------------------//
 extern  void (*g_callBackPtr0)(void)  ;
 extern  void (*g_callBackPtr1)(void) ;
 extern  void (*g_callBackPtr2)(void)  ;
 

extern uint32 OVF_Counter_Loop[MAX_NUM_OF_TIMERS] ;
extern uint8 Running_Flag[MAX_NUM_OF_TIMERS];		//this Flag is used when your Delay occured by using Interrupt so as to run your operation which needs that delay
extern uint8 PWM_Value;

#endif /* TIMER_H_ */
