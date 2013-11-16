
#include "uart_A3.hpp"

__UART__ uart_A3;

#define DEFAULT_BAUDRATE 9600

#pragma vector=USCI_A3_VECTOR
__interrupt void uart_A3_interrupt_vector(void)
{
	uint8_t c;
	uint8_t *c_p;
	c_p = &c;
	switch((UCA3IV))
	{
		case 0: break;
			// Vector 0 - no interrupt
	
		case 2:
			// Vector 2 - RXIFG
			c = UCA3RXBUF;
			uart_A3.__on_rx_receive(c);
			break;
		
		case 4:
			if( uart_A3.__on_tx_receive(&c))
			{
				// Bytes available to transmit
				UCA3TXBUF = c;
			}
			else
			{
				// No bytes available to transmit
				UCA3IE &= ~(UCTXIE);
				UCA3IFG &= ~(UCTXIFG);
			}
			break;
		
		default:
			break;
		// Vector 4 - TXIFG
	}
}

void uart_A3_enable_uart_A3_interrupts()
{
	UCA3IE |= UCTXIE + UCRXIE;
}


void uart_A3_disable_uart_A3_interrupts()
{
	UCA3IE &= ~(UCTXIE+UCRXIE);
}

void uart_A3_configure(uint16_t baud = 9600, PARITY par = EVEN_PARITY)
{
    float n, x;
    P10DIR = P10DIR | 1<<4;           //set P10.4=1 (Transmitter) as output
    P10DIR = P10DIR & ~(1<<5);        //set P10.5=0 (Receiver) as input
    P10SEL = P10SEL | 1<<4 | 1<<5;    //set P10.4=1 & P10.5=1 as TX/RX

    UCA3CTL1 = UCA3CTL1 | 0x01;     //UCSWRST = 1 (bit0 = 1)   //page 417
    UCA3CTL1 = (UCA3CTL1 | 1<<7) & ~(1<<6);       //select SMCLK as BaudRate source clock (bit6 = 1; bit7 = 1)

    //calculate registers for the BaudRate +

    UCA3MCTL = UCA3MCTL | 0x01;       //UCOS16 = 1
    //calculate UCBR
    n = configCPU_CLOCK_HZ  /baud;
    UCA3BR0 = (int)(n/16) % 256;
    UCA3BR1 = (int)(n/16) / 256;

    //caluclate UCBRS
    x = (int)((n-(int)(n))*8+0.5);
    UCA3MCTL = UCA3MCTL | (int)(x)<<1;

    //caluclate UCBRF
    x = (int)(((n/16 - (int)(n/16))*16) + 0.5);
    UCA3MCTL = UCA3MCTL | (int)(x)<<4;

    //calculate registers for the BaudRate -

    UCA3CTL0 = 0x00;                  //Set Format Frame: 8 bit data; no parity; 1 stop bit

    UCA3CTL1 = UCA3CTL1 & ~(0x01);    //UCSWRST = 0 (bit0 = 0) //page 417

}

void uart_A3_uninitialize()
{
    float n, x;
    P10DIR = P10DIR | 1<<4;           //set P10.4=1 (Transmitter) as output
    P10DIR = P10DIR & ~(1<<5);        //set P10.5=0 (Receiver) as input
    P10SEL = P10SEL | 1<<4 | 1<<5;    //set P10.4=1 & P10.5=1 as TX/RX


    UCA3CTL1 = UCA3CTL1 | 0x01;     //UCSWRST = 1 (bit0 = 1)   //page 417

    UCA3CTL1 = (UCA3CTL1 | 1<<7) & ~(1<<6);       //select SMCLK as BaudRate source clock (bit6 = 1; bit7 = 1)

    //calculate registers for the BaudRate +

    UCA3MCTL = UCA3MCTL | 0x01;       //UCOS16 = 1
    //calculate UCBR
    n = configCPU_CLOCK_HZ  /DEFAULT_BAUDRATE;
    UCA3BR0 = (int)(n/16) % 256;
    UCA3BR1 = (int)(n/16) / 256;

    //caluclate UCBRS
    x = (int)((n-(int)(n))*8+0.5);
    UCA3MCTL = UCA3MCTL | (int)(x)<<1;

    //caluclate UCBRF
    x = (int)(((n/16 - (int)(n/16))*16) + 0.5);
    UCA3MCTL = UCA3MCTL | (int)(x)<<4;

    //calculate registers for the BaudRate -

    UCA3CTL0 = 0x00;                  //Set Format Frame: 8 bit data; no parity; 1 stop bit

    UCA3CTL1 = UCA3CTL1 & ~(0x01);    //UCSWRST = 0 (bit0 = 0) //page 417
}

void uart_A3_enable_uart_A3_tx()
{
	uart_A3_enable_uart_A3_interrupts();
	UCA3IE |= UCTXIE;
	UCA3IFG |= UCTXIFG;
}

void uart_A3_disable_uart_A3_tx()
{
	UCA3IE &= ~UCTXIE;
	UCA3IFG &= ~UCTXIFG;
}


void uart_A3_initialize()
{
	uart_A3.enable_uart_interrupts = uart_A3_enable_uart_A3_interrupts;
	uart_A3.disable_uart_interrupts = uart_A3_disable_uart_A3_interrupts;
	uart_A3.configure = uart_A3_configure;
	uart_A3.enable_tx = uart_A3_enable_uart_A3_tx;
	uart_A3.disable_tx = uart_A3_disable_uart_A3_tx;
}


