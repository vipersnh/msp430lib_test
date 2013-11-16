
#include "spi.h"

__SPI__::__SPI__()
{
	slave_tx_length = slave_tx_start = 0;
}


void __SPI__::master_putchar(char &c)
{
	send_bytes_to_slave(&c, 1);
}

void __SPI__::master_getchar(char &c)
{
	fetch_bytes_from_slave(&c, 1);
}


void __SPI__::master_puts(char *buffer, uint_16 length)
{
	for(uint_16 i = 0; i< length; i++)
	{
		master_putchar(buffer[i]);
	}
}

void __SPI__::master_gets(char *buffer, uint_16 length)
{
	char ch;
	for(uint_16 i=0; i< length; i++)
	{
		master_getchar(ch);
		buffer[i] = ch;
	}
}

void __SPI__::slave_putchar(char &c)
{
	if(slave_tx_length < SPI_BUFFER_LENGTH)
	{
		disable_spi_interrupts();
		slave_txBuffer[(slave_tx_start+slave_tx_length)%SPI_BUFFER_LENGTH] = c;
		slave_tx_length++;
		enable_spi_interrupts();
	}
}

void __SPI__::slave_puts(char *buffer, uint_16 length)
{
	if(slave_tx_length + length -1 < SPI_BUFFER_LENGTH)
	{
		disable_spi_interrupts();
		for(uint_16 i=0;i<length ; i++)
		{
			slave_txBuffer[(slave_tx_start+slave_tx_length+i)%SPI_BUFFER_LENGTH] = buffer[i];
		}
		slave_tx_length += length;
	}
}


void __SPI__::slave_clear_tx_buffer()
{
	slave_tx_length = slave_tx_start = 0;
}


