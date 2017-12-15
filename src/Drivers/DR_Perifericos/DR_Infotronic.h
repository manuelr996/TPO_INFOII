/*******************************************************************************************************************************//**
 *
 * @file		DR_Infotronic.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		26 nov. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_INFOTRONIC_H_
#define DR_INFOTRONIC_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_gpio.h"
#include "DR_pinsel.h"
#include "PR_SoftTimers.h"
#include "DR_ADC.h"
#include "PR_ADC.h"
#include "DR_7seg.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define		SD0		PORT2,0
#define 	SD4		PORT0,28
/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void InitInfotronic ( void );
void InitTeclado ( void );
void InitRGB ( void );
#endif /* DR_INFOTRONIC_H_ */
