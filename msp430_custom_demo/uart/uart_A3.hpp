
#ifndef __UART_A3__
#define __UART_A3__

#include "uart.hpp"
#include "../FreeRTOSConfig.h"
#include "msp430.h"

void uart_A3_enable_uart_A3_interrupts();
void uart_A3_disenable_uart_A3_interrupts();
void uart_A3_configure(uint16_t, PARITY);
void uart_A3_uninitialize();
void uart_A3_enable_uart_A3_tx();
void uart_A3_disable_uart_A3_tx();
void uart_A3_initialize();

#endif
