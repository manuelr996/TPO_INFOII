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
#define	RTCILR		(*(volatile uint32_t*)0x40024000UL)
#define	RTC_CCR		(*(volatile uint32_t*)0x40024008)
#define	RTC_CIIR	(*(volatile uint32_t*)0x4002400C)
#define	RTC_AMR		(*(volatile uint32_t*)0x40024010)
#define	RTC_AUX		(*(volatile uint32_t*)0x4002405C)
#define	RTC_AUXEN	(*(volatile uint32_t*)0x40024058)
#define	RTC_CTIME0	(*(volatile uint32_t*)0x40024014)
#define	RTC_CTIME1	(*(volatile uint32_t*)0x40024018)
#define	RTC_CTIME2	(*(volatile uint32_t*)0x4002401C)
#define	RTC_SEC		(*(volatile uint32_t*)0x40024020)
#define	RTC_MIN		(*(volatile uint32_t*)0x40024024)
#define	RTC_HOUR	(*(volatile uint32_t*)0x40024028)
#define	RTC_DOM		(*(volatile uint32_t*)0x4002402C)
#define	RTC_DOW		(*(volatile uint32_t*)0x40024030)
#define	RTC_DOY		(*(volatile uint32_t*)0x40024034)
#define	RTC_MONTH	(*(volatile uint32_t*)0x40024038)
#define	RTC_YEAR	(*(volatile uint32_t*)0x4002403C)
#define	RTC_CALIBRATION	(*(volatile uint32_t*)0x40024040)
#define	RTC_GPREG	( (volatile uint32_t*)0x40024044)
#define	RTC_GPREG0	(*(volatile uint32_t*)0x40024044)
#define	RTC_GPREG1	(*(volatile uint32_t*)0x40024048)
#define	RTC_GPREG2	(*(volatile uint32_t*)0x4002404C)
#define	RTC_GPREG3	(*(volatile uint32_t*)0x40024050)
#define	RTC_GPREG4	(*(volatile uint32_t*)0x40024054)
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
/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef struct {
	uint16_t	year;	/* 1..4095 */
	uint8_t		month;	/* 1..12 */
	uint8_t		mday;	/* 1.. 31 */
	uint8_t		wday;	/* 1..7 */
	uint8_t		hour;	/* 0..23 */
	uint8_t		min;	/* 0..59 */
	uint8_t		sec;	/* 0..59 */
} RTC;

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
