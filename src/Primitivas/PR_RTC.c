/*******************************************************************************************************************************//**
 *
 * @file		PR_RTC.c
 * @brief		Descripcion del modulo
 * @date		16 dic. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_RTC.h"

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
 	\date 16 dic. 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

void SetAlarm(struct RTC_t *rtc)
{
	RTC_ALMIN = rtc->Minutes;
	RTC_ALHOUR = rtc->Hours;
}

void DecrementoTiempo(RTC_t *rtc)
{
	if(rtc->Seconds == 0)
	{
		if(rtc->Minutes == 0)
		{
			rtc->Hours--;
			rtc->Minutes = 59;
		}
		else
		{
			rtc->Minutes--;
		}
		rtc->Seconds = 59;
	}
	else
		rtc->Seconds--;
}

void IncrementoTiempo(RTC_t *rtc)
{
	if(rtc->Seconds == 59)
	{
		if(rtc->Minutes == 59)
		{
			rtc->Hours++;
			rtc->Minutes = 00;
		}
		else
		{
			rtc->Minutes++;
		}
		rtc->Seconds = 00;
	}
	else
		rtc->Seconds++;
}

RTC_t GetTime(void)
{
	return currentTime;
}

RTC_t DiferenciaTiempos(RTC_t *rtc)
{
	RTC_t aux = {0};

	aux.Hours = currentTime.Hours - rtc->Hours;
	aux.Minutes = currentTime.Minutes - rtc->Minutes;
	return aux;
}

uint8_t Alarma(void)
{
	uint8_t aux = AlarmBuffer;
	AlarmBuffer = 0;
	return aux;
}

RTC_t FromGetTimer(uint32_t tiempo, uint8_t base)
{
	RTC_t aux = {0};
	switch(base)
	{
	case MIN:
		aux.DayOfWeek = tiempo/1440;
		if(aux.DayOfWeek > 0)
		{
			tiempo -= 1440;
		}
		aux.Hours 	  = ((float)tiempo)/60;
		aux.Minutes   = tiempo%60;
		break;
	case SEG:
		aux.Hours 	= tiempo/3600;
		aux.Minutes = tiempo/60;
		aux.Seconds = tiempo%60;
		break;
	case DEC:
		aux.Hours	= tiempo/36000;
		aux.Minutes = tiempo%36000;
		aux.Seconds = tiempo-((aux.Hours*3600)+(aux.Minutes*60));
		break;
	}
	return aux;
}

uint32_t ToTimer(RTC_t *src, uint8_t base)
{
	uint32_t aux = 0;

	switch(base)
	{
	case MIN:
		aux =  src->Hours*60;
		aux += src->Minutes;
		aux += src->Seconds/60;
	case SEG:
		aux =  src->Hours*3600;
		aux += src->Minutes*60;
		aux += src->Seconds;
	case DEC:
		aux = src->Hours*36000;
		aux = src->Minutes*600;
		aux = src->Seconds*10;
	}

	return aux;
}

void ActualizarRTC(const char *src)
{
	RTC_t auxrtc;

	auxrtc.Hours 	= (src[0]-'0')*10;
	auxrtc.Hours 	+= (src[1]-'0');
	auxrtc.Minutes 	= (src[2]-'0')*10;
	auxrtc.Minutes 	+= (src[3]-'0');
	auxrtc.Seconds 	= (src[4]-'0')*10;
	auxrtc.Seconds 	+= (src[5]-'0');

	SetRTCTime(&auxrtc);

}
