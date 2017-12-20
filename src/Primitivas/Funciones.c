/*******************************************************************************************************************************//**
 *
 * @file		Funciones.c
 * @brief		Descripcion del modulo
 * @date		26 oct. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "Funciones.h"

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
	\fn  MostrarSensores
	\brief Funcion que muestra al usuario el estado de los diferentes sensores
 	\author Tomás Bautista Ordóñez
 	\date 26 oct. 2017
*/
void MostrarSensores( void )
{
	uint8_t Humedad = GetHumedadSuelo();
	uint8_t Temp = GetTemperatura();

	char aux[6];


	FuncLluvia();

	Display( Temp , DSP0 );
	Display( Humedad , DSP1 );

	ComponerMedicion( aux , Humedad );
	TransmitirString( aux );

	ComponerMedicion( aux , Temp );
	TransmitirString( aux );

	SetTimer(E_Display,T_Display);
}
/**
	\fn  FuncLluvia
	\brief Refleja al exterior el estado de la variable Lluvia
 	\author Tomás Bautista Ordóñez
 	\date 26 oct. 2017
*/
void FuncLluvia( void )
{
	 if( GetLluvia() == ON )
		LedLluvia_On();
	else
		LedLluvia_Off();
}

void ComponerMedicion(char *string, uint8_t med)
{
	string[0] = '#';
	string[1] = 'h';
	string[2] = (med/10) + '0';
	string[3] = (med%10) + '0';
	string[4] = '$';
}
