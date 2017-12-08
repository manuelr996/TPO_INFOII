/*******************************************************************************************************************************//**
 *
 * @file		DR_ElectroValvula.c
 * @brief		Descripcion del modulo
 * @date		30 nov. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_ElectroValvula.h"

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
uint32_t RIEGO_Ttime;
uint32_t RIEGO_AtimeON;
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
	\fn  EstadoElectroValvula
	\brief Devuelve el estado actual de la electro-valvula
 	\author Tomás Bautista Ordóñez
 	\date 30 nov. 2017
*/
uint8_t EstadoElectroValvula ( void )
{
	return GetPIN( VALVULA , ALTO );
}

/**
	\fn  IniciarCicloDeRiego
	\brief Inicia un ciclo de riego temporizado
 	\author Tomás Bautista Ordóñez
 	\date 30 nov. 2017
*/
void IniciarCicloDeRiegoTemporizado ( void )
{
	ElectroValvula_On();
	TimerStart( RIEGO_Tevent , RIEGO_Ttime , ElectroValvula_Off , RIEGO_Tbase );
}

/**
	\fn  DetenerCicloDeRiego
	\brief Detiene el ciclo de riego
 	\author Tomás Bautista Ordóñez
 	\date 30 nov. 2017
*/
void DetenerCicloDeRiegoTemporizado ( void )
{
	ElectroValvula_Off();
	TimerStop( RIEGO_Tevent );
}

/**
	\fn  IniciarCicloDeRiegoAutomatico
	\brief Inicia el ciclo de riego para el modo automatico
 	\author Tomás Bautista Ordóñez
 	\date 30 nov. 2017
*/
void IniciarCicloDeRiegoAutomatico ( void )
{
	EventoAutomatico_1();
}

/**
	\fn  DetenerCicloDeRiegoAutomatico
	\brief Detiene el ciclo de riego para el modo automatico
 	\author Tomás Bautista Ordóñez
 	\date 30 nov. 2017
*/
void DetenerCicloDeRiegoAutomatico ( void )
{
	ElectroValvula_Off();
	TimerStop( RIEGO_Aevent );
}
/**
	\fn  EventoAutomatico_1
	\brief Evento que enciende la valvula y temporiza su apagado
 	\author Tomás Bautista Ordóñez
 	\date 30 nov. 2017
*/
void EventoAutomatico_1 ( void )
{
	ElectroValvula_On();
	TimerStart( RIEGO_Aevent , RIEGO_AtimeON , EventoAutomatico_2 , RIEGO_Abase );
}
/**
	\fn  EventoAutomatico_2
	\brief Evento que apaga la valvula y temporiza su encendido
 	\author Tomás Bautista Ordóñez
 	\date 30 nov. 2017
*/
void EventoAutomatico_2 ( void )
{
	ElectroValvula_Off();
	TimerStart( RIEGO_Aevent , RIEGO_AtimeOFF , EventoAutomatico_1 , RIEGO_Abase );
}
