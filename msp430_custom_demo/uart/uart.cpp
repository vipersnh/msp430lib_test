/*
* DESCRIPTION        : Function declarations file
* DATE               : 22/10/2013
*/


#include "uart.hpp"



/*
* NAME               : __UART__::__UART__()
* DESCRIPTION        : __UART__ constructor
*/
__UART__::__UART__()
{
	rx_length =  tx_length = rx_start = tx_start = 0;
}




/*
* NAME               : uint8_t __UART__::getchar(uint8_t &c)
* DESCRIPTION        : Return a character from the receive buffer
*/
uint8_t __UART__::getchar(uint8_t &c)
{
	if(rx_length > 0)
	{
		disable_uart_interrupts();
		c = rxBuffer[rx_start];
		rx_start++;
		rx_length--;
		if(rx_start >= UART_BUFFER_LENGTH)
		{
			rx_start = rx_start - UART_BUFFER_LENGTH;
		}
		enable_uart_interrupts();
		return 1;
	}
	else
	{
		return 0;
	}
}



/*
* NAME               : uint8_t __UART__::putchar(uint8_t c)
* DESCRIPTION        : Input a character to be transmitted by queueing into the tx buffer 
*/
uint8_t __UART__::putchar(uint8_t c)
{
	if(tx_length < UART_BUFFER_LENGTH)
	{
		disable_uart_interrupts();
		txBuffer[(tx_start+tx_length)%UART_BUFFER_LENGTH] = c;
		tx_length++;
		enable_tx();
		enable_uart_interrupts();
		return 1;
	}
	else
	{
		return 0;
	}
}



/*
* NAME               : uint8_t __UART__::gets(char *buffer, uint16_t length)
* DESCRIPTION        : Return a given length of characters from the received buffer into 
		       the memory location passed and return the number of bytes copied.
*/
uint8_t __UART__::gets(uint8_t *buffer,uint16_t length)
{
	//disable all interrupts from uart here
	disable_uart_interrupts();
	if(rx_length >= length)
	{
		for(uint16_t i = 0; i< length; i++)
		{
			buffer[i] = rxBuffer[rx_start];
			rx_start++;
			if(rx_start >= UART_BUFFER_LENGTH)
				rx_start -= UART_BUFFER_LENGTH;
		}
		rx_length -= length;
		return length;
	}
	else
	{
		uint16_t &i = length;
		for(i = 0;i<rx_length;i++)
		{
			buffer[i] = rxBuffer[(rx_start)];
			rx_start++;
			if(rx_start>=UART_BUFFER_LENGTH)
			{
				rx_start-=UART_BUFFER_LENGTH;
			}
		}
		length = rx_length;
		rx_length = 0;
		return length;
	}
	//enable all interrupts from uart here
	enable_uart_interrupts();
}



/*
* NAME               : uint8_t __UART__::puts(char *buffer, uint16_length)
* DESCRIPTION        : Insert the characters from the given buffer into the tx buffer. 
		       Count of characters is given in length.
*/
uint8_t __UART__::puts(char const *buffer, uint16_t length)
{
	if((UART_BUFFER_LENGTH - tx_length) > length)
	{
		disable_uart_interrupts();
		for(uint16_t i = 0; i< length ; i++)
		{
			txBuffer[(tx_start+tx_length+i)%UART_BUFFER_LENGTH] = buffer[i];
		}
		tx_length += length;
		enable_tx();
		enable_uart_interrupts();
		return 1;
	}
	else
	{
		return  0;
	}
}



/*
* NAME               : uint8_t __UART__::peek(char &c)
* DESCRIPTION        : Get the immediate next character from rx buffer without removing it.
*/
uint8_t __UART__::peek(uint8_t &c)
{
	if(rx_length)
	{
		disable_uart_interrupts();
		c  = rxBuffer[(rx_start+rx_length)%UART_BUFFER_LENGTH];
		enable_uart_interrupts();
		return 1;
	}
	else
	{
		return 0;
	}
}



/*
* NAME               : uint8_t __UART__::get_rxCount()
* DESCRIPTION        : Return the number of received bytes/chars available in the received buffer.
*/
uint16_t __UART__::get_rxCount()
{
	return rx_length;
}



/*
* NAME               : uint8_t __UART__::get_txCount
* DESCRIPTION        : Return the number of bytes/chars space available in the transmit buffer.
*/
uint16_t __UART__::get_txCount()
{
	return (UART_BUFFER_LENGTH - tx_length);
}



/*
* NAME               : uint8_t __UART__::__on_rx_interrupt
* DESCRIPTION        : Function to insert byte/char into rxBuffer after receiving one char.
*/
void __UART__::__on_rx_receive(uint8_t c)
{
	if(rx_length<UART_BUFFER_LENGTH)
	{
		rxBuffer[(rx_start+rx_length)%UART_BUFFER_LENGTH] = c;
		rx_length++;
	}
	else
	{
		rxBuffer[(rx_start+rx_length)%UART_BUFFER_LENGTH] = c;
		rx_start++;
		if(rx_start >= UART_BUFFER_LENGTH)
			rx_start -= UART_BUFFER_LENGTH;
	}
}



/*
* NAME               : unsigned uint8_t __UART__::__on_tx_interrupt
* DESCRIPTION        : Function to return a character to be transmitted from txBuffer
*/

uint8_t __UART__::__on_tx_receive(uint8_t *c)
{
	if(tx_length)
	{
		*c = txBuffer[tx_start];
		tx_length--;
		tx_start++;
		if(tx_start >= UART_BUFFER_LENGTH)
			tx_start -= UART_BUFFER_LENGTH;
		return 1;
	}
	else
	{
		disable_tx();
		return 0;
	}
}

