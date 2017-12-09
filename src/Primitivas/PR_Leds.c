/*******************************************************************************************************************************//**
 *
 * @file		PR_Leds.c
 * @brief		Descripcion del modulo
 * @date		8 sept. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_Leds.h"

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
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Tomás Bautista Ordóñez
 	\date 8 sept. 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void ApagarLeds ( void )
{
	SetPIN( LEDrgb_R , BAJO );
	SetPIN( LEDrgb_G , BAJO );
	SetPIN( LEDrgb_B , BAJO );
}

void PrenderLed (RGB_Led color)
{
	switch ( color )
	{
		case AZUL:
			SetPIN( LEDrgb_B , ALTO );
			break;

		case ROJO:
			SetPIN( LEDrgb_R , ALTO );
			break;

		case VERDE:
			SetPIN( LEDrgb_G , ALTO );
			break;
	}
}


