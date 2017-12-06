/*******************************************************************************************************************************//**
 *
 * @file		DR_EINT.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		24 de oct. de 2017
 * @author		Ing. Marcelo Trujillo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef LEDCOLORES_DRIVERS_DR_EINT_H_
#define LEDCOLORES_DRIVERS_DR_EINT_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_tipos.h"
#include "DR_TIMER0123.h"
#include "DR_gpio.h"
#include "DR_pinsel.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

#define     DIREXTINT       ( (uint32_t *) 0x400FC140UL )

#define     EXTINT          DIREXTINT[0]
#define     EXTMODE         DIREXTINT[2]
#define     EXTPOLAR        DIREXTINT[3]

#define     DIRIOIntStatus  ( (uint32_t *) 0x40028080UL )

#define     DIRIO0IntEnR    ( (uint32_t *) 0x40028090UL )
#define     DIRIO0IntEnF    ( (uint32_t *) 0x40028094UL )
#define 	DIRIO2IntEnR	( (uint32_t *) 0x400280B0UL )
#define 	DIRIO2IntEnF	( (uint32_t *) 0x400280B4UL )

#define     DIRIO0IntStatR  ( (uint32_t *) 0x40028084UL )
#define     DIRIO0IntStatF  ( (uint32_t *) 0x40028088UL )
#define		DIRIO2IntStatR	( (uint32_t *) 0x400280A4UL )
#define		DIRIO2IntStatF	( (uint32_t *) 0x400280A8UL )

#define     DIRIO0IntClr    ( (uint32_t *) 0x4002808CUL )
#define		DIRIO2IntClr	( (uint32_t *) 0x400280ACUL )

#define     DIRIOIntStatus  ( (uint32_t *) 0x40028080UL )

#define     ENTDIG2         2,11
#define 	EXPANSION16		2,8
#define 	SENSOR_LLUVIA	EXPANSION16

#define 	FUNC0			0

#define		ACTIVADO		1
#define		DESACTIVADO		0

#define 	RISING 			1
#define		FALLING			2

#define 	ISE_EINT3		21

#define 	Offset_GPIOint 	0x10
#define 	OffsetR_F		1
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
extern volatile uint8_t Lluvia;
/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void InitExtIntGPIO ( void );
void ClearGPIOInt( uint8_t puerto, uint8_t bit );
uint8_t GetRFStatus( uint8_t puerto, uint8_t bit );
void SetFalling( uint8_t puerto, uint8_t bit, uint8_t estado);
void SetRising( uint8_t puerto, uint8_t bit, uint8_t estado);
#endif /* LEDCOLORES_DRIVERS_DR_EINT_H_ */
