
#ifndef __I2C__H_
#define __I2C__H_

#ifndef I2C_BUFFER_LENGTH
#define I2C_BUFFER_LENGTH 128
#endif

#include <msp430.h>

enum I2C_MODE { MASTER, SLAVE };

class __I2C__
{
	private:
	uint_16 slave_rx_length;	
	uint_16 slave_tx_length;
	uint_16 master_rx_length;	
	uint_16 master_tx_length;
	uint8_t i2c_mode;	

	private:
	char slave_txBuffer[I2C_BUFFER_LENGTH];
	char slave_rxBuffer[I2C_BUFFER_LENGTH];
	char master_txBuffer[I2C_BUFFER_LENGTH];
	char master_rxBuffer[I2C_BUFFER_LENGTH];
	uint_16 slave_tx_start;
	uint_16 slave_rx_start;
	uint_16 master_tx_start;
	uint_16 master_rx_start;
	uint8_t master_address;
	public:
	void set_master_address(uint8_t master_address);
	void __on_rx_receive(char c);
	unsigned char __on_tx_receive(char &c);
	__I2C__();
	void (*configure)(uint_16 speed);
	void (*enable_i2c_interrupts)();
	void (*disable_i2c_interrupts)();
	void (*enable_tx)();
	void (*disable_tx)();
	void (*fetch_bytes_from_slave)(char *buffer, uint_16 length, uint8_t slave_address);
	void (*send_bytes_to_slave)(char *buffer, uint_16 length, uint8_t slave_address);
	unsigned char slave_getchar(char &c);
	void master_getchar(char &c, uint8_t slave_address);
	unsigned char slave_putchar(char &c);
	void master_putchar(char &c, uint8_t slave_address);
	unsigned char slave_gets(char *buffer, uint_16 length);
	void master_gets(char *buffer, uint_16 length, uint8_t slave_address);
	unsigned char slave_puts(char *buffer, uint_16 length);
	void master_puts(char *buffer, uint_16 length, uint8_t slave_address);
	unsigned char slave_peek(char &c);
	uint_16 get_rxCount();	// Only defined for slave devices
	uint_16 get_txCount();  // Only defined for slave devices
	void (*slave_reset_release_bus)();
};

#endif


	
