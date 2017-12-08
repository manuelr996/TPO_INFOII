/*******************************************************************************************************************************//**
 *
 * @file		Template.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Fecha de creacion del archivo XX/XX/XXXX
 * @author		Nombre, Apellido
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_7SEG_H_
#define DR_7SEG_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_tipos.h"
#include "DR_SysTick.h"
#include "DR_Inicializacion.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define		EXPANSION0		PORT2,7
#define		EXPANSION1		PORT1,29
#define		EXPANSION2		PORT4,28
#define		EXPANSION3		PORT1,23
#define		EXPANSION4		PORT1,20
#define		EXPANSION5		PORT0,19
#define		EXPANSION6		PORT3,26
#define		EXPANSION7		PORT1,19
#define		EXPANSION8		PORT0,20
#define		EXPANSION9		PORT3,25
#define		EXPANSION10		PORT1,27
#define		EXPANSION11		PORT1,24
#define		EXPANSION12		PORT1,21
#define		EXPANSION13		PORT1,18

#define		digito0			EXPANSION0
#define		digito1			EXPANSION1
#define		digito2			EXPANSION2
#define		digito3			EXPANSION3
#define		digito4			EXPANSION4
#define		digito5			EXPANSION5
#define		segmento_a		EXPANSION6
#define		segmento_dp		EXPANSION7
#define		segmento_g		EXPANSION8
#define		segmento_f		EXPANSION9
#define		segmento_e		EXPANSION10
#define		segmento_d		EXPANSION11
#define		segmento_c		EXPANSION12
#define		segmento_b		EXPANSION13

#define DIGITOS		6

#define DIGITO_0	5
#define DIGITO_1	4
#define DIGITO_2	3
#define DIGITO_3	2
#define DIGITO_4	1
#define DIGITO_5	0

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// 	Digitos del display
extern uint8_t Digito_Display[  ];	//!< 	Digitos del display

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void BarridoDisplay( void );
void InitExp2 ( void );

#endif /* DR_7SEG_H_ */
