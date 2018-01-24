/*******************************************************************************************************************************//**
 *
 * @file		Funciones.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		26 oct. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef FUNCIONES_H_
#define FUNCIONES_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "PR_ADC.h"
#include "PR_7seg.h"
#include "PR_EINT.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define E_Display 6
#define T_Display 5
#define B_Display DEC

#define MENSAJE_LLUVIA_ON	"#L$\0"
#define	MENSAJE_LLUVIA_OFF	"#l$\0"
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
void FuncLluvia( void );
void MostrarSensores( void );
void ComponerMedicion(char*,uint8_t,uint8_t);
#endif /* FUNCIONES_H_ */
