/*******************************************************************************************************************************//**
 *
 * @file		MaquinasdeEstados2.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Dec 8, 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef APLICACION_MAQUINASDEESTADOS_H_
#define APLICACION_MAQUINASDEESTADOS_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include <stdlib.h>
#include "PR_SoftTimers.h"
#include "PR_Boton.h"
#include "PR_Leds.h"
#include "PR_Sensores.h"
#include "DR_ElectroValvula.h"
#include "DR_ADC.h"
#include "DR_gpio.h"
#include "PR_RTC.h"
#include "PR_UART.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define B_Riego SEG
#define E_Riego (uint8_t)1

#define E_Potenciometro (uint8_t)2
#define B_Potenciometro SEG
#define T_Potenciometro 1
/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/
#define EV_RIEGO_ON SetPIN(VALVULA, ALTO);
#define EV_RIEGO_OFF SetPIN(VALVULA, BAJO);
/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

typedef enum
{
	AGUARDANDO_OK = 0,
	RIEGO_TEMPORIZADO
}EstadosTemporizado;

typedef enum
{
	RIEGO_OFF = 0,
	RIEGO_ON
}EstadosManual;

typedef enum
{
	REGANDO = 0,
	NO_REGANDO
}EstadosAutomatico;

typedef enum
{
	INIT_CONFIGURACION = 0,
	HUMEDADMINIMA_D,	//Humedad Minima - Decenas
	HUMEDADMINIMA_U,	//Humedad Minima - Unidad
	HUMEDADMAXIMA_D,	//Humedad Maxima - Decenas
	HUMEDADMAXIMA_U,	//Humedad Maxima - Unidad
	TEMPORIZADOR_HH,	//Tiempo Riego - Horas
	TEMPORIZADOR_MMD,	//Tiempo Riego - Minutos(Decenas)
	TEMPORIZADOR_MMU,	//Tiempo Riego - Minutos(Unidades)
	TEMPORIZADOR_SSD,	//Tiempo Riego - Segundos(Decenas)
	TEMPORIZADOR_SSU,	//Tiempo Riego - Segundos(Unidades)
	HORA_RIEGO_HH,		//Hora Riego - Horas
	HORA_RIEGO_MMD,		//Hora Riego - Minutos(Decenas)
	HORA_RIEGO_MMU,		//Hora Riego - Minutos(Unidades)
	HORA_RIEGO_SSD,		//Hora RIego - Segundos(Decenas)
	HORA_RIEGO_SSU,		//Hora Riego - Segundos(Unidades)
	CERRAR_CONFIGURACION
}EstadosConfiguracion;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern EstadosConfiguracion EstadoConfiguracion;
extern EstadosManual EstadoManual;
extern EstadosTemporizado EstadoTemporizado;
extern EstadosAutomatico EstadoAutomatico;

extern uint32_t T_Riego;
extern struct RTC_t AlarmTime;
/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

void RiegoOn(void);
void RiegoOff(void);

void RiegoAutomaticoOn(void);
void RiegoAutomaticoOff(void);

void PrintHumedad(void);
void PrintHour(void);
void ConfiguracionInicializada(void);
void SetHumedadMinimaDecenas(void);
void SetHumedadMinimaUnidades(void);
void SetHumedadMaximaDecenas(void);
void SetHumedadMaximaUnidades(void);
void SetTemporizadorHH(void);
void SetTemporizadorMMD(void);
void SetTemporizadorMMU(void);
void SetTemporizadorSSD(void);
void SetTemporizadorSSU(void);
void SetHoraRiegoHH(void);
void SetHoraRiegoMMD(void);
void SetHoraRiegoMMU(void);
void SetHoraRiegoSSD(void);
void SetHoraRiegoSSU(void);
void ConfiguracionFinalizada(void);

void AguardandoOk(void);
void PrintTimer(void);
void PrintAlarm(void);
void PrintStatus(void);
void PrintCurrentTime(void);
void VolverAguardando(void);
void RiegoTemporizado(void);
#endif /* APLICACION_MAQUINASDEESTADOS_H_ */
