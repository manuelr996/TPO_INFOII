/*******************************************************************************************************************************//**
 *
 * @file		PR_MaquinasdeEstados.c
 * @brief		Descripcion del modulo
 * @date		Dec 8, 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_MaquinasdeEstados.h"

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
 	\date Dec 8, 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void InitConfiguracion( void )
{
	ApagarLeds();
	PrenderLed(ROJO);
	IniciarPotenciometro();
	Display_LCD("Configuracion   ", RENGLON_1, 0);
	Display_LCD("OK p/continuar  ", RENGLON_2, 0);

}

void CloseConfiguracion(void)
{
	DetenerPotenciometro();
	//To-Do: Volver al Estado Anterior
}


void InitAutomatico(void)
{
	ApagarLeds();
	PrenderLed(VERDE);
	Display_LCD("Auto    Valv:OFF" , RENGLON_1 , 0 );
	Display_LCD("Pote:   Humedad:" , RENGLON_2 , 0 );
}

void InitManual(void)
{
	ApagarLeds();
	PrenderLed(AZUL);
	//to-do: InitTemporizado();
	Display_LCD("Manual  Valv:OFF" , RENGLON_1 , 0 );
	Display_LCD("Pote:   Humedad:" , RENGLON_2 , 0 );
}

void InitTemporizado(void)
{
	ApagarLeds();
	PrenderLed(ROJO);
	PrenderLed(VERDE);
	Display_LCD("Tempo   Valv:OFF" , RENGLON_1 , 0 );
	Display_LCD("Pote:   Humedad:" , RENGLON_2 , 0 );
}
