#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void config_uart(void);

void write_data(uint8_t data);

uint8_t read_data(uint8_t data);

#endif /* UART_H_ */
