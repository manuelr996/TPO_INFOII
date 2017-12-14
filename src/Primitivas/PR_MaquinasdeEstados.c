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
uint8_t EstadoAnterior;
/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/

void SwitchEstados(EstadosGenerales nuevoEstado)
{
	switch(nuevoEstado)
	{
	case RESET_G:
		Estado = RESET_G;
	case AUTOMATICO:
		InitAutomatico();
		Estado = AUTOMATICO;
		EstadoAutomatico = NO_REGANDO;
		btn = NO_KEY;
		break;
	case CONFIGURACION:
		InitConfiguracion();
		Estado = CONFIGURACION;
		EstadoConfiguracion = INIT_CONFIGURACION;
		btn = NO_KEY;
		break;
	case MANUAL:
		InitManual();
		Estado = MANUAL;
		EstadoManual = RIEGO_OFF;
		btn = NO_KEY;
		break;
	case TEMPORIZADO:
		InitTemporizado();
		Estado = TEMPORIZADO;
		EstadoTemporizado = AGUARDANDO_OK;
		btn = NO_KEY;
		break;
	}
}

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
	TimerStop(E_Riego);
	TimerStop(E_Potenciometro);
	IniciarPotenciometro();
	Display_LCD("Configuracion   ", RENGLON_1, 0);
	Display_LCD("OK p/continuar  ", RENGLON_2, 0);
	EstadoAnterior = Estado;

}

void CloseConfiguracion(void)
{
	SwitchEstados(EstadoAnterior);
}


void InitAutomatico(void)
{
	CloseEstados();
	PrenderLed(VERDE);
	Display_LCD("Modo Automatico " , RENGLON_1 , 0 );
	Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
}

void InitManual(void)
{
	CloseEstados();
	PrenderLed(AZUL);
	TimerStop(E_Riego);
	Display_LCD("   Modo Manual  " , RENGLON_1 , 0 );
	Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
}

void InitTemporizado(void)
{
	CloseEstados();
	PrenderLed(ROJO);
	PrenderLed(VERDE);
	Display_LCD("Modo Temporizado" , RENGLON_1 , 0 );
	Display_LCD("  Timer:  OFF   " , RENGLON_2 , 0 );
}

void CloseEstados(void)
{
	EV_RIEGO_OFF;
	ApagarLeds();
	TimerStop(E_Riego);
	TimerStop(E_Potenciometro);
	DetenerPotenciometro();
}
