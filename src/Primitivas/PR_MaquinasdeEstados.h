/*******************************************************************************************************************************//**
 *
 * @file		PR_MaquinasdeEstados.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Dec 8, 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PRIMITIVAS_PR_MAQUINASDEESTADOS_H_
#define PRIMITIVAS_PR_MAQUINASDEESTADOS_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "PR_ADC.h"
#include "PR_Leds.h"
#include "PR_RTC.h"
#include "DR_Boton.h"
#include "MaquinaGeneral.h"
#include "MaquinasdeEstados.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define HrD	 0
#define HrU	 1
#define MinD 2
#define MinU 3
#define SegU 4
#define SegD 5

#define Decena 0
#define Unidad 1
/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/
#define EV_RIEGO_ON SetPIN(VALVULA, ALTO);
#define EV_RIEGO_OFF SetPIN(VALVULA, BAJO);
/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/

extern uint8_t vUnidad; //variable utilizada para definir la unidad a la hora de editar
extern int32_t vConfig; //variable utilizada para almacenar los valores que se cargan en la estructura de configuracion

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void InitManual(void);
void InitAutomatico(void);
void InitTemporizado(void);
void InitConfiguracion(void);
void CloseConfiguracion(void);
void ParsearConfiguracion(const char*);
void CloseEstados(void);
void SwitchEstados(uint8_t);

//void ComponerHumedad(uint8_t,char *);
//void ComponerTemporizador(const RTC_t *,char*);
//void ComponerTemporizadorCorto(const RTC_t *,char*);

void PrintHumMin(void);
void PrintHumMax(void);
void PrintEstadoAutomatico(void);

void PrintHumedad(void);
void PrintHour(void);
void PrintAlarmaOnOff(void);

void PrintConfigHumMax(void);
void PrintConfigHumMin(void);
void PrintCerrarConfig(void);
void PrintTempDeRiego(void);
void PrintPrenderAlarma(void);
void PrintConfigAlarma(void);

void PrintRemainingTime(void);
void PrintCurrentTime(void);
void PrintAlarm(void);
void PrintStatus(void);
void PrintTimer(void);

#endif /* PRIMITIVAS_PR_MAQUINASDEESTADOS_H_ */
