/*******************************************************************************************************************************//**
 *
 * @file		PR_ADC.c
 * @brief		Descripcion del modulo
 * @date		3 de dic. de 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_ADC.h"

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
 	\author Manuel A. Rafaele
 	\date 3 de dic. de 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
/////////////////////////SENSOR HUMEDAD////////////////////////////
/**
	\fn  leerHumedad
	\brief realiza una lectura de la humedad y redispara el conversor
 	\author Tomás Bautista Ordóñez
 	\date 9 oct. 2017
 	\param void
	\return void
*/
void leerHumedad( void )
{
	//Disparamos la conversion
	ADCSTART =	STOP;
	ADCSTART =	TRIGGER;

	//Reseteo el timer
	SetTimer( ADCevent , ADCtime );

//	printf("Humedad: %d\n",HumedadSuelo);
}
////////////////////////POTENCIOMETRO//////////////////////////////
/**
	\fn  IniciarPotenciometro
	\brief Cambia entre el potenciometro y el sensor de humedad
 	\author Manuel A. Rafaele
 	\date 3 de dic. de 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void IniciarPotenciometro(void)
{
	TimerStop(ADCevent); 	//desactivo el disparador del conversor de Humedad
	CambiarCanal(0);
	TimerStart( ADCevent , ADCtime , leerPotenciometro , ADCbase );
}

void leerPotenciometro(void)
{
	ADCSTART = STOP;
	ADCSTART = TRIGGER;
	
	SetTimer(ADCevent, ADCtime);
	
//	printf("Potenciometro: %d\n", vPotenciometro);
}

void DetenerPotenciometro(void)
{
	TimerStop(ADCevent);
	CambiarCanal(2);
	TimerStart(ADCevent, ADCtime, leerHumedad, ADCbase);
}

