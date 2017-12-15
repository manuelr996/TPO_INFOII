/*******************************************************************************************************************************//**
 *
 * @file		PR_SoftTimers.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		23 jul. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_SOFTTIMERS_H_
#define PR_SOFTTIMERS_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_tipos.h"
#include "DR_SoftTimers.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define 	DEC			(uint8_t)0
#define 	SEG			(uint8_t)1
#define 	MIN			(uint8_t)2
#define 	CEN			(uint8_t)3

#define		CENTESIMAS	(uint8_t)4
#define 	DECIMAS		CENTESIMAS*10
#define 	SEGUNDOS	DECIMAS*10
#define 	MINUTOS		SEGUNDOS*60

#define 	RUN			(uint8_t)0
#define 	PAUSE		(uint8_t)1

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef void (*Timer_Handler)(void);

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

void 		TimerStart(uint8_t event, uint32_t time, Timer_Handler handler , uint8_t base );
void 		SetTimer( uint8_t event, uint32_t time );
uint32_t 	GetTimer( uint8_t event );
void 		StandByTimer( uint8_t event , uint8_t accion);
void 		TimerStop(uint8_t event);
void 		TimerClose(void);

#endif /* PR_SOFTTIMERS_H_ */
