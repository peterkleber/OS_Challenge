/*
 * main.c
 *
 *  Created on: Oct 11, 2019
 *      Author: PeterKleber
 */


#include "OS.h"


 

void Task1 (void);
void Task2 (void);
void Task3 (void);

ST_Task_Info Task_Buffer[BUFFER_SIZE]=
{
	{&Task1,7,PERIODIC,Ready,1},
	{&Task2,5,PERIODIC,Ready,2},
	{&Task3,13,PERIODIC,Ready,3}
};

void Task1 (void)
{
	PORTC ^= (1<<PC5);
}

void Task2 (void)
{
	PORTC ^= (1<<PC6);
}

void Task3 (void)
{
	PORTC ^= (1<<PC7);
}


int main()
{
	DDRD = 0xFF;
	DDRC = 0xFF;
	DDRB = 0xFF;
	
	OS_Init(&OS_cnfg);
	
	OS_Create_Task(&Task_Buffer[0]);
	OS_Create_Task(&Task_Buffer[1]);
	OS_Create_Task(&Task_Buffer[2]);
	

	OS_Run();

	return 0;

}


