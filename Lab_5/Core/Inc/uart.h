/*
 * uart.h
 *
 *  Created on: Dec 19, 2023
 *      Author: datph
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "software_timer.h"

#define MAX_BUFFER_SIZE 30

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

void command_parser_fsm();
void uart_communication_fsm();

#endif /* INC_UART_H_ */
