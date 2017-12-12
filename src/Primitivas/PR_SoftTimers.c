/*******************************************************************************************************************************//**
 *
 * @file		PR_SoftTimers.c
 * @brief		Descripcion del modulo
 * @date		23 jul. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_SoftTimers.h"

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
volatile 	uint32_t Tmr_Run[ N_TIMERS ];
volatile 	uint8_t  TMR_Events[ N_TIMERS ];
void 	 	(* TMR_handlers [N_TIMERS]) (void);
volatile 	uint8_t  TMR_StandBy[ N_TIMERS ];
volatile 	uint8_t  Tmr_Base[ N_TIMERS ];
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
	\fn 		void TimerStart(uint8_t event, uint32_t time, void (*handler)(void))
	\brief 		Inicia un timer
	\author 	Tomás Bautista Ordóñez
 	\details 	Inicia el timer e al transcurrir el tiempo
 				especificado por time se llama a la funcion
 				apuntada por handler
 	\param [in] event:		Numero de evento
 	\param [in] time:		Tiempo del evento. Dependiente de la base de tiempos
 	\param [in] handler:	Callback del evento
	\return 	void
*/
void TimerStart(uint8_t event, uint32_t time, Timer_Handler handler , uint8_t base )
{
	switch ( base )
	{
		case DEC:
			time *= DECIMAS;
			break;
		case SEG:
			time *= SEGUNDOS;
			break;
		case MIN:
			time *= MINUTOS;
			break;
	}

	Tmr_Run[event] = time;
	Tmr_Base[event] = base;

	if(time)
		TMR_Events[event] = 0;

	else
		TMR_Events[event] = 1;

	TMR_handlers[event] = handler;
}

/**
	\fn 		void SetTimer( uint8_t event , uint32_t time )
	\brief 		Inicia un timer
	\author 	Tomás Bautista Ordóñez
 	\details 	Reinicia el timer con el valor time (no lo resetea)
 	\param [in] event: 	Numero de evento
 	\param [in] time: 	Tiempo del evento. Dependiente de la base de tiempos
 	\return 	void
*/
void SetTimer( uint8_t event, uint32_t time )
{
	switch ( Tmr_Base[event] )
	{
		case DEC:
			time *= DECIMAS;
			break;
		case SEG:
			time *= SEGUNDOS;
			break;
		case MIN:
			time *= MINUTOS;
			break;
	}
	Tmr_Run[event] = time;
}

/**
	\fn  		GetTimer( uint8_t event )
	\brief 		Toma el valor al vuelo del timer en cuestion
	\author 	Tomás Bautista Ordóñez
 	\details 	Lee el timer
 	\param [in] event: Numero de evento
 	\return 	valor del timer
*/
uint32_t GetTimer( uint8_t event )
{
	uint32_t tiempo = Tmr_Run[event];

	switch ( Tmr_Base[event] )
	{
		case DEC:
			tiempo /= DECIMAS;
			break;
		case SEG:
			tiempo /= SEGUNDOS;
			break;
		case MIN:
			tiempo /= MINUTOS;
			break;
	}
	return tiempo;
}

/**
	\fn  		StandByTimer( uint8_t event , uint8_t accion)
	\brief 		Detiene/Arranca el timer, NO lo resetea
	\author 	Tomás Bautista Ordóñez
 	\details 	lo pone o lo saca de stand by
 	\param [in] event: 	Numero de evento entre 0 y 31
 	\param [in] accion: RUN lo arranca, PAUSE lo pone en stand by
 	\return 	void
*/
void StandByTimer( uint8_t event , uint8_t accion)
{
	TMR_StandBy[ event ] = accion;
}

/**
	\fn 		void Timer_Stop(uint8_t event)
	\brief 		Detiene un timer
	\author 	Tomás Bautista Ordóñez
 	\details 	Detiene el timer event
 	\param [in] event: Numero de evento
	\return 	void
*/
void TimerStop(uint8_t event)
{
	Tmr_Run[ event ] = 0;
	TMR_Events[ event ] = 0;
	TMR_handlers[ event ] = NULL;
	Tmr_Base[ event ] = 0;
	TMR_StandBy[ event ] = RUN;
}

/**
	\fn 		void Timer_Close(void)
	\brief 		Detiene los timers
	\author 	Tomás Bautista Ordóñez
 	\details 	Detiene todos los timers
	\return 	void
*/
void TimerClose(void)
{
	uint32_t i;

	for( i=0 ; i < N_TIMERS ; i++ )
		TimerStop( i );
}
