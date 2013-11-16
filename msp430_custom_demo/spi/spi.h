

#ifndef __SPI__H__
#define __SPI__H__

#ifndef SPI_BUFFER_LENGTH
#define SPI_BUFFER_LENGTH 128
#endif

#include <msp430.h>

class __SPI__
{	
	private:
	uint_16 slave_tx_length;
	
	private:
	char slave_txBuffer[SPI_BUFFER_LENGTH];
	uint_16 slave_tx_start;

	public:
	__SPI__();
	void master_putchar(char &c);
	void master_getchar(char &c);
	void master_puts(char *buffer, uint_16 length);
	void master_gets(char *buffer, uint_16 length);

	void (*disable_spi_interrupts)();
	void (*enable_spi_interrupts)();
	void (*configure)(uint_16 speed);
	void (*send_bytes_to_slave)(char *buffer, uint_16 length);
	void (*fetch_bytes_from_slave)(char *buffer, uint_16 length);	
	void (*on_slave_byte_receive)(char &c);	//Function callback for slave byte reception
	void slave_putchar(char &c);
	void slave_puts(char *buffer, uint_16 length);
	void slave_clear_tx_buffer();
};

#endif	
