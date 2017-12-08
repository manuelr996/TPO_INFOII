/*******************************************************************************************************************************//**
 *
 * @file		DR_Infotronic.c
 * @brief		Descripcion del modulo
 * @date		26 nov. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_Infotronic.h"

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
	\fn  InitInfotronic
	\brief Inicializa el hardware del kit infotronic
 	\author Tomás Bautista Ordóñez
 	\date 26 nov. 2017
*/
void InitInfotrinic ( void )
{
	InitTeclado();
	InitRGB();
	InitExp2();
	InitSalidasDigitales();
	TimerStart( ADCevent , ADCtime , leerHumedad , ADCbase );	//Inicio un timer para leer el ADC02 segun el tiempo determinado
}

/**
	\fn  InitTeclado
	\brief Inicializa el teclado de 5x1 ubicado en el infotronic
 	\author Tomás Bautista Ordóñez
 	\date 26 nov. 2017
*/
void InitTeclado ( void )
{
	//Configuro mediante setPINSEL los pines como GPIO
	SetPINSEL( TECLADO_0 , FUNCION_GPIO );
	SetPINSEL( TECLADO_1 , FUNCION_GPIO );
	SetPINSEL( TECLADO_2 , FUNCION_GPIO );
	SetPINSEL( TECLADO_3 , FUNCION_GPIO );
	SetPINSEL( TECLADO_4 , FUNCION_GPIO );

	//Configuro mediante setDIR los pines como entrada
	SetDIR( TECLADO_0 , ENTRADA );
	SetDIR( TECLADO_1 , ENTRADA );
	SetDIR( TECLADO_2 , ENTRADA );
	SetDIR( TECLADO_3 , ENTRADA );
	SetDIR( TECLADO_4 , ENTRADA );

	//Configuro mediante setMODE los pines con una resistencia pull-up
	SetMODE( TECLADO_0 , PULLUP );
	SetMODE( TECLADO_1 , PULLUP );
	SetMODE( TECLADO_2 , PULLUP );
	//SetMODE( TECLADO_3 , PULLUP );	No hace falta, ya tiene pull-up por hardware
	//SetMODE( TECLADO_4 , PULLUP );	No hace falta, ya tiene pull-up por hardware
}
/**
	\fn  InitRGB
	\brief Inicializa el led RGB ubicado en el Infotronic
 	\author Tomás Bautista Ordóñez
 	\date 26 nov. 2017
*/
void InitRGB ( void )
{
	//Configuro mediante setPINSEL los pines como GPIO
	SetPINSEL( LEDrgb_R , FUNCION_GPIO );
	SetPINSEL( LEDrgb_G , FUNCION_GPIO );
	SetPINSEL( LEDrgb_B , FUNCION_GPIO );

	//Configuro mediante setDIR los pines como salida
	SetDIR( LEDrgb_R , SALIDA );
	SetDIR( LEDrgb_G , SALIDA );
	SetDIR( LEDrgb_B , SALIDA );

	//Configuro mediante setMODEOD los pines en modo normal
	SetMODEOD( LEDrgb_R , NORMAL);
	SetMODEOD( LEDrgb_G , NORMAL);
	SetMODEOD( LEDrgb_B , NORMAL);

	//Apago todos los leds
	SetPIN( LEDrgb_B , BAJO );
	SetPIN( LEDrgb_G , BAJO );
	SetPIN( LEDrgb_R , BAJO );
}
