/*
 * can.c
 *
 *  Created on: Oct 20, 2018
 *      Author: thanhnguyenquoc
 */
#include "xmc_gpio.h"
#include "stdio.h"
#include "stdarg.h"
#include "xmc_can.h"
#include "uart_app.h"
/** @brief Can MO8 RX Params */
#define CAN_RX_ID_MO8						0x100
#define CAN_RX_ID_MASK_MO8					0x7FF
#define CAN_RX_MESSAGE_OBJECT_8				CAN_MO8

/** @brief Can MO9 RX Params */
#define CAN_RX_ID_MO9						0x300
#define CAN_RX_ID_MASK_MO9					0x7FF
#define CAN_RX_MESSAGE_OBJECT_9				CAN_MO9

/** @brief Can Tx Params */
#define CAN_TX_ID							0x200
#define CAN_TX_ID_MASK						0x0F0
#define CAN_TX_MESSAGE_OBJECT				CAN_MO0


#define CAN1_TXD 				P4_9
#define CAN1_RXD 				P4_8
#define CAN1_STB 				P4_7
#define SYSTICKS_PER_SECOND 	(10U)			/* System tick */
#define CAN_FREQUENCY			(11000000U)
#define CAN_BAUDRATE 			(250000U)
#define CAN_SAMPLE_POINT		(6000U)
#define CAN_SYNCHRONIZATION_JUMP_WIDTH		(3U)


/*CAN Bit time*/
const XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t CAN_BIT_TIMING_Configuration =
{
  .can_frequency = CAN_FREQUENCY,
  .baudrate = CAN_BAUDRATE,
  .sample_point = CAN_SAMPLE_POINT,
  .sjw = CAN_SYNCHRONIZATION_JUMP_WIDTH,
};

/* CAN message= CAN_MO0 */
XMC_CAN_MO_t CAN_MO0_Tx =
{
  .can_mo_ptr = CAN_MO0,
  .can_priority = XMC_CAN_ARBITRATION_MODE_IDE_DIR_BASED_PRIO_2,
  .can_identifier = CAN_TX_ID,
  .can_id_mask= CAN_TX_ID_MASK, //0x5f0
  .can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
  .can_ide_mask = 1,
  .can_data_length = 8,
  .can_data = {0xBEEF, 0x12345678},
  .can_mo_type= XMC_CAN_MO_TYPE_TRANSMSGOBJ
};

/* CAN message= CAN_MO8 */
XMC_CAN_MO_t CAN_MO8_Rx =
{
	.can_mo_ptr = CAN_MO8,
	.can_priority = XMC_CAN_ARBITRATION_MODE_IDE_DIR_BASED_PRIO_2,
	.can_identifier = CAN_RX_ID_MO8,
	.can_id_mask = CAN_RX_ID_MASK_MO8,
	.can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	.can_ide_mask = 1,
	.can_data_length = 8,
	.can_mo_type = XMC_CAN_MO_TYPE_RECMSGOBJ
};

/* CAN message= CAN_MO9 */
XMC_CAN_MO_t CAN_MO9_Rx =
{
	.can_mo_ptr = CAN_MO9,
	.can_priority = XMC_CAN_ARBITRATION_MODE_IDE_DIR_BASED_PRIO_2,
	.can_identifier = CAN_RX_ID_MO9,
	.can_id_mask = CAN_RX_ID_MASK_MO9,
	.can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	.can_ide_mask = 1,
	.can_data_length = 8,
	.can_mo_type = XMC_CAN_MO_TYPE_RECMSGOBJ
};

const XMC_GPIO_CONFIG_t CAN1_TXD_config =
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9
};

const XMC_GPIO_CONFIG_t CAN1_RXD_config =
{
	.mode = XMC_GPIO_MODE_INPUT_TRISTATE
};

