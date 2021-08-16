#include "stdint.h"
#include "uart_app.h"

void can_init(void);
void CAN_Rx_Poll(void);
uint8_t can_send();

uint8_t driver_init() {
	uart_init();
	can_init();
}

int main()
{
	uint32_t u32Count = 0;
	driver_init();
	DEBUG_PRINT("XMC1400 program started\r\n");
	can_send();
	while(1) {
		CAN_Rx_Poll();
	}
	return 0;
}

