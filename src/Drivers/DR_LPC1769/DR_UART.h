/*******************************************************************************************************************************//**
 *
 * @file		DR_UART.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		12 de oct. de 2017
 * @author		Ing. Marcelo Trujillo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DRIVERLPC_DR_UART_H_
#define DRIVERLPC_DR_UART_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_tipos.h"
#include "DR_pinsel.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define MAX_BUFF_RX		64
#define MAX_BUFF_TX		64


//////!< /////////////		UARTs		///////////////////////////
//////0x40010000UL : Registro de recepcion de la UART0:
#define		DIR_UART0		( ( __RW uint32_t  * ) 0x4000C000UL )

#define		UART0RBR		DIR_UART0[0]	// Registro de Recepción RBR
#define		UART0THR		DIR_UART0[0]	// Registro de Transmisión THR
#define		UART0DLL		DIR_UART0[0]	// Parte baja del divisor de la UART0:
#define		UART0IER		DIR_UART0[1]	// Registro de Habilitación de interrupciones de la UART0:
#define		UART0DLM		DIR_UART0[1]	// Parte Alta del divisor de la UART0:
#define		UART0IIR		DIR_UART0[2]	// Registro de Identificación de interrupciones de la UART0:
#define		UART0LCR		DIR_UART0[3]	// Line CONTROL Register de la UART0:
#define		UART0LSR		DIR_UART0[5]	// Line STATUS Register de la UART0:
#define		UART0FCR        DIR_UART0[2]

//Macros UART0
#define		UART0DLAB_OFF	(U0LCR & 0xEF)
#define		UART0DLAB_ON	(U0LCR | 0x80)

#define 	TX0			0, 2	//Tx de la UART0
#define 	RX0			0, 3	//Rx de la UART0

//0x40010000UL : Registro de recepcion de la UART1:
#define		DIR_UART1		( ( __RW uint32_t  * ) 0x40010000UL )

#define		UART1RBR		DIR_UART1[0]
#define		UART1THR		DIR_UART1[0]
#define		UART1DLL		DIR_UART1[0]
#define		UART1IER		DIR_UART1[1]
#define		UART1DLM		DIR_UART1[1]
#define		UART1IIR		DIR_UART1[2]
#define		UART1LCR		DIR_UART1[3]
#define		UART1LSR		DIR_UART1[5]
#define     UART1FCR        DIR_UART1[2]







/****************************************************  UART0  UART1 **********************************************************
		//UART0:
		//0x4001000CUL : Registro de control de la UART0:
		#define		DIR_U0LCR	( ( uint32_t  * ) 0x4000C00CUL )
		//0x40010014UL : Registro de recepcion de la UART0:
		#define		DIR_U0LSR		( ( uint32_t  * ) 0x4000C014UL )
		//0x40010000UL : Parte baja del divisor de la UART0:
		#define		DIR_U0DLL	( ( uint32_t  * ) 0x4000C000UL )
		//0x40010004UL : Parte alta del divisor de la UART0:
		#define		DIR_U0DLM	( ( uint32_t  * ) 0x4000C004UL )
		//0x40010000UL : Registro de recepcion de la UART0:
		#define		DIR_U0RBR		( ( uint32_t  * ) 0x4000C000UL )
		//0x40010000UL : Registro de transmision de la UART0:
		#define		DIR_U0THR		( ( uint32_t  * ) 0x4000C000UL )
		//0x4000C004UL : Registro habilitacion de interrupciones de la UART0:
		#define		DIR_U0IER	( ( uint32_t  * ) 0x4000C004UL )
		//0x4000C008UL : Registro de control de la FIFO de la UART0:
		#define		DIR_U0FCR	( ( uint32_t  * ) 0x40000C008UL )


		//UART1:
		//0x4001000CUL : Registro de control de la UART1:
		#define		DIR_U1LCR	( ( uint32_t  * ) 0x4001000CUL )
		//0x40010014UL : Registro de recepcion de la UART1:
		#define		DIR_U1LSR		( ( uint32_t  * ) 0x40010014UL )
		//0x40010000UL : Parte baja del divisor de la UART1:
		#define		DIR_U1DLL	( ( uint32_t  * ) 0x40010000UL )
		//0x40010004UL : Parte alta del divisor de la UART1:
		#define		DIR_U1DLM	( ( uint32_t  * ) 0x40010004UL )
		//0x40010000UL : Registro de recepcion de la UART1:
		#define		DIR_U1RBR	( ( uint32_t  * ) 0x40010000UL )
		//0x40010000UL : Registro de transmision de la UART1:
		#define		DIR_U1THR	( ( uint32_t  * ) 0x40010000UL )
		//0x40010004UL : Registro habilitacion de interrupciones de la UART1:
		#define		DIR_U1IER	( ( uint32_t  * ) 0x40010004UL )
		//0x40010008UL : Registro de identificación de la interrupción de la UART1:
		#define		DIR_U1IIR	( ( uint32_t  * ) 0x40010008UL )
		//0x40010008UL : Registro de control de la FIFO de la UART1:
		#define		DIR_U1FCR	( ( uint32_t  * ) 0x40010008UL )


		//0x4001004CUL : Registro de control comunicacion 485:
		#define		DIR_U1RS485CTRL		( ( uint32_t  * ) 0x4001004CUL )
		//0x40010050UL : Registro de direccion 485:
		#define		DIR_U1RS485ADRMATCH	( ( uint32_t  * ) 0x40010050UL )
		//0x40010054UL : Registro de demora entre fin de transmision y fin de RTS:
		#define		DIR_U1RS485DLY	( ( uint32_t  * ) 0x40010054UL )

		//Registros de la UART0:
		#define		U0THR		DIR_U0THR[0]
		#define		U0RBR		DIR_U0RBR[0]
		#define		U0LCR		DIR_U0LCR[0]
		#define		U0LSR		DIR_U0LSR[0]
		#define		U0DLL		DIR_U0DLL[0]
		#define		U0DLM		DIR_U0DLM[0]
        #define		U0IER		DIR_U0IER[0]
		#define		U0RDR		(U0LSR&0x01)
        #define		U0FCR		DIR_U0FCR[0]
		#define		U0THRE		((U0LSR&0x20)>>5)

		//Registros de la UART1:
		#define		U1THR		DIR_U1THR[0]
		#define		U1RBR		DIR_U1RBR[0]
		#define		U1LCR		DIR_U1LCR[0]
		#define		U1LSR		DIR_U1LSR[0]
		#define		U1DLL		DIR_U1DLL[0]
		#define		U1DLM		DIR_U1DLM[0]
		#define		U1IER		DIR_U1IER[0]
		#define		U1IIR		DIR_U1IIR[0]
		#define		U1FCR		DIR_U1FCR[0]

		#define		U1RDR		(U1LSR&0x01)
		#define		U1THRE		((U1LSR&0x20)>>5)

		#define		U1RS485CTRL		DIR_U1RS485CTRL[0]
		#define		U1RS485ADRMATCH	DIR_U1RS485ADRMATCH[0]
		#define		U1RS485DLY		DIR_U1RS485DLY[0]



//
*/




















/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void UART0_Inicializacion (void);
void InitUART0 (void);
void PushRX( uint8_t dato );
int16_t PopRX( void );
void PushTX( uint8_t dato );
int16_t PopTX( void );

void InitUart1A ( void );
void InitUart1B ( void );
void InitUart1C ( void );
#endif /* DRIVERLPC_DR_UART_H_ */
