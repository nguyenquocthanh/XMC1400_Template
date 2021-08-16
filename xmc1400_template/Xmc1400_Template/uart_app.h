/*
 * uart_app.h
 *
 *  Created on: Nov 20, 2018
 *      Author: nguyenquocthanh
 */

#ifndef SOURCES_UART_APP_H_
#define SOURCES_UART_APP_H_

#include "xmc_uart.h"
#include "xmc_gpio.h"
#include "stdio.h"
#include "stdarg.h"
#include "xmc_scu.h"
#define UART_DEBUG

#define STRING_MAX_NO   					(128)        /* Max length of a string (text). */
#define printf 										DEBUG_PRINT

#define UART_Rx_ISR 										IRQ9_Handler
#define SENSOR_UART_Rx_ISR							UART_Rx_ISR

/* */
#define DEBUG_UART_CH			XMC_UART0_CH1
#define DEBUG_TXD					P1_2
#define DEBUG_RXD					P1_3
#define DEBUG_UART_SERVICE_REQUEST_LINE	XMC_SCU_IRQCTRL_USIC0_SR0_IRQ9
/* */
///* */
//#define SENSOR_UART_CH										XMC_UART0_CH1
//#define SENSOR_TXD												P1_2
//#define SENSOR_RXD												P1_3
//#define SENSOR_UART_SERVICE_REQUEST_LINE	XMC_SCU_IRQCTRL_USIC0_SR0_IRQ9
///* */
//#define DEBUG_UART_CH		XMC_UART1_CH0
//#define DEBUG_TXD					P0_2
//#define DEBUG_RXD					P0_3
							
void uart_init();
void UART_PRINT(char* s);
void DEBUG_PRINT(char* pszFormat, ...);

#endif /* SOURCES_UART_APP_H_ */
