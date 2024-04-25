/* uart.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_UART_H
#define RDF_MCAL_UART_H

#include <stm32f4xx.h>
#include <inttypes.h>
#include <stdbool.h>

#define UART_DR_MASK 0x1FF

void uart_setup(USART_TypeDef *usart, uint64_t baud);
void uart_setup_pins(USART_TypeDef *usart);
void uart_enable_clock(USART_TypeDef *usart);
void uart_set_baudrate(USART_TypeDef *usart, uint64_t baud);
void uart_enable(USART_TypeDef *usart);
void uart_enable_isr(USART_TypeDef *usart);
void uart_disable_isr(USART_TypeDef *usart);

uint32_t uart_write(USART_TypeDef *usart, uint8_t *data, uint32_t length);
void uart_write_byte(USART_TypeDef *usart, uint8_t byte);
void uart_read(USART_TypeDef *usart, uint8_t *data, uint32_t length); // blocking
uint8_t uart_read_byte(USART_TypeDef *usart);
bool uart_data_available(USART_TypeDef *usart);
void uart_wait_transmit_ready(USART_TypeDef *usart);
void uart_wait_receive_ready(USART_TypeDef *usart);

#endif // RDF_MCAL_UART_H
