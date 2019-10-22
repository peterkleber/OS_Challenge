/*
 * OS.h
 *
 *  Created on: Oct 22, 2019
 *      Author: PeterKleber
 */

#ifndef OS_H_
#define OS_H_

#include <avr/io.h>
#include"std_types.h"
#include "Timer.h"
#include "OS_cnfg.h"

#define BUFFER_SIZE ((uint8)3)

typedef enum {Ready,Running,DELETED,Idle}Status_t;

typedef enum {
 PERIODIC, ONE_SHOT,NO_MODE
 } Rotation_t;

typedef enum {
	OS_NOK, OS_OK
} EnmOSError_t;


typedef struct{
	void (*Ptr) ();
	uint16 Run_Time ;
	Rotation_t Mode ;
	Status_t Status ;
	uint8 Priority; //0 is the max priority
}ST_Task_Info;

/*EnmTMUError_t TMU_Init(const TMU_ConfigType *ConfigPtr);
EnmTMUError_t TMU_Start_Timer(uint16 Time_Delay, void (*Ptr)(void), Rotation_t mode);
EnmTMUError_t TMU_Stop_Timer(void (*Ptr)(void));
EnmTMUError_t TMU_Dispatch();
EnmTMUError_t TMU_DeInit(void);
*/

EnmOSError_t OS_Init (const OS_ConfigType * ConfigPtr );
EnmOSError_t OS_DeInit ( void ) ;
void OS_Run(void);
EnmOSError_t OS_Create_Task(const ST_Task_Info * OS_Buffer );
EnmOSError_t OS_Delete_Task(const ST_Task_Info * OS_Buffer );
void CPU_Sleep (void);
void ISR_Generated_Flag_Setter();

//extern uint8 Buffer_ptr ;

#endif /* OS_H_ */
