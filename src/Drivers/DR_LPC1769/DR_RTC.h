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
#include "PR_SoftTimers.h"
#include "PR_UART.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
/* RTC */
#define	RTC_ILR			(*(__RW uint32_t*)0x40024000UL)
#define	RTC_CCR			(*(__RW uint32_t*)0x40024008UL)
#define	RTC_CIIR		(*(__RW uint32_t*)0x4002400CUL)
#define	RTC_AMR			(*(__RW uint32_t*)0x40024010UL)
#define RTC_CTIME		( (__RW CTIME   *)0x40024014UL)
#define	RTC_AUX			(*(__RW uint32_t*)0x4002405CUL)
#define	RTC_AUXEN		(*(__RW uint32_t*)0x40024058UL)
#define	RTC_CALIBRATION	(*(__RW uint32_t*)0x40024040UL)
#define	RTC_GPREG		( (__RW uint32_t*)0x40024044UL)
#define	RTC_GPREG0		RTC_GPREG[0]
#define	RTC_GPREG1		RTC_GPREG[1]
#define	RTC_GPREG2		RTC_GPREG[2]
#define	RTC_GPREG3		RTC_GPREG[3]
#define	RTC_GPREG4		RTC_GPREG[4]
#define RTC_TIMER		( (__RW uint32_t *)0x40024020UL)
#define RTC_SEC			RTC_TIMER[0]
#define RTC_MIN			RTC_TIMER[1]
#define RTC_HOUR		RTC_TIMER[2]
#define RTC_DOM			RTC_TIMER[3]
#define RTC_DOW			RTC_TIMER[4]
#define RTC_DOY			RTC_TIMER[5]
#define RTC_MONTH		RTC_TIMER[6]
#define RTC_YEAR		RTC_TIMER[7]
#define RTC_ALREG		( (__RW uint32_t *)0x40024060UL)
#define	RTC_ALSEC		RTC_ALREG[0]
#define	RTC_ALMIN		RTC_ALREG[1]
#define	RTC_ALHOUR		RTC_ALREG[2]
#define	RTC_ALDOM		RTC_ALREG[3]
#define	RTC_ALDOW		RTC_ALREG[4]
#define	RTC_ALDOY		RTC_ALREG[5]
#define	RTC_ALMON		RTC_ALREG[6]
#define	RTC_ALYEAR		RTC_ALREG[7]


#define RTC_CONFIG		((__RW CONFIG_t *) 0x40024044UL)

#define IMSEC			0x00000001
/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/
#define	_BV(bit) (1<<(bit))
#define GetFailFlag (RTC_AUX &  0x10)
#define CLRFailFlag (RTC_AUX |= 0x10)
/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
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
extern RTC_t currentTime;
extern RTC_t Alarm;
extern uint8_t AlarmBuffer;
/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void InitRTC ( void );					/* Inicializa al RTC */
void TimeUpdate(void);			/* 		Get time	 */
void SetRTCTime (const RTC_t *);		/*		Set time	 */

#endif /* DR_RTC_H_ */
