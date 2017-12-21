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
uint8_t btn;
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

void (*MaquinaConfiguracion[])(void) = {ConfiguracionInicializada  	//INIT_CONFIGURACION
										,SetHumedadMinimaDecenas	//HUMEDADMINIMA_D
										,SetHumedadMinimaUnidades	//HUMEDADMINIMA_U
										,SetHumedadMaximaDecenas	//HUMEDADMAXIMA_D
										,SetHumedadMaximaUnidades	//HUMEDADMAXMIA_U
										,SetTemporizadorHHD			//TEMPORIZADOR_HH
										,SetTemporizadorHHU			//TEMPORIZADOR_HH
										,SetTemporizadorMMD			//TEMPORIZADOR_MMD
										,SetTemporizadorMMU			//TEMPORIZADOR_MMU
										,SetTemporizadorSSD			//TEMPORIZADOR_SSD
										,SetTemporizadorSSU			//TEMPORIZADOR_SSU
										,SetHoraRiegoHHD			//HORA_RIEGO_HH
										,SetHoraRiegoHHU			//HORA_RIEGO_HH
										,SetHoraRiegoMMD			//HORA_RIEGO_MMD
										,SetHoraRiegoMMU			//HORA_RIEGO_MMU
										,SetHoraRiegoSSD			//HORA_RIEGO_SSD
										,SetHoraRiegoSSU			//HORA_RIEGO_SSU
										,ConfiguracionFinalizada};	//CERRAR_CONFIGURACION

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
	if(Estado!=CONFIGURACION)
		btn = getTecla();

	switch(Estado)
	{
	case RESET_G:
		Estado = MANUAL;
		btn = B_MANUAL;
		Display_LCD("   Modo Manual  " , RENGLON_1 , 0 );
		Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
		break;
	case CONFIGURACION:
		MaquinaConfiguracion[EstadoConfiguracion]();
		break;
	case MANUAL:
		MaquinaManual[EstadoManual]();
		break;
	case TEMPORIZADO:
		MaquinaTemporizado[EstadoTemporizado]();
		break;
	case AUTOMATICO:
		MaquinaAutomatico[EstadoAutomatico]();
		break;
	default:
		Estado = RESET_G;
		break;
	}

	SwitchEstados(btn);
}
