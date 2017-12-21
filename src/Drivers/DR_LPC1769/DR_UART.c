/*******************************************************************************************************************************//**
 *
 * @file		DR_UART.c
 * @brief		Descripcion del modulo
 * @date		12 de oct. de 2017
 * @author		Ing. Marcelo Trujillo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_PCONP.h"
#include "DR_UART.h"

#include "DR_gpio.h"
#include "DR_pinsel.h"
#include "DR_Infotronic.h"
//#include "DR_EINT.h"
#include "DR_PLL.h"
#include"DR_Inicializacion.h"
//#include "Aplicacion.h"
/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
uint8_t BufferRx[MAX_BUFF_RX] = {0};
uint8_t BufferTx[MAX_BUFF_TX] = {0};
uint8_t	inRX,outRX,inTX,outTX;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
uint8_t flagTx = 0;

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/



/**
	\fn  void PushRX( uint8_t dato )
	\brief pone un Byte en el buffer de recepcion
 	\
 	\param [in] uint8_t dato Dato a guardar
	\return void
*/
void PushRX( uint8_t dato )
{
	BufferRx[ inRX ] = dato;
	inRX ++;
	inRX %= MAX_BUFF_RX;
}

/**
	\fn  int16_t PopRX( void )
	\brief saca un Byte en el buffer de recepcion

 	\param void
	\return int16_t valor del dato o -1 por ERROR
*/
int16_t PopRX( void )
{
	int16_t salida = -1;

	if ( inRX != outRX )
	{
		salida = BufferRx[ outRX ] ;

		outRX ++;
		outRX %= MAX_BUFF_RX;
	}
	return salida;
}
/**
	\fn  void PushTX( uint8_t dato )
	\brief pone un Byte en el buffer de transmicion

 	\param [in] uint8_t dato Dato a guardar
	\return void
*/
void PushTX( uint8_t dato )
{
	if ( !flagTx )
	{
		flagTx = 1;
		UART0THR = dato;
		return;
	}

	BufferTx[ inTX ] = dato;
	inTX ++;
	inTX %= MAX_BUFF_TX;
}

/**
	\fn int16_t PopTX( void )
	\brief saca un Byte en el buffer de transmicion
 	\param void
	\return int16_t valor del dato o -1 por ERROR
*/
int16_t PopTX( void )
{
	int16_t salida = -1;

	if ( inTX != outTX )
	{
		salida = BufferTx[ outTX ] ;

		outTX ++;
		outTX %= MAX_BUFF_TX;
	}
	return salida;
}


//#define		UART0THR		DIR_UART0[0]	// Registro de Transmisión THR
//#define		UART0DLL		DIR_UART0[0]	// Parte baja del divisor de la UART0:
//#define		UART0IER		DIR_UART0[1]	// Registro de Habilitación de interrupciones de la UART0:
//#define		UART0DLM		DIR_UART0[1]	// Parte Alta del divisor de la UART0:
//#define		UART0IIR		DIR_UART0[2]	// Registro de Identificación de interrupciones de la UART0:
//#define		UART0LCR		DIR_UART0[3]	// Line CONTROL Register de la UART0:
//#define		UART0LSR		DIR_UART0[5]	// Line STATUS Register de la UART0:
//#define		UART0FCR        DIR_UART0[2]

void InitUART0 (void)
{

	//1.- Registro PCONP (0x400FC0C4) - bit 3 en 1 prende la UART:
	PCONP->bits._PCUART0=1;
	//2.- Registro PCLKSEL0 (0x400FC1A8) - bits 6 y 7 en 0 seleccionan que el clk de la UART0 sea 25MHz:
	PCLKSEL0 &= ~(0x03<<6);
	//3.- Registro UART0LCR (0x4001000C) - transmision de 8 bits, 1 bit de stop, sin paridad, sin break cond, DLAB = 1:
	UART0LCR = 0x00000083;
	//4.- Registros U1DLL (0x40010000) y U1DLM (0x40010004) - 115200 baudios:
	UART0DLM = 0;
	UART0DLL = 0xA3;//0xD9;
	//5.- Registros PINSEL0 (0x4002C000) y PINSEL1 (0x4002C004) - habilitan las funciones especiales de los pines:
	//TX1D : PIN ??	-> 		P0[2]	-> PINSEL0: 04:05
	SetPINSEL(0,2,PINSEL_FUNC1);
	//RX1D : PIN ??	-> 		P0[3]	-> PINSEL1: 06:07
	SetPINSEL(0,3,PINSEL_FUNC1);
	//6.- Registro U1LCR, pongo DLAB en 0:
	UART0LCR = 0x03;
	//7. Habilito las interrupciones (En la UART -IER- y en el NVIC -ISER)
	UART0IER = 0x03;
	ISER0 |= (1<<5);
}

void UART0_IRQHandler (void)
{
	int16_t iir, dato;
	int8_t aux;
	do
	{
		//IIR es reset por HW, una vez que lo lei se resetea.
		iir = UART0IIR;

		if ( iir & 0x02 ) //THRE
		{
			dato=PopTX();
			if( dato != -1)
				UART0THR = (char)dato;
			else
				flagTx = 0;
		}
		if ( iir & 0x04 ) //Data ready
		{
			aux=UART0RBR;
			PushRX(aux);
		}

	}
	while( ! ( iir & 0x01 ) ); /* me fijo si cuando entre a la ISR habia otra
						     	int. pendiente de atencion: b0=1 (ocurre unicamente si dentro del mismo
								espacio temporal lleguan dos interrupciones a la vez) */
}
