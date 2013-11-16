/*
* DESCRIPTION        : Function declarations file
* DATE               : 22/10/2013
*/

#include "i2c.h"



__I2C__::__I2C__()
{
	slave_rx_length = slave_tx_length = slave_tx_start = slave_rx_start = 0;
	master_rx_length = master_tx_length = master_tx_start = master_rx_start = 0;
	i2c_mode = SLAVE;
}


void __I2C__::set_master_address(uint8_t master_address)
{
	this->master_address = master_address;
}

void __I2C__::__on_rx_receive(char c)
{
	if(i2c_mode == MASTER)
	{
		if(master_rx_length < I2C_BUFFER_LENGTH)
		{
			master_rxBuffer[(master_rx_start + master_rx_length)%I2C_BUFFER_LENGTH] = c;
			master_rx_length++;
		}
		else
		{
			master_rxBuffer[(master_rx_start + master_rx_length)%I2C_BUFFER_LENGTH] = c;
			master_rx_start++;
			if(master_rx_start >= I2C_BUFFER_LENGTH)
				master_rx_start -= I2C_BUFFER_LENGTH;
		}	
	}
	else
	{
		if(slave_rx_length < I2C_BUFFER_LENGTH)
		{
			slave_rxBuffer[(slave_rx_start + slave_rx_length)%I2C_BUFFER_LENGTH] = c;
			slave_rx_length++;
		}
		else
		{
			slave_rxBuffer[(slave_rx_start + slave_rx_length)%I2C_BUFFER_LENGTH] = c;
			slave_rx_start++;
			if(slave_rx_start >= I2C_BUFFER_LENGTH)
				slave_rx_start -= I2C_BUFFER_LENGTH;
		}	

	}
}



unsigned char __I2C__::__on_tx_receive(char &c)
{
	if(i2c_mode == MASTER)
	{
		if(master_tx_length)
		{
			c = master_txBuffer[(master_tx_start)];
			master_tx_start++;
			if(master_tx_start >= I2C_BUFFER_LENGTH)
				master_tx_start -= I2C_BUFFER_LENGTH;
			return 1;
		}
		else
		{
			disable_tx();
			return -1;
		}
	}
	else
	{
		if(slave_tx_length)
		{
			c = slave_txBuffer[(slave_tx_start)];
			slave_tx_start++;
			if(slave_tx_start >= I2C_BUFFER_LENGTH)
				slave_tx_start -= I2C_BUFFER_LENGTH;
			return 1;
		}
		else
		{
			disable_tx();
			return -1;
		}
	}
}



unsigned char __I2C__::slave_getchar(char &c)
{
	if(slave_rx_length > 0)
	{
		disable_i2c_interrupts();
		c = slave_rxBuffer[slave_rx_start];
		slave_rx_start++;
		slave_rx_length--;
		if(slave_rx_start >= I2C_BUFFER_LENGTH)
			slave_rx_start -= I2C_BUFFER_LENGTH;
		enable_i2c_interrupts();
		return 1;
	}
	else
	{
		return -1;
	}
}
	

void __I2C__::master_getchar(char &c uint8_t slave_address)
{
	fetch_bytes_from_slave(&c,1,slave_address);	
}



unsigned char __I2C__::slave_putchar(char &c)
{
	if(slave_tx_length < I2C_BUFFER_LENGTH)
	{
		disable_i2c_interrupts();
		slave_txBuffer[(slave_tx_start+slave_tx_length)%I2C_BUFFER_LENGTH] = c;
		slave_tx_length++;
		enable_i2c_interrupts();
		return 1;
	}
	else
	{
		return -1;
	}
}



void __I2C__::master_putchar(char &c, uint8_t slave_address)
{
	send_bytes_to_slave(&c, 1, slave_address);
}		



unsigned char __I2C__::slave_gets(char *buffer, uint_16 length)
{
	disable_i2c_interrupts();
	if(slave_rx_length >= length)
	{
		for(uint_16 = 0; i < length ; i++)
		{
			buffer[i] = slave_rxBuffer[slave_rx_start];
			slave_rx_start++;
			if(slave_rx_start >= I2C_BUFFER_LENGTH)
				slave_rx_start -= I2C_BUFFER_LENGTH;
		}
		slave_rx_length -= length;
		return length;
	}
	else
	{
		uint_16 &i = length;
		for(i=0;i<slave_rx_length;i++)
		{
			buffer[i] = slave_rxBuffer[slave_rx_start];
			slave_rx_start++;
			if(slave_rx_start >= I2C_BUFFER_LENGTH)
				slave_rx_start -= I2C_BUFFER_LENGTH;
		}
		length = slave_rx_length;
		slave_rx_length = 0;
		return length;
	}
	enable_i2c_interrupts();
}



void __I2C__::master_gets(char *buffer, uint_16 length, uint8_t slave_address)
{
	fetch_bytes_from_slave(buffer, length, slave_address);
}



unsigned char __I2C__::slave_puts(char *buffer, uint_16 length)
{
	if( (I2C_BUFFER_LENGTH - slave_tx_length) > length)
	{
		disable_i2c_interrupts();
		for(uint_16 i=0;i< length;i++)
			txBuffer[(slave_tx_start+slave_tx_length+i)%I2C_BUFFER_LENGTH] = buffer[i];
		slave_tx_length += length;
		enable_i2c_interrupts();
		return 1;
	}
	else
	{
		return -1;
	}
}



void __I2C__::master_puts(char *buffer, uint_16 length. uint8_t slave_address)
{
	send_bytes_to_slave(buffer, length, slave_address);
}



unsigned char __I2C__::slave_peek(char &c)
{
	if(slave_rx_length)
	{
		disable_i2c_interrupts();
		c = slave_rxBuffer[(slave_rx_start+slave_rx_length)%I2C_BUFFER_LENGTH];
		enable_i2c_interrupts();
		return 1;
	}
	else
	{
		return -1;
	}
}



uint_16 __I2C__::get_rxCount()
{
	return slave_rx_length;
}



uint_16 __I2C__::get_txCount()
{
	return I2C_BUFFER_LENGTH - slave_tx_length;
}




