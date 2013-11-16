
#ifndef __UART__H__
#define __UART__H__

#ifndef UART_BUFFER_LENGTH
#define UART_BUFFER_LENGTH 128
#endif

#include <msp430.h>
#include <stdint.h>
enum PARITY {EVEN_PARITY = 0, ODD_PARITY = 1};


class __UART__
{
	private:
	uint16_t rx_length;
	uint16_t tx_length;

	private:
	uint8_t txBuffer[UART_BUFFER_LENGTH];
	uint8_t rxBuffer[UART_BUFFER_LENGTH];
	uint16_t tx_start;
	uint16_t rx_start;
	
	public:
	void __on_rx_receive(uint8_t c);
	uint8_t __on_tx_receive(uint8_t *c = 0x00);
	__UART__();
	void (*configure)(uint16_t, PARITY);
	void (*enable_uart_interrupts)();
	void (*disable_uart_interrupts)();
	void (*enable_tx)();
	void (*disable_tx)();
	uint8_t getchar(uint8_t &c);
	uint8_t putchar(uint8_t c);
	uint8_t gets(uint8_t *buffer, uint16_t length);
	uint8_t puts(char const *buffer, uint16_t length);
	uint8_t peek(uint8_t &c);
	uint16_t get_rxCount();	// Count of received bytes available
	uint16_t get_txCount();	// Count of bytes left available and can be accomodated at the time of calling
};

#endif


