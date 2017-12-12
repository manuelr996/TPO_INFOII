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
char vString[4];
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
		Display_LCD("OFF" , RENGLON_1 , 13 );
		EstadoManual = RIEGO_OFF;
	}
}

void RiegoOff ( void )
{
	//btn = getTecla();
	EV_RIEGO_OFF;
	if(btn == B_OK)
	{
		Display_LCD("ON " , RENGLON_1 , 13 );
		EstadoManual = RIEGO_ON;
	}
}
/////////////////////////////////AUTOMATICO/////////////////////////////////
void RiegoAutomaticoOn( void )
{
	EV_RIEGO_ON;
	if(CondicionesFin())
	{
		Display_LCD("OFF" , RENGLON_1 , 13 );
		EstadoAutomatico = NO_REGANDO;
	}
}

void RiegoAutomaticoOff( void )
{
	EV_RIEGO_OFF;
	if(CondicionesInicio())
	{
		Display_LCD("ON " , RENGLON_1 , 13 );
		EstadoAutomatico = REGANDO;
	}
}
////////////////////////////////CONFIGURACION///////////////////////////////

void PrintPotenciometro(void)
{
	itoa(vPotenciometro, vString, 10);
	GuardarMensajeLCD(vString,vString);
	Display_LCD(vString, RENGLON_2, 12);
	SetTimer(E_Potenciometro,T_Potenciometro);
}

void ConfiguracionInicializada (void)
{
	//btn = getTecla();
	if(btn == B_OK)
	{
		Display_LCD("Config HumMin   ", RENGLON_1 , 0 );
		Display_LCD("Humedad Min=   %", RENGLON_2 , 0 );
		TimerStart(E_Potenciometro,T_Potenciometro,PrintPotenciometro,B_Potenciometro);
		EstadoConfiguracion = HUMEDADMINIMA;
	}
}

void SetHumedadMinima (void)
{

	if(btn == B_OK)
	{
		HumedadMinima = vPotenciometro;
		//DisplayLCD: "Configurando Humedad Minima\nHumedad Maxima: %%%"
		Display_LCD( "Config HumMax   " , RENGLON_1 , 0 );
		Display_LCD( "Humedad Max=   %" , RENGLON_2 , 0 );
		EstadoConfiguracion = HUMEDADMAXIMA;
	}
}

void SetHumedadMaxima (void)
{
	if(btn == B_OK)
	{
		HumedadMaxima = vPotenciometro;
		//Display LCD: "Configurando Temporizador:\n Tiempo: %%%m
		Display_LCD( "Config Temporiza" , RENGLON_1 , 0 );
		Display_LCD( "Tiempo=        m" , RENGLON_2 , 0 );
		EstadoConfiguracion = TEMPORIZADOR;
	}
}

void SetTemporizador(void)
{
	if(btn == B_OK)
	{
		T_Riego = vPotenciometro;
		//Display LCD: "Cerrando Configuraciones Presione OK para continuar
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
	}
}

void ConfiguracionFinalizada (void)
{
	//btn = getTecla();
	if(btn == B_OK)
	{
		CloseConfiguracion();
	}
}
////////////////////////////////TEMPORIZADO////////////////////////////////
void PrintTimer (void)
{
	uint8_t vTimer = GetTimer(E_Riego);
	itoa(vTimer,vString,10);
	GuardarMensajeLCD(vString,vString);
	Display_LCD(vString, RENGLON_2, 12);
	SetTimer(E_Potenciometro,T_Potenciometro);
}

void AguardandoOk(void)
{
	if(btn == B_OK)
	{
		EV_RIEGO_ON;
		Display_LCD("Regando - Tiempo", RENGLON_1, 0);
		Display_LCD("Restante:   TTTm", RENGLON_2, 0);
		TimerStart(E_Riego, T_Riego, VolverAguardando, B_Riego);
		TimerStart(E_Potenciometro,T_Potenciometro,PrintTimer,B_Potenciometro);
		EstadoTemporizado = RIEGO_TEMPORIZADO;
	}
}

void VolverAguardando(void)
{
	Display_LCD("No Regando      ", RENGLON_1, 0);
	Display_LCD("                ", RENGLON_2, 0);
	EstadoTemporizado = AGUARDANDO_OK;
	TimerStop(E_Potenciometro);
}

void RiegoTemporizado(void)
{

}
