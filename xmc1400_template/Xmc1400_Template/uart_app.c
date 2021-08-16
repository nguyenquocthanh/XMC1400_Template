/*
 * uart_app.c
 *
 *  Created on: Nov 20, 2018
 *      Author: nguyenquocthanh
 */

/* P1_2, P1_3: Debug TX, RX */
/* P0_2, P0_3: Custom TX, RX */
#include "uart_app.h"

/* UART configuration */
XMC_UART_CH_CONFIG_t     uart_config =
{
  .baudrate = 115200U,
  .data_bits = 8U,
  .frame_length = 8U,
  .stop_bits = 1U,
  .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};

XMC_UART_CH_CONFIG_t     custom_uart_config =
{
  .baudrate = 115200U,
  .data_bits = 8U,
  .frame_length = 8U,
  .stop_bits = 1U,
  .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};

/* UART Tx and Rx Pins */
XMC_GPIO_CONFIG_t rx_pin_config =
{
  .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7//XMC_GPIO_MODE_INPUT_PULL_UP,
};

XMC_GPIO_CONFIG_t tx_pin_config =
{
 .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7  // Tx output in ALT7
};

/* UART Tx and Rx Pins */
XMC_GPIO_CONFIG_t custom_rx_pin_config =
{
  .mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT9		//XMC_GPIO_MODE_INPUT_PULL_UP,
};

XMC_GPIO_CONFIG_t custom_tx_pin_config =
{
 .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9  	// Tx output in ALT9
};



void uart_init() {

	 /* Configure UART channel */
	XMC_UART_CH_Init(DEBUG_UART_CH, &uart_config);
	/* Configure input multiplexor */
	XMC_UART_CH_SetInputSource(DEBUG_UART_CH,XMC_UART_CH_INPUT_RXD, 0);
	
	/* Enable receive event */
//	XMC_UART_CH_EnableEvent(DEBUG_UART_CH, XMC_UART_CH_EVENT_STANDARD_RECEIVE);
//	/* Set receive interrupt Service request number*/
//	XMC_SCU_SetInterruptControl(IRQ9_IRQn, DEBUG_UART_SERVICE_REQUEST_LINE);
//		/* Enable Interrupt */
//	NVIC_SetPriority(IRQ9_IRQn,20);
//	NVIC_EnableIRQ(IRQ9_IRQn);
	
	/* Start UART channel */
	XMC_UART_CH_Start(DEBUG_UART_CH);
	XMC_GPIO_Init(DEBUG_TXD, &tx_pin_config);
	XMC_GPIO_Init(DEBUG_RXD, &rx_pin_config);

}

void UART_PRINT(char* s)
{
  while (*s) {
	XMC_UART_CH_Transmit(DEBUG_UART_CH, *s);
    s++;
  }
}

void DEBUG_PRINT(char* pszFormat, ...)
{
	va_list vlArgs;

	va_start(vlArgs, pszFormat);

	char szString[STRING_MAX_NO];

	/* Build string */
	if (0 < vsprintf(szString, pszFormat, vlArgs))
	{
		UART_PRINT(szString);
	}
}

uint8_t g_u8Char;
uint16_t Uart_Poll() {
	if((XMC_UART_CH_GetStatusFlag(XMC_UART0_CH1) & XMC_UART_CH_STATUS_FLAG_RECEIVE_FRAME_FINISHED) != 0)
	{	
		uint8_t u8Byte[1];
		u8Byte[0] = (uint8_t)XMC_UART_CH_GetReceivedData(XMC_UART0_CH1);
		DEBUG_PRINT((char*)u8Byte);
		XMC_UART_CH_ClearStatusFlag(XMC_UART0_CH1, (uint32_t)XMC_UART_CH_STATUS_FLAG_RECEIVE_FRAME_FINISHED);
		return g_u8Char;
	}
	return 0;
}


void IRQ9_Handler(void) {

}

