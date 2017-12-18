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
	\fn  GetHumedadSuelo
	\brief Retorna el valor del buffer HumedadSuelo
 	\author Tomás Bautista Ordóñez
 	\date 9 oct. 2017
*/
uint8_t GetHumedadSuelo (void)
{
	return HumedadSuelo;
}
/**
	\fn  GetPotenciometro
	\brief Retorna el valor del buffer vPotenciometro
 	\author Tomás Bautista Ordóñez
 	\date 9 oct. 2017
*/
uint32_t GetPotenciometroHumedad (void)
{
	return (100 - ( vPotenciometro / 41 ));
}

uint32_t GetPotenciometroHora(void)
{
	return vPotenciometro/2.8;
}

/**
	\fn  GetTemperatura
	\brief Retorna el valor del buffer Temperatura
 	\author Tomás Bautista Ordóñez
 	\date 9 oct. 2017
*/
uint8_t GetTemperatura (void)
{
	return Temperatura;
}


//////////////////////Temperatura-Humedad//////////////////////////
void Ev1SecuenciaTempHumedad ( void )
{
	CambiarCanal( S_HUMEDAD );
	DispararConversion();
	TimerStart( ADCevent , ADCTHtime, Ev2SecuenciaTempHUmedad , ADCbase );
}
void Ev2SecuenciaTempHUmedad ( void )
{
	CambiarCanal( S_TEMPERATURA );
	DispararConversion();
	TimerStart( ADCevent , ADCTHtime, Ev1SecuenciaTempHumedad , ADCbase );
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
	TimerStop(ADCevent); 													//Desactivo el disparador del conversor de humedad y temperatura
	CambiarCanal( POTE );													//Potenciometro como objeto de conversion
	TimerStart( ADCevent , ADCPtime , DispararPotenciometro , ADCbase );	//Disparo un timer para iniciar una secuencia de conversion del potenciometro
}

void DispararPotenciometro (void)
{
	DispararConversion();			//Disparo
	SetTimer(ADCevent, ADCPtime);	//Reseteo
}

void DetenerPotenciometro(void)
{
	TimerStop(ADCevent);
	Ev1SecuenciaTempHumedad();
}
