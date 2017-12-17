/*******************************************************************************************************************************//**
 *
 * @file		DR_RTC.c
 * @brief		Descripcion del modulo
 * @date		8 dic. 2017
 * @author		Rafaele Manuel Adrian
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_RTC.h"
/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define E_RTC (uint8_t)3
#define T_RTC 1
#define B_RTC SEG
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
RTC_t currentTime;
RTC_t defaultTime = {0};
uint8_t AlarmBuffer;

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

void InitRTC (void)
{
	// Enable PCLK to the RTC
	//__set_PCONP(PCRTC, 1);
	PCONP->bits._PCRTC = 1;					//Prendo el periferico RTC

	// Start RTC with external XTAL
	RTC_CCR = 0x11;

	//Prendemos el registro de deteccion de falla de poder
	RTC_AUXEN = 0x8;

	//Habilitamos la interrupcion de alarmas en la hora y minuto
	RTC_AMR = 0x6;

	defaultTime.Seconds = 0; // se carga un tiempo default en 0

	TimerStart(E_RTC,T_RTC,TimeUpdate,B_RTC);
}

void TimeUpdate(void)
{
	currentTime.Seconds = RTC_CTIME->CTIME0.bits.Seconds;
	currentTime.Minutes = RTC_CTIME->CTIME0.bits.Minutes;
	currentTime.Hours = RTC_CTIME->CTIME0.bits.Hours;
	currentTime.DayOfWeek = RTC_CTIME->CTIME0.bits.DayOfWeek;
	currentTime.DayofMonth = RTC_CTIME->CTIME1.bits.DayOfMonth;
	currentTime.Month = RTC_CTIME->CTIME1.bits.Month;
	currentTime.Year = RTC_CTIME->CTIME1.bits.Year;
	SetTimer(E_RTC,T_RTC);
}

void RTC_IRQHandler(void)
{
	if (GetFailFlag) // Si se detecto una falla de poder se solicita la temperatura al software acompañante
	{
		CLRFailFlag;
		TransmitirString("#CF$"); //CF: comando de RTC Failure solicita por el puerto UART el envio de la hora actual para re-sincronizar
		SetRTCTime(&defaultTime); //En el mientras tanto se
		StandByTimer(E_RTC, PAUSE);
	}
	else if (RTC_ILR & 0x2)
	{
		RTC_ILR |= 0x2;
		AlarmBuffer = 1;
	}
}

void SetRTCTime (const RTC_t *rtc)
{
	RTC_CCR = 0x12;		//Apagamos el RTC

	//Actualizamos los valores del RTC
	RTC_SEC   = rtc->Seconds;
	RTC_MIN   = rtc->Minutes;
	RTC_HOUR  = rtc->Hours;
	RTC_DOW   = rtc->DayOfWeek;
	RTC_DOM   = rtc->DayofMonth;
	RTC_MONTH = rtc->Month;
	RTC_YEAR  = rtc->Year;

	StandByTimer(E_RTC, RUN);
	RTC_CCR = 0x11;		//Encendemos el RTC nuevamente
}


