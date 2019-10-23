/*
 * OS.c
 *
 *  Created on: Oct 22, 2019
 *      Author: PeterKleber
 */

#include "OS.h"



static uint8 Buffer_ptr = 0;
static uint8 OS_init_flag = 0;
volatile uint16 OS_Tick_Count = 0;
volatile uint8 ISR_Generated_Flag = 0;
static uint8 System_Tick_Count = 0 ;
static uint16 Max_Run_Time = 0;
static uint8 Timer_Select ;	//any value except 0,1,2


ST_Task_Info Task_Buffer[BUFFER_SIZE];

static void Null_func() {
}


//Call back function for the ISR to set the flag
void ISR_Generated_Flag_Setter() 
{ 
	ISR_Generated_Flag = 1;
}


void CPU_Sleep (void)
{
		MCUCR |= (1<<SE);
}


EnmOSError_t OS_Init(const OS_ConfigType *ConfigPtr) 
{
	PORTB ^= (1<<PB7);
	Timer_Select = ConfigPtr->timer;
	if (OS_init_flag == 0) 
	{
		if (ConfigPtr != NULL_PTR) 
		{
			//Initialize the buffer
			for (uint8 i = 0; i < BUFFER_SIZE; i++) 
			{
				Task_Buffer[i].Ptr = Null_func;
				Task_Buffer[i].Run_Time = 0;
				Task_Buffer[i].Mode = NO_MODE;     //Outside the enum values
				Task_Buffer[i].Status = Idle;
				Task_Buffer[i].Priority = (uint8)255;
			}

			TIMER_ID_init(Timer_Select);
			//send callback function
			if ((ConfigPtr->timer) == TIMER_0) 
			{
				Timer0_COMP_Set_Callback(ISR_Generated_Flag_Setter);
			} else if ((ConfigPtr->timer) == TIMER_1) 
			{
				Timer1_COMP_Set_Callback(ISR_Generated_Flag_Setter);
			} else if ((ConfigPtr->timer) == TIMER_2) 
			{
				Timer2_COMP_Set_Callback(ISR_Generated_Flag_Setter);
			}

			Time_Delay(Timer_Select,(ConfigPtr->Resolution), ms);		
			OS_init_flag = 1;
		}
	}

	return OS_OK;
}

/*
// feh haga fe accessing beta3 el incoming task info
EnmOSError_t OS_Create_Task(const ST_Task_Info *ST_Incoming_Task_Info)
{
	if (OS_init_flag == 1)
	{
		PORTB |= (1<<PB0);
		if (Task_Buffer->Ptr == NULL_PTR)
		{
			PORTB |= (1<<PB1);
			if (Buffer_ptr < BUFFER_SIZE)
			{
				PORTB |= (1<<PB2);
				Task_Buffer[Buffer_ptr].Ptr = ST_Incoming_Task_Info->Ptr;
				Task_Buffer[Buffer_ptr].Mode = ST_Incoming_Task_Info->Mode;
				Task_Buffer[Buffer_ptr].Run_Time = ST_Incoming_Task_Info->Run_Time;
				Task_Buffer[Buffer_ptr].Status =  ST_Incoming_Task_Info->Status;
				Task_Buffer[Buffer_ptr].Priority = ST_Incoming_Task_Info->Priority;

				Buffer_ptr++;
			}
			
			if(Max_Run_Time < Task_Buffer[Buffer_ptr].Run_Time)
			{
				PORTB |= (1<<PB3);
				Max_Run_Time = Task_Buffer[Buffer_ptr].Run_Time;
			}
		}

	}
	return OS_OK;
}
*/


// feh haga fe accessing beta3 el incoming task info 
EnmOSError_t OS_Create_Task( void (*Ptr) (void) , uint16 Run_Time , Rotation_t Mode ,  Status_t Status ,uint8 Priority )
{
	if (OS_init_flag == 1)
	{
		PORTB ^= (1<<PB0);
		if (Task_Buffer->Ptr != NULL_PTR)
		{
			PORTB ^= (1<<PB1);			
			if (Buffer_ptr < BUFFER_SIZE)
			{
				PORTB ^= (1<<PB2);
				Task_Buffer[Buffer_ptr].Ptr = Ptr;
				Task_Buffer[Buffer_ptr].Mode = Mode;
				Task_Buffer[Buffer_ptr].Run_Time = Run_Time;
				Task_Buffer[Buffer_ptr].Status = Status;
				Task_Buffer[Buffer_ptr].Priority = Priority;
				
				if(Max_Run_Time < Task_Buffer[Buffer_ptr].Run_Time)
				{
					PORTB ^= (1<<PB3);
					Max_Run_Time = Task_Buffer[Buffer_ptr].Run_Time;
				}
				
				Buffer_ptr++;		
			}
		}
	}
	return OS_OK;
}



