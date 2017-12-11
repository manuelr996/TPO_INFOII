/*******************************************************************************************************************************//**
 *
 * @file		MaquinaGeneral.c
 * @brief		Descripcion del modulo
 * @date		Dec 8, 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "MaquinaGeneral.h"

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
EstadosGenerales Estado;

void (*MaquinaConfiguracion[])(void) = {ConfiguracionInicializada, SetHumedadMinima, SetHumedadMaxima, SetTemporizador, ConfiguracionFinalizada};
void (*MaquinaManual[])(void) = {RiegoOff, RiegoOn};
void (*MaquinaTemporizado[])(void) = {AguardandoOk, RiegoTemporizado};
void (*MaquinaAutomatico[])(void) = {RiegoAutomaticoOn, RiegoAutomaticoOff};
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
void MaquinaGeneral (void)
{
	//uint8_t btn = NO_KEY;

	btn = getTecla();

	switch(Estado)
	{
	case RESET_G:
		Estado = MANUAL;
		btn = B_MANUAL;
		Display_LCD("        Valv:OFF" , RENGLON_1 , 0 );
		Display_LCD("Pote:   Humedad:" , RENGLON_2 , 0 );
		break;
	case CONFIGURACION:
		MaquinaConfiguracion[EstadoConfiguracion];
		break;
	case MANUAL:
		MaquinaManual[EstadoManual]();
		break;
	case TEMPORIZADO:
		MaquinaTemporizado[EstadoTemporizado];
		break;
	case AUTOMATICO:
		MaquinaAutomatico[EstadoAutomatico];
		break;
	default:
		Estado = RESET_G;
		break;
	}

	switch(btn)
	{
	case B_AUTOMATICO:
		InitAutomatico();
		Estado = AUTOMATICO;
		EstadoAutomatico = NO_REGANDO;
		btn = NO_KEY;
		break;
	case B_CONFIGURACION:
		InitConfiguracion();
		Estado = CONFIGURACION;
		EstadoConfiguracion = INIT_CONFIGURACION;
		btn = NO_KEY;
		break;
	case B_MANUAL:
		InitManual();
		Estado = MANUAL;
		EstadoManual = RIEGO_OFF;
		btn = NO_KEY;
		break;
	case B_TEMPORIZADO:
		InitTemporizado();
		Estado = TEMPORIZADO;
		EstadoTemporizado = AGUARDANDO_OK;
		btn = NO_KEY;
		break;
	}
}
