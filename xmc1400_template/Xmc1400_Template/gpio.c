/**
 * @file gpio.c
 * @Firmware PMSM_FOC_SL_XMC13_V1_2
 * @Modified date: 2017-06-19
 *
 * @cond
 *********************************************************************************************************************
 * PMSM FOC Motor Control Library
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * @file gpio.c
 *
 * @brief Sensorless FOC with 3-shunt <br>
 *
 * <b>Detailed description of file</b> <br>
 *  IDE: Infineon DAVE 4, Version: 4.1.2, Installer build : 2016-09-30
 *  HW:  Infineon PMSM-LV-15W, or XMC 750 Watt Motor Control Application Kit.
 *  MCU: Infineon XMC1302.
 *
 * History
 *
 * @endcond
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "gpio.h"

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/
/**
 *  Data Structure initialization - GPIO Configuration for Gate Driver enable pin .
 */
const XMC_GPIO_CONFIG_t IO_PadConfig_Pushpull  =
{
	.mode            = (XMC_GPIO_MODE_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
	.output_level    = (XMC_GPIO_OUTPUT_LEVEL_t)ENABLE_LEVEL,
	.input_hysteresis= XMC_GPIO_INPUT_HYSTERESIS_STANDARD
};

/* Hall sensor interface configuration */
const XMC_GPIO_CONFIG_t HALL_PadConfig_InputTristate  =
{
	.mode            = (XMC_GPIO_MODE_t)XMC_GPIO_MODE_INPUT_TRISTATE,
	.output_level    = (XMC_GPIO_OUTPUT_LEVEL_t)XMC_GPIO_OUTPUT_LEVEL_LOW,
	.input_hysteresis= XMC_GPIO_INPUT_HYSTERESIS_STANDARD
};

/* GPIO Input interface configuration */
const XMC_GPIO_CONFIG_t Input_PadConfig_InputTristate  =
{
	.mode            = (XMC_GPIO_MODE_t)XMC_GPIO_MODE_INPUT_TRISTATE,
	.output_level    = (XMC_GPIO_OUTPUT_LEVEL_t)XMC_GPIO_OUTPUT_LEVEL_LOW,
	.input_hysteresis= XMC_GPIO_INPUT_HYSTERESIS_STANDARD
};

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* API to initialize GPIO pins used */


