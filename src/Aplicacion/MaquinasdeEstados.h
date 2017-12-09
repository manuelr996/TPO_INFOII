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
#include "PR_SoftTimers.h"
#include "DR_tipos.h"
#include "PR_Boton.h"
#include "PR_Leds.h"
#include "DR_ElectroValvula.h"
#include "DR_ADC.h"
#include "DR_gpio.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

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
	HUMEDADMINIMA,
	HUMEDADMAXIMA,
	TEMPORIZADOR,
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

extern uint8_t btn;
/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

void RiegoOn(void);
void RiegoOff(void);

void ConfiguracionInicializada(void);
void SetHumedadMinima(void);
void SetHumedadMaxima(void);
void SetTemporizador(void);
void ConfiguracionFinalizada(void);

#endif /* APLICACION_MAQUINASDEESTADOS_H_ */
