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
#include "DR_UART.h"
#include "DR_gpio.h"
#include "DR_pinsel.h"
//#include "DR_EINT.h"
#include "DR_PLL.h"
#include"DR_Inicializacion.h"
#include"Aplicacion.h"
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
uint8_t BufferRx[MAX_BUFF_RX];
uint8_t BufferTx[MAX_BUFF_TX];
uint8_t	inRX,outRX,inTX,outTX;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
uint8_t flagTx;

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/


void UART1_IRQHandler (void)
{
	uint8_t iir, aux;
	int16_t datoTx;

	do
	{
		//IIR es reset por HW, una vez que lo lei se resetea.
		iir = UART1IIR;

		if ( iir & 0x04 ) 							//Data ready
		{
			//Display_lcd ("LLEGO msj -UART0", 0 , 0);
			aux = UART1RBR;
			PushRX ( aux );   	//guardo en buffer e incremento índice
									//garantizo el buffer circular
		}
		if ( iir & 0x02 ) //THRE
		{
			datoTx = PopTX();

			if ( datoTx != -1 )
				UART1THR = (uint8_t) datoTx;
			else
				flagTx = 0;

		}
	}
	while( ! ( iir & 0x01 ) ); /* me fijo si cuando entre a la ISR habia otra
						     	int. pendiente de atencion: b0=1 (ocurre unicamente si dentro del mismo
								espacio temporal lleguan dos interrupciones a la vez) */

}



/**
	\fn  void PushRX( uint8_t dato )
	\brief pone un Byte en el buffer de recepcion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
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
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
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
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param [in] uint8_t dato Dato a guardar
	\return void
*/
void PushTX( uint8_t dato )
{
	if ( !flagTx )
	{	flagTx = 1;
		UART1THR = dato;
		return;
	}

	BufferTx[ inTX ] = dato;
	inTX ++;
	inTX %= MAX_BUFF_TX;
}

/**
	\fn int16_t PopTX( void )
	\brief saca un Byte en el buffer de transmicion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
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


void InitUART1 ( void )
{
	//TODO: Completar Inicializacion de UART1
	//1.- Registro PCONP: Energizo la UART:
	// Como amanece encendida no sería en ppo necesario, pero se la activara de cualquier mmanera

	  //1.- Registro PCONP (0x400FC0C4) - bit 4 en 1 prende la UART:
		       PCONP |= 0x00000010;  //Pag 63

	//2.- Registro PCLKSEL0 (0x400FC1A8) - selecciono el clk de la UART (recordar que cclk = 100MHz)

		//2.- Registro PCLKSEL0 (0x400FC1A8) - bits 8 y 9 en 0 seleccionan que el clk de la UART1 sea 25MHz--> DIVIDE POR 4:
			        PCLKSEL0 &= ~(0x03<<8); //pAG 56

	//3.- Registro U1LCR (0x4001000C) - transmision de 8 bits, 1 bit de stop, SIN paridad, sin break cond, DLAB = 1:

			//3.- Registro U1LCR (0x4001000C) - transmision de 8 bits, 1 bit de stop, sin paridad, sin break cond, DLAB = 1:
				    UART1LCR =0x00000083; //pAG 326

	//4.- Registros U1DLL (0x40010000) y U1DLM (0x40010004):
				//4.- Registros U1DLL (0x40010000) y U1DLM (0x40010004) en 0xA2:
					UART1DLM = 0x00;   // es el resultado de 25Mhz/(9600*16)---> 162
					UART1DLL = 0xA2;	// lo cargo en ambos registros pAG 320

	//5.- Registros PINSEL0 (0x4002C000) y PINSEL1 (0x4002C004) - habilitan las funciones especiales de los pines:
	//TX1D : P0[15] Hay colocar un 1 en el bit 30 y un cERO en el bit 31



					PINSEL0 |= 0X01 << 30;//pARA PONER UN 1 EN EL BIT 30
					PINSEL0 &= ~(0X01 <<31);//pARA PONER UN 1 EN EL BIT 31


	//RX1D : P0[16]  Hay que colocar un 1 ene l bit 0 y un 0 en el bit 1                         Pag 108

					PINSEL1 |= 0X01 ;      //pARA PONER UN 1 EN EL BIT 0
					PINSEL1 &= ~(0X01 <<1);  //pARA PONER UN 0 EN EL BIT 1

	//6. Habilito las interrupciones (En la UART -IER- y en el NVIC -ISER)

					 // hay que poner el DLAB=0 para habilitar las interrupciones por TX RX;
					 UART1LCR &= ~(0x01<<7);// pongo en cero el bit 7 DLAB=0                          Pag 326
					 UART1IER |= 0X03 ; // bit 0 y 1 del registro U1Ier Habilia int por TX y RX       Pag 322

					 ISER0 |= (0x01<< 6);//  Habilita Interrupcion por UART1 del NVIC  Pag77
					 UART1FCR |=(0x01);     //  habilita la FIFO de TX RX                             Pag 325



}
