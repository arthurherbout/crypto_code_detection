#ifndef _MEGA32_H_
#define _MEGA32_H_

/* Part-Code ISP */
#define DEVTYPE_ISP     0x72
/* Part-Code Boot */
#define DEVTYPE_BOOT    0x73

#define SIG_BYTE1	0x1E
#define SIG_BYTE2	0x95
#define SIG_BYTE3	0x02

#define UART_BAUD_HIGH	UBRRH
#define UART_BAUD_LOW	UBRRL
#define UART_STATUS	UCSRA
#define UART_TXREADY	UDRE
#define UART_RXREADY	RXC
#define UART_DOUBLE	U2X
#define UART_CTRL	UCSRB
#define UART_CTRL_DATA	((1<<TXEN) | (1<<RXEN))
#define UART_CTRL2	UCSRC
#define UART_CTRL2_DATA	((1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0))
#define UART_DATA	UDR

#endif // #ifndef _MEGA32_H_
