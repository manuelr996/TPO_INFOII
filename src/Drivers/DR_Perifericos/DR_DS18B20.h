/*******************************************************************************************************************************//**
 *
 * @file		DR_DS18B20.h
 * @brief		Drivers del sensor One Wire DS18B20
 * @date		10 de oct. de 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

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

#define uS 			0x64		//un Microsegundo en ticks del timer
#define S			1000000*uS	//un segundo en ticks del timer

#define tempData  	2,8			//puerto y bit del bus del sensor de temperatura

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
	SKIP_ROM = 0xCC,			//Comando para realizar un skip del ROM
	CONVERT_TEMPERATURE = 0x44,	//Comando para solicitar la conversion de la temperatura
	READ_SCRATCHPAD = 0xBE		//Comando para solicitar la lectura de una temperatura ya convertida
 }COMANDOS_DS18B20;
 
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
//extern uint8_t currentCommand;
extern volatile uint32_t tempBuffer;	//Buffer donde se almacena la ultima temperatura convertida
/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void OWire_Init(void);					//Funcion que se encarga de inicializar el Driver One Wire
void TxCommand(void);					//Funcion que realiza la Transmision de un bit del comando
void ResetRx(void);						//Funcion que realiza la recepcion de la señal de vida del Sensor
void ResetTx(void);						//Funcion que realiza la Transmision de un pulso de reset al sensor
void ConvRx(void);						//Funcion que realiza la recepcion de la señal que indica que se finalizo la conversion de una temperatura
void ReadScratchpad(void);				//Funcion que realiza la carga del buffer de la ultima temperatura convertida

#endif /* PRIMITIVAS_DR_DS18B20_H_ */
