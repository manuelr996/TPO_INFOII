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
#include "Configuracion.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define B_Riego SEG
#define E_Riego (uint8_t)1

#define E_Print (uint8_t)2
#define B_Print SEG
#define T_Print 1
#define T_Print_largo 	3
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
	TEMPORIZADOR_HHD,	//Tiempo Riego - Horas (Decenas)
	TEMPORIZADOR_HHU,	//Tiempo Riego - Horas (Unidades)
	TEMPORIZADOR_MMD,	//Tiempo Riego - Minutos (Decenas)
	TEMPORIZADOR_MMU,	//Tiempo Riego - Minutos (Unidades)
	ALARMA_ON_OFF,		//Prender o Apagar la Alarma
	HORA_RIEGO_HHD,		//Hora Riego - Horas (Decenas)
	HORA_RIEGO_HHU,		//Hora Riego - Horas (Unidades)
	HORA_RIEGO_MMD,		//Hora Riego - Minutos (Decenas)
	HORA_RIEGO_MMU,		//Hora Riego - Minutos (Unidades)
	HORA_HHD,
	HORA_HHU,
	HORA_MMD,
	HORA_MMU,
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

extern CONFIG_t config;
/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

void RiegoOn(void);
void RiegoOff(void);


void RiegoAutomaticoOn(void);
void RiegoAutomaticoOff(void);

void ConfiguracionInicializada(void);
void SetHumedadMinimaDecenas(void);
void SetHumedadMinimaUnidades(void);
void SetHumedadMaximaDecenas(void);
void SetHumedadMaximaUnidades(void);
void SetTemporizadorHHD(void);
void SetTemporizadorHHU(void);
void SetTemporizadorMMD(void);
void SetTemporizadorMMU(void);
void SetAlarmaOn(void);
void SetHoraRiegoHHD(void);
void SetHoraRiegoHHU(void);
void SetHoraRiegoMMD(void);
void SetHoraRiegoMMU(void);
void SetHoraHHD(void);
void SetHoraHHU(void);
void SetHoraMMD(void);
void SetHoraMMU(void);
void ConfiguracionFinalizada(void);

void AguardandoOk(void);
void VolverAguardando(void);
void RiegoTemporizado(void);
#endif /* APLICACION_MAQUINASDEESTADOS_H_ */
