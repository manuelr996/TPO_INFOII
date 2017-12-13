/*******************************************************************************************************************************//**
 *
 * @file		DR_DS18B20.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		10 de oct. de 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/
	/*to-do:
		Escritura:
		Lectura:
			Una vez envie el comando, uso el capture register para tomar el momento en el que se termine la conversion
			en el momento en el cual se termino la conversion el integrado va a empezar a enviar los datos serializados
			a partir de ese momento uso los match registers para leer una vez cada cierta cantidad de tiempo y tomar ese valor
			se utiliza la misma metodologia que en el anterior, pero tengo que leer 8 veces si o si, y en vez de alternar
			cargo el valor a algun buffer.

		Carga del comando:
			tengo configurar un Match register para que alterne el estado de un pin GPIO
			Ese match tiene que generar una interrupcion para que pueda resetearse en una posicion mas adelantada
			Esto se hace tantas veces tenga que alternar los bits para mandar el comando
			En este caso tengo que enviar el comando 0x44, entonces tengo que alternar 4 veces minimo para generar un 0100 0100
			Inmediatamente despues de que termine de cargar el comando se debe reconfigurar el GPIO como Entrada ya que vamos
			a tener que leer el dato.

	*/

/*Inicializacion:
		-Prendo el Timer
		-Configuro los Registros de Match y sus interrupts
		-Hay que configurar el Sensor?
*/
/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PRIMITIVAS_DR_DS18B20_H_
#define PRIMITIVAS_DR_DS18B20_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_TIMER0123.h"
#include "DR_gpio.h"
#include "DR_pinsel.h"
#include "DR_PCONP.h"
#include "DR_tipos.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

#define uS 			0x64
#define S			1000000*uS

#define tempData  	2,8

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

#define GetTempData 		FIO2PIN & 0x100		//Mascara para leer el pin 8 del puerto 2
#define TempDataInput 		FIO2DIR |= 0x100	//Mascara para Configurar el pin 8 del puerto 2 como input
#define TempDataOutput		FIO2DIR &= ~(0x100)	//Mascara para Configurar el pin 8 del puerto 2 como output
#define W1TempData			FIO2SET |= 0x100	//Prende el pin 8 del puerto 2
#define W0TempData			FIO2CLR |= 0x100	//Apaga el pin 8 del puerto 2

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef enum 
{
	RESET_TX = 0, 		//Generar Reset
	RESET_RX, 			//Leer Presencia
	ROMSKIP_TX, 		//Skip ROM check
	CONV_TX, 			//Generar Convertir Temperatura
	CONV_RX, 			//Esperar Convertir Temperatura
	R_SCRP_TX, 			//Generar Leer Scratchpad
	R_SCRP_RX 			//Leer Scratchpad
}ESTADOS_DS18B20;
 
 typedef enum
 {
	SKIP_ROM = 0xCC,
	CONVERT_TEMPERATURE = 0x44,
	READ_SCRATCHPAD = 0xBE	
 }COMANDOS_DS18B20;
 
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
//extern uint8_t currentCommand;
extern volatile uint32_t tempBuffer;
/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void OWire_Init(void);
void TxCommand(void);
void ResetRx(void);
void ResetTx(void);
void ConvRx(void);
void ReadScratchpad(void);

#endif /* PRIMITIVAS_DR_DS18B20_H_ */
