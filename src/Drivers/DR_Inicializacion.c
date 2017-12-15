/*******************************************************************************************************************************//**
 *
 * @file		DR_Inicializacion.c
 * @brief		Descripcion del modulo
 * @date		8 sept. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_Inicializacion.h"

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

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

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
	\fn  Inicializacion
	\brief Da los valores iniciales  y las rutinas necesarias para el correcto funcionamiento del proyecto
 	\author Tomás Bautista Ordóñez
 	\date 8 sept. 2017
*/
void Inicializacion ( void )
{
	Estado = RESET_G;
	Lluvia 	= OFF;
	tecla 	= NO_KEY;
	RIEGO_Ttime = RIEGO_TtimeRESET;
	RIEGO_AtimeON = RIEGO_AtimeRESET;
	HumedadSuelo = HumedadSueloRESET;
	T_Riego = 20;

	InitPLL();			//PLL
	InitSysTick();		//SysTick
	InitADC();			//Conversor Analogo/Digital
	InitExtIntGPIO();  	//Interrupciones de GPIO
	//OWire_Init();		//DS18B20
	InitInfotronic();  	//Sensores, Teclado, LEDs(placa), Relays
	InitLCD ();			//LCD
	InitUART0(); 		// UArt 0
	TimerStart( ADCevent , ADCtime , DispararConversion , ADCbase );	//Inicio un timer para disparar el ADC segun el tiempo determinado
	ElectroValvula_Off();
	//SecuenciaBienvenidaLCD();	//Se inicia una secuencia de bienvenida en el LCD
}
