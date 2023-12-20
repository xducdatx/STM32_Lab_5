/*
 * uart.c
 *
 *  Created on: Dec 19, 2023
 *      Author: datph
 */

#ifndef SRC_UART_C_
#define SRC_UART_C_
#include "uart.h"

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

uint8_t data_receive[MAX_BUFFER_SIZE]; //SAVE DATA TYPED FROM KEYBOARD
uint8_t data_receive_idx = 0; // INDEX OF ARRAY data_receive

uint8_t data_transmit[MAX_BUFFER_SIZE]; // DATA TO TRANSMIT TO TERMINAL
uint8_t data[MAX_BUFFER_SIZE]; // SAVE DATA TYPED FROM KEYBOARD
uint8_t data_size; // SIZE OF DATA TRANSMIT
enum CommandParserState
{
	WAIT_FOR_BEGIN_CHAR,
	WRITING_DATA
};
enum CommandParserState data_parser = WAIT_FOR_BEGIN_CHAR;

enum UartCommunicationState
{
	RTS_REQUEST_STATE,
	PROCESSING_STATE,
	OK_RESPONSE_STATE
};
enum UartCommunicationState uart_state = RTS_REQUEST_STATE;

void command_parser_fsm()
{
	switch(data_parser)
	{
		case WAIT_FOR_BEGIN_CHAR:
			if (temp == '!') // CHECK IF IT IS CHAR(!)
			{
				data_parser = WRITING_DATA;
				data_receive_idx = 0;
				memset(data_receive, 0, MAX_BUFFER_SIZE);
			}
			break;
		case WRITING_DATA:
			if (temp == '#') // CHECK IF IT IS CHAR(#)
			{
				data_parser = WAIT_FOR_BEGIN_CHAR;
				memcpy(data, data_receive, MAX_BUFFER_SIZE);
			}
			else
			{
				data_receive[data_receive_idx++] = temp;
			}
			break;
		default:
			break;
	}
}



void uart_communication_fsm()
{
	switch(uart_state)
	{
		case RTS_REQUEST_STATE:
			if (strcmp((char *)data, "RTS") == 0) // CHECK IF str(DATA) is "RTS"
			{
				uart_state = PROCESSING_STATE;
				data_size = sprintf((char *) data_transmit, "\r!ADC=%d#", (int) HAL_ADC_GetValue(&hadc1));
			}
			break;
		case PROCESSING_STATE:
			HAL_UART_Transmit(&huart2, data_transmit, sizeof(data_transmit), 1000);
			uart_state = OK_RESPONSE_STATE;
			setTimer2(300);
			break;
		case OK_RESPONSE_STATE:
			if (strcmp((char *)data, "OK") == 0) // CHECK IF str(DATA) is "OK"
			{
				uart_state = RTS_REQUEST_STATE;
				memset(data, 0, MAX_BUFFER_SIZE);
			}
			if (timer2_flag == 1)
			{
				uart_state = PROCESSING_STATE;
			}
			break;
		default:
			break;
	}
}

#endif /* SRC_UART_C_ */
