/*******************************************************************************************************************************//**
 *
 * @file		MaquinasdeEstados.c
 * @brief		Descripcion del modulo
 * @date		Dec 8, 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "MaquinasdeEstados.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define B_Riego MIN;

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
EstadosConfiguracion EstadoConfiguracion;
EstadosManual EstadoManual;
EstadosTemporizado EstadoTemporizado;
EstadosAutomatico EstadoAutomatico;
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
uint8_t btn;

uint8_t T_Riego;
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
 	\date Dec 8, 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/


///////////////////////////////////MANUAL///////////////////////////////////
void RiegoOn ( void )
{
	//btn = getTecla();
	EV_RIEGO_ON;
	if(btn == B_OK)
	{
		Display_LCD( "Off" , RENGLON_1 , 13 );
		EstadoManual = RIEGO_OFF;
	}
}

void RiegoOff ( void )
{
	//btn = getTecla();
	EV_RIEGO_OFF;
	if(btn == B_OK)
	{
		Display_LCD( "On " , RENGLON_1 , 13 );
		EstadoManual = RIEGO_ON;
	}
}
/////////////////////////////////AUTOMATICO/////////////////////////////////
////////////////////////////////CONFIGURACION///////////////////////////////
void ConfiguracionInicializada (void)
{
	btn = getTecla();
	if(btn == B_OK)
	{
		//DisplayLCD: "Configurando Humedad Minima\nHumedad Minima: %%%"
		EstadoConfiguracion = HUMEDADMINIMA;
	}
}

void SetHumedadMinima (void)
{
	//Display LCD: (actualizar los %%% para que muestren el valor del potenciometro)
	btn = getTecla();
	if(btn == B_OK)
	{
		HumedadMinima = vPotenciometro;
		//DisplayLCD: "Configurando Humedad Minima\nHumedad Maxima: %%%"
		EstadoConfiguracion = HUMEDADMAXIMA;
	}
}

void SetHumedadMaxima (void)
{
	//Display LCD: (actualizar los %%% para que muestren el valor del potenciometro)
	btn = getTecla();
	if(btn == B_OK)
	{
		HumedadMaxima = vPotenciometro;
		//Display LCD: "Configurando Temporizador:\n Tiempo: %%%m
		EstadoConfiguracion = TEMPORIZADOR;
	}
}

void SetTemporizador(void)
{
	//Display LCD: (actualizar los %%% para que muestren el valor del potenciometro)
	btn = getTecla();
	if(btn == B_OK)
	{
		T_Riego = vPotenciometro;
		//Display LCD: "Cerrando Configuraciones Presione OK para continuar
		EstadoConfiguracion = CERRAR_CONFIGURACION;
	}
}

void ConfiguracionFinalizada (void)
{
	btn = getTecla();
	if(btn == B_OK)
	{
		CloseConfiguracion();
	}
}
////////////////////////////////TEMPORIZADO////////////////////////////////
