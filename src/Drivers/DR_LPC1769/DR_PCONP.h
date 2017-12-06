/*
 * DR_PCONP.h
 *
 *  Created on: 12 de nov. de 2017
 *      Author: manue
 */

#ifndef DRIVERS_DR_PCONP_H_
#define DRIVERS_DR_PCONP_H_
#include "DR_tipos.h"

typedef struct
{
	union
	{
		__RW uint32_t Register;
		struct
		{
			__R  uint32_t _RESERVED0:1; 		//0	 - RESERVED
			__RW uint32_t _PCTIM0:1; 			//1	 - TIMER0
			__RW uint32_t _PCTIM1:1; 			//2  - TIMER1
			__RW uint32_t _PCUART0:1; 			//3  - UART0
			__RW uint32_t _PCUART1:1; 			//4  - UART1
			__R  uint32_t _RESERVED1:1; 		//5  - RESERVED
			__RW uint32_t _PCPWM1:1; 			//6  - PWM1
			__RW uint32_t _PCI2C0:1; 			//7  - I2C0
			__RW uint32_t _PCSPI:1; 			//8  - SPI
			__RW uint32_t _PCRTC:1; 			//9  - RTC
			__RW uint32_t _PCSSP1:1; 			//10 - SSP1
			__R  uint32_t _RESERVED2:1; 		//11 - RESERVED
			__RW uint32_t _PCADC:1;				//12 - ADC
			__RW uint32_t _PCCAN1:1;			//13 - CAN1
			__RW uint32_t _PCCAN2:1;			//14 - CAN2
			__RW uint32_t _PCGPIO:1;			//15 - GPIO
			__RW uint32_t _PCRIT:1;				//16 - RIT
			__RW uint32_t _PCMCPWM:1;			//17 - MCPWM
			__RW uint32_t _PCQEI:1;				//18 - QEI
			__RW uint32_t _PCI2C1:1;			//19 - I2C1
			__R  uint32_t _RESERVED3:1;			//20 - RESERVED
			__RW uint32_t _PCSSP0:1;			//21 - SSP0
			__RW uint32_t _PCTIM2:1;			//22 - TIMER2
			__RW uint32_t _PCTIM3:1;			//23 - TIMER3
			__RW uint32_t _PCUART2:1;			//24 - UART2
			__RW uint32_t _PCUART3:1;			//25 - UART3
			__RW uint32_t _PCI2C2:1;			//26 - I2C2
			__RW uint32_t _PCI2S:1;				//27 - I2S
			__R  uint32_t _RESERVED4:1;			//28 - RESERVED
			__RW uint32_t _PCGPDMA:1;			//29 - GPDMA
			__RW uint32_t _PCENET:1;			//30 - ETHERNET
			__RW uint32_t _PCUSB:1;				//31 - USB
		}bits;
	};
}pconp_t;

#define PCONP ((pconp_t *)0x400FC0C4UL)

#endif /* DRIVERS_DR_PCONP_H_ */
