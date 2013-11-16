/* Standard includes. */

/* Scheduler includes. */
extern "C"
{
#include "FreeRTOS.h"
#include "task.h"
#include <signal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
}


#include "uart/uart.hpp"
#include "uart/uart_A3.hpp"
#define LED_DELAY1 1000
#define LED_DELAY2 2000

extern __UART__ uart_A3;

void hardwareInitialize()
{
	WDTCTL = WDTPW+WDTHOLD;
	int timeout = 5000;
	P11DIR = P11DIR | 0x02 | 0x04;            //set P11.1 & P11.2 as output
	P11SEL = P11SEL | 0x02 | 0x04;            //set P11.1 & P11.2 as MCLK/SMCLK function
	P5SEL = P5SEL | 0x04 | 0x08;              //set P5.2 & P5.3 as XT2IN/XT2OUT
	
	UCSCTL6 = UCSCTL6 & (~XT2OFF);            //set XT2OFF=0 (bit8 = 0) --> enable XT2
	UCSCTL3 = UCSCTL3 | 0x20;                 //set SELREF = 010 --> FLL clock source = REFOCLK
	UCSCTL4 = UCSCTL4 | 0x200;                //set SELA = 010 --> ACLK=REFO
	
	do
	{
	  if (!(UCSCTL7 & 0x08)) break;
	  UCSCTL7 = UCSCTL7 & ~(0x0F);            // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 = SFRIFG1 & ~(0x02);            // Clear fault flags (SFRIFG1.OFIFG = 0)
	  timeout--;
	}
	while ((SFRIFG1 & 0x02) && timeout);      // Test oscillator fault flag
	
	UCSCTL5 = UCSCTL5 | 0x0005;               //MCLK/SMCLK divisor = 32
	
	UCSCTL4 = UCSCTL4 | 0x55;                 //SMCLK=MCLK=XT2
	UCSCTL4 = ((UCSCTL4 & ~0x100) & ~0x200) & ~0x400;     //ACLK=XT1  (SELA = 0b000)
	uart_A3_initialize();
	eint();
}
void task_1(void *p)
{
	P10DIR = P10DIR|0x80;
	while(1)
	{
		P10OUT = P10OUT | 0x80 ;	//Set LED1 ON, LED2 OFF
		vTaskDelay(LED_DELAY1/2);
		P10OUT = (P10OUT&(~0x80));	//Set LED1 ON, LED2 OFF
		vTaskDelay(LED_DELAY1/2);
		uart_A3.putchar(char(97+(uint8_t)(rand()%3)));
	}
}

void task_2(void *p)
{
	P10DIR = P10DIR|0x40;
	while(1)
	{
		P10OUT = (P10OUT|0x40);	//Set LED1 ON, LED2 OFF
		vTaskDelay(LED_DELAY2/2);
		P10OUT = (P10OUT&(~0x40));	//Set LED1 ON, LED2 OFF
		vTaskDelay(LED_DELAY2/2);
	}
}
int main( void )
{
	hardwareInitialize();
	uart_A3.configure(9600,EVEN_PARITY);
	xTaskCreate(task_1, (signed char *) "task_1",100,0,1,0);
	xTaskCreate(task_2, (signed char *) "task_2",100,0,1,0);
	vTaskStartScheduler();
	return -1;
}










