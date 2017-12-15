/*******************************************************************************************************************************//**
 *
 * @file		DR_Boton.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		9 sept. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_RTC_H_
#define DR_RTC_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_PCONP.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
/* RTC */
#define	RTCILR		(*(__RW uint32_t*)0x40024000UL)
#define	RTC_CCR		(*(__RW uint32_t*)0x40024008)
#define	RTC_CIIR	(*(__RW uint32_t*)0x4002400C)
#define	RTC_AMR		(*(_RW uint32_t*)0x40024010)
#define RTC_CTIME	((__RW CTIME* ) 0x40024014UL)
#define	RTC_AUX		(*(volatile uint32_t*)0x4002405C)
#define	RTC_AUXEN	(*(volatile uint32_t*)0x40024058)
#define	RTC_CALIBRATION	(*(volatile uint32_t*)0x40024040)
#define	RTC_GPREG	((volatile uint32_t*)0x40024044)
#define	RTC_GPREG0	RTC_GPREG[0]
#define	RTC_GPREG1	RTC_GPREG[1]
#define	RTC_GPREG2	RTC_GPREG[2]
#define	RTC_GPREG3	RTC_GPREG[3]
#define	RTC_GPREG4	RTC_GPREG[4]
#define RTC_TIMER	((__RW uint32_t *)0x40024020)
#define RTC_SEC		RTC_TIMER[0]
#define RTC_MIN		RTC_TIMER[1]
#define RTC_HOUR	RTC_TIMER[2]
#define RTC_DOM		RTC_TIMER[3]
#define RTC_DOW		RTC_TIMER[4]
#define RTC_DOY		RTC_TIMER[5]
#define RTC_MONTH	RTC_TIMER[6]
#define RTC_YEAR	RTC_TIMER[7]
#define	RTC_ALSEC	(*(volatile uint32_t*)0x40024060)
#define	RTC_ALMIN	(*(volatile uint32_t*)0x40024064)
#define	RTC_ALHOUR	(*(volatile uint32_t*)0x40024068)
#define	RTC_ALDOM	(*(volatile uint32_t*)0x4002406C)
#define	RTC_ALDOW	(*(volatile uint32_t*)0x40024070)
#define	RTC_ALDOY	(*(volatile uint32_t*)0x40024074)
#define	RTC_ALMON	(*(volatile uint32_t*)0x40024078)
#define	RTC_ALYEAR	(*(volatile uint32_t*)0x4002407C)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/
#define	_BV(bit) (1<<(bit))
#define GetFailFlag (RTC_AUX & 0x10)
/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef struct 		//Estructura para las primitivas TODO: Moverse a PR_RTC.h
{
	uint32_t Seconds:6;
	uint32_t Minutes:6;
	uint32_t Hours:5;
	uint32_t DayOfWeek:3;
	uint32_t DayofMonth:5;
	uint32_t Month:4;
	uint32_t Year:12;
} RTC;

typedef struct		//Estructura para registro CTIME
{

	union
	{
		__RW uint32_t Register;
		struct
		{
			__RW uint32_t Seconds:6;
			__R  uint32_t RESERVED_0:2;
			__RW uint32_t Minutes:6;
			__R  uint32_t RESERVED_1:2;
			__RW uint32_t Hours:5;
			__R  uint32_t RESERVED_2:3;
			__RW uint32_t DayOfWeek:3;
			__R  uint32_t RESERVED_3:5;
		}bits;
	}CTIME0;
	union
	{
		__RW uint32_t Register;
		struct
		{
			__RW uint32_t DayOfMonth:5;
			__R  uint32_t RESERVED_0:3;
			__RW uint32_t Month:4;
			__R  uint32_t RESERVED_1:4;
			__RW uint32_t Year:12;
			__R  uint32_t RESERVED_2:4;
		}bits;
	}CTIME1;
	union
	{
		__RW uint32_t Register;
		struct
		{
			__RW uint32_t DayOfYear:12;
			__R  uint32_t RESERVED_0:20;
		}bits;
	}CTIME2;

}CTIME;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void InitRTC ( void );					/* Inicializa al RTC */
uint32_t rtc_gettime ( RTC* );			/* 		Get time	 */
void rtc_settime ( const RTC* );		/*		Set time	 */

#endif /* DR_RTC_H_ */