void OS_Run(void) 
{
	TIMER_Start(Timer_Select);
while(1)
{
	 uint8 Highest_Priority_Index = (uint8) 255;
	 uint8 Highest_Priority  = (uint8) 255;

	if (OS_init_flag == 1) 
	{	
		PORTD ^= (1<<PD0);
		if (ISR_Generated_Flag == 1) 
		{
			PORTD ^= (1<<PD1);
			System_Tick_Count++; //when an ISR happens increment the tick counter

			for (uint8 i = 0; i < Buffer_ptr; i++) 
			{
				PORTD ^= (1<<PD2);
				if (System_Tick_Count == Task_Buffer[i].Run_Time) 
				{
					PORTD ^= (1<<PD3);
					if (Task_Buffer[i].Ptr != NULL_PTR) 
					{	
						PORTD ^= (1<<PD4);
						if (Task_Buffer[i].Status == Ready) 
						{
							PORTD ^= (1<<PD5);
							if (Highest_Priority > Task_Buffer[i].Priority) 
							{	
								PORTD ^= (1<<PD6);
								Highest_Priority = Task_Buffer[i].Priority;
								Highest_Priority_Index = i;
								
							}
								
						}
						
						PORTD ^= (1<<PD7);
						void (*Ptr_to_excute)(void) = Task_Buffer[Highest_Priority_Index].Ptr;
						
						Ptr_to_excute();

						Task_Buffer[Highest_Priority_Index].Status = Running;

						if (Task_Buffer[Highest_Priority_Index].Mode == ONE_SHOT)
						{
						
							if (Buffer_ptr > 1)
							{
								Task_Buffer[Highest_Priority_Index].Ptr =Task_Buffer[Buffer_ptr - 1].Ptr;
								Task_Buffer[Highest_Priority_Index].Mode =Task_Buffer[Buffer_ptr - 1].Mode;
								Task_Buffer[Highest_Priority_Index].Run_Time = Task_Buffer[Buffer_ptr- 1].Run_Time;
								Task_Buffer[Highest_Priority_Index].Status = Task_Buffer[Buffer_ptr- 1].Status;
								
								Buffer_ptr--;
							}
						} //end of the ONE_SHOT Task
			
					}//end of the NULL_Pointer condition
			}// end of the comparison between system tick and Run_Time

		}	// end of the For Loop
			
			ISR_Generated_Flag = 0;
	}
}
	// CPU_SLeep_Function Should be here
	//CPU_Sleep ();
}// end of while (1)

}// end of the OS_Run Function


EnmOSError_t OS_Delete_Task(const ST_Task_Info *ST_Incoming_Task_Info) 
{
if (OS_init_flag == 1) 
{

	if (ST_Incoming_Task_Info->Ptr != NULL_PTR) 
	{
		for (uint8 i = 0; i < Buffer_ptr; i++) 
		{

			if ((ST_Incoming_Task_Info->Ptr == Task_Buffer[i].Ptr)) { //Search for the function in the buffer
				//		TMU_Buffer[i].Status = NOT_Active; //if it exist change it's state to not active
				Task_Buffer[i].Status = DELETED;
				//Overwrite the not active element with the last active one
				if (Buffer_ptr > 1) 
				{
					Task_Buffer[i].Ptr = Task_Buffer[Buffer_ptr - 1].Ptr;
					Task_Buffer[i].Mode = Task_Buffer[Buffer_ptr - 1].Mode;
					Task_Buffer[i].Run_Time =Task_Buffer[Buffer_ptr - 1].Run_Time;
					Task_Buffer[i].Status = Task_Buffer[Buffer_ptr - 1].Status;
					Task_Buffer[i].Priority = Task_Buffer[Buffer_ptr - 1].Priority;

					Buffer_ptr--;
				}
			}
		}

		for (uint8 i = 0; i < Buffer_ptr; i++)
		{
			if (Max_Run_Time < Task_Buffer[i].Run_Time) 
				{
				Max_Run_Time = Task_Buffer[i].Run_Time;
				}
		}

		if (System_Tick_Count >= Max_Run_Time) 
		{
			System_Tick_Count = 0;
		}
	}
}
return OS_OK;
}


EnmOSError_t OS_DeInit ( void )
{
	if (OS_init_flag == 1)
	{
		//Clear the Buffer
		for (uint8 i = 0; i < Buffer_ptr; i++)
		{
			Task_Buffer[i].Ptr = NULL_PTR;
			Task_Buffer[i].Run_Time = 0;
			Task_Buffer[i].Mode = NO_MODE; //Outside the enum values
			Task_Buffer[i].Status = Idle;
			Task_Buffer[i].Priority = (uint8)255;
		}

		Buffer_ptr = 0; //return Buffer_ptr to zero

		OS_init_flag = 0;

		return OS_OK;
	}
	else
	{
		return OS_NOK;
	}
	return OS_OK;
}