void can_init(void)
{
	/* Configure CAN pins*/
	XMC_GPIO_Init(CAN1_TXD, &CAN1_TXD_config);
	XMC_GPIO_Init(CAN1_RXD, &CAN1_RXD_config);
	XMC_GPIO_SetMode(CAN1_STB, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);

	/* Enable CAN tranceiver */
	XMC_GPIO_SetOutputLow(CAN1_STB);

	/* Configure CAN Module*/
	XMC_CAN_Init(CAN, XMC_CAN_CANCLKSRC_MCLK, CAN_FREQUENCY);

	XMC_CAN_NODE_EnableConfigurationChange(CAN_NODE1);

	XMC_CAN_NODE_SetReceiveInput(CAN_NODE1, XMC_CAN_NODE_RECEIVE_INPUT_RXDCC);

	XMC_CAN_NODE_DisableConfigurationChange(CAN_NODE1);

	/* NODE 1 initialization*/
	XMC_CAN_NODE_EnableConfigurationChange(CAN_NODE1);

	XMC_CAN_NODE_NominalBitTimeConfigure(CAN_NODE1, &CAN_BIT_TIMING_Configuration);

	XMC_CAN_NODE_DisableConfigurationChange(CAN_NODE1);

	/* MO4 Configuration - Transmit*/
	XMC_CAN_MO_Config(&CAN_MO0_Tx);

	/* MO2 Configuration - Receive */
	XMC_CAN_MO_Config(&CAN_MO8_Rx);
	XMC_CAN_MO_Config(&CAN_MO9_Rx);

	/* Enable receive event */
	XMC_CAN_MO_EnableEvent(&CAN_MO8_Rx,XMC_CAN_MO_EVENT_RECEIVE);
	XMC_CAN_MO_EnableEvent(&CAN_MO9_Rx,XMC_CAN_MO_EVENT_RECEIVE);

	/* Set receive interrupt Service request number*/
	XMC_SCU_SetInterruptControl(IRQ7_IRQn, XMC_SCU_IRQCTRL_CAN0_SR3_IRQ7);
//	XMC_SCU_SetInterruptControl(IRQ5_IRQn, XMC_SCU_IRQCTRL_CAN0_SR2_IRQ5);

	XMC_CAN_MO_SetEventNodePointer(&CAN_MO8_Rx,XMC_CAN_MO_POINTER_EVENT_RECEIVE,3);
	XMC_CAN_MO_SetEventNodePointer(&CAN_MO9_Rx,XMC_CAN_MO_POINTER_EVENT_RECEIVE,2);

//	NVIC_SetPriority(IRQ7_IRQn,CAN_RX_MO8_PRIORITY);
//	NVIC_EnableIRQ(IRQ7_IRQn);

//	NVIC_SetPriority(IRQ5_IRQn,CAN_RX_MO9_PRIORITY);
//	NVIC_EnableIRQ(IRQ5_IRQn);

	/* Allocate MO in Node List*/
	XMC_CAN_AllocateMOtoNodeList(CAN,1,0);

	XMC_CAN_AllocateMOtoNodeList(CAN,1,8);
//	XMC_CAN_AllocateMOtoNodeList(CAN,1,9);

	XMC_CAN_NODE_ResetInitBit(CAN_NODE1);
}

uint8_t can_send()
{
	uint8_t u8ErrorStatus = 0x00;
	XMC_CAN_MO_SetIdentifier(&CAN_MO0_Tx, CAN_TX_ID);
	CAN_MO0_Tx.can_data[0] = 0x11223344;
	CAN_MO0_Tx.can_data[1] = 0x55667788;
	u8ErrorStatus = XMC_CAN_MO_UpdateData(&CAN_MO0_Tx);
	if(u8ErrorStatus > 0)
	{
		return u8ErrorStatus;
	}
	u8ErrorStatus = XMC_CAN_MO_Transmit(&CAN_MO0_Tx);
	if(u8ErrorStatus > 0)
	{
		return u8ErrorStatus;
	}
	return u8ErrorStatus;
}


void CAN_Rx_Poll(void)
{

	if((XMC_CAN_MO_GetStatus(&CAN_MO8_Rx) & CAN_MO_MOSTAT_RXPND_Msk))
	{
		/* Copy receiving CAN Message */
		XMC_CAN_MO_Receive(&CAN_MO8_Rx);
		DEBUG_PRINT("Recv CAN ID: %04X, Data: %08X%08X", CAN_MO8_Rx.can_identifier, CAN_MO8_Rx.can_data[0], CAN_MO8_Rx.can_data[1]);
		
		/* Handle CAN Msg */
		switch(CAN_MO8_Rx.can_identifier)
		{
			case CAN_RX_ID_MO8:
			{
				/* Do something */
				break;
			}
		}	
		
		/* Reset pending status after receiving each frame */
		XMC_CAN_MO_ResetStatus(&CAN_MO8_Rx, XMC_CAN_MO_RESET_STATUS_RX_PENDING);
	}

}

