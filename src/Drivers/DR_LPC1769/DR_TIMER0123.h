/*
 * DR_InicializaTIMER0_1.h
 *
 *  Created on: 30 de ago. de 2017
 *      Author: Poyo
 */

#ifndef DRIVERLPC_DR_INICIALIZATIMER0_1_H_
#define DRIVERLPC_DR_INICIALIZATIMER0_1_H_




/***********************************************************************************************************//**
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
    #define PORT0     0
    #define PORT1     1
    #define PORT2     2
    #define PORT3     3
    #define PORT4     4
/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include"DR_tipos.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
//Timers:
//0x40004000UL : Direccion de inicio de los registros del Timer0
#define		DIR_TIMER0		( ( timer_t  * ) 0x40004000UL )
//0x40008000UL : Direccion de inicio de los registros del Timer1
#define		DIR_TIMER1		( ( timer_t  * ) 0x40008000UL )
//0x40090000UL : Direccion de inicio de los registros del Timer2
#define		DIR_TIMER2		( ( timer_t  * ) 0x40090000UL )
//0x40094000UL : Direccion de inicio de los registros del Timer3
#define		DIR_TIMER3		( ( timer_t  * ) 0x40094000UL )

//Registros de timers:
#define		TIMER0		DIR_TIMER0[0]
#define		TIMER1		DIR_TIMER1[0]
#define		TIMER2		DIR_TIMER2[0]
#define		TIMER3		DIR_TIMER3[0]

 // Timer 0
#define		T0IR		TIMER0.IR
#define     T0_IR_MR0   T0IR & 0X01       // Bit 0 del registro IR corresponde al flag de la Int por Match 0
#define     T0_IR_MR1   (T0IR & (0X01 <<1))>>1 // Bit 1 del registro IR corresponde al flag de la Int por Match 1
#define     T0_IR_MR2   (T0IR & (0X01 <<2))>>2 // Bit 2 del registro IR corresponde al flag de la Int por Match 2
#define     T0_IR_MR3   (T0IR & (0X01 <<3))>>3 // Bit 3 del registro IR corresponde al flag de la Int por Match 3
#define     T0_IR_CR0   (T0IR & (0X01 <<1))>>4 // Bit 4 del registro IR corresponde al flag de la Int por CAPTURE 0
#define     T0_IR_CR1   (T0IR & (0X01 <<1))>>5 // Bit 5 del registro IR corresponde al flag de la Int por CAPTURE 1

//Timer 1
#define		T1IR		TIMER1.IR
#define     T1_IR_MR0   T1IR & 0X01       // Bit 0 del registro IR corresponde al flag de la Int por Match 0
#define     T1_IR_MR1   (T1IR & (0X01 <<1))>>1 // Bit 1 del registro IR corresponde al flag de la Int por Match 1
#define     T1_IR_MR2   (T1IR & (0X01 <<2))>>2 // Bit 2 del registro IR corresponde al flag de la Int por Match 2
#define     T1_IR_MR3   (T1IR & (0X01 <<3))>>3 // Bit 3 del registro IR corresponde al flag de la Int por Match 3
#define     T1_IR_CR0   (T1IR & (0X01 <<1))>>4 // Bit 4 del registro IR corresponde al flag de la Int por CAPTURE 0
#define     T1_IR_CR1   (T1IR & (0X01 <<1))>>5 // Bit 5 del registro IR corresponde al flag de la Int por CAPTURE 1

#define		T2IR		TIMER2.IR
#define		T3IR		TIMER3.IR

#define		T0TCR		TIMER0.TCR
#define		T1TCR		TIMER1.TCR
#define		T2TCR		TIMER2.TCR
#define		T3TCR		TIMER3.TCR

#define		T0PR		TIMER0.PR
#define		T1PR		TIMER1.PR
#define		T2PR		TIMER2.PR
#define		T3PR		TIMER3.PR

#define		T0PC		TIMER0.PC
#define		T1PC		TIMER1.PC
#define		T2PC		TIMER2.PC
#define		T3PC		TIMER3.PC

#define		T0MCR		TIMER0.MCR
#define		T1MCR		TIMER1.MCR
#define		T2MCR		TIMER2.MCR
#define		T3MCR		TIMER3.MCR

#define		T0MR0		TIMER0.MR0
#define		T1MR0		TIMER1.MR0
#define		T2MR0		TIMER2.MR0
#define		T3MR0		TIMER3.MR0

#define		T0MR1		TIMER0.MR1
#define		T1MR1		TIMER1.MR1
#define		T2MR1		TIMER2.MR1
#define		T3MR1		TIMER3.MR1

#define		T0MR2		TIMER0.MR2
#define		T1MR2		TIMER1.MR2
#define		T2MR2		TIMER2.MR2
#define		T3MR2		TIMER3.MR2

#define		T0MR3		TIMER0.MR3
#define		T1MR3		TIMER1.MR3
#define		T2MR3		TIMER2.MR3
#define		T3MR3		TIMER3.MR3

#define		T0CCR		TIMER0.CCR
#define		T1CCR		TIMER1.CCR
#define		T2CCR		TIMER2.CCR
#define		T3CCR		TIMER3.CCR

#define		T0CR0		TIMER0.CR0
#define		T1CR0		TIMER1.CR0
#define		T2CR0		TIMER2.CR0
#define		T3CR0		TIMER3.CR0

#define		T0CR1		TIMER0.CR1
#define		T1CR1		TIMER1.CR1
#define		T2CR1		TIMER2.CR1
#define		T3CR1		TIMER3.CR1

#define		T0EMR		TIMER0.EMR
#define		T1EMR		TIMER1.EMR
#define		T2EMR		TIMER2.EMR
#define		T3EMR		TIMER3.EMR


//Registros Globales
#define		T0CTCR		((__RW uint32_t *) 0x40004070UL)
#define		T1CTCR		((__RW uint32_t *) 0x40008070UL)
#define		T2CTCR		((__RW uint32_t *) 0x40090070UL)
#define		T3CTCR		((__RW uint32_t *) 0x40094070UL)


//!< ///////////////////   PCLKSEL   //////////////////////////
//!< Peripheral Clock Selection registers 0 and 1 (PCLKSEL0 -0x400F C1A8 and PCLKSEL1 - 0x400F C1AC) [pag. 56 user manual]
//!< 0x400FC1A8UL : Direccion de inicio de los registros de seleccion de los CLKs de los dispositivos:
#define		PCLKSEL		( ( __RW uint32_t  * ) 0x400FC1A8UL )
//!< Registros PCLKSEL
#define		PCLKSEL0	PCLKSEL[0]
#define		PCLKSEL1	PCLKSEL[1]
//0xE000E100UL : Direccion de inicio de los registros de habilitación (set) de interrupciones en el NVIC:
#define		ISER		( ( uint32_t * ) 0xE000E100UL )
#define		ISER0 ISER[0]
//0xE000E180UL : Direccion de inicio de los registros de deshabilitacion (clear) de interrupciones en el NVIC:
#define		ICER		( (  uint32_t * ) 0xE000E180UL )
#define 	ICER0 ICER[0]

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

#define     EMCO_TOGGLE  T1EMR|=(0x3 << 4) // Configurado como TOGGLE el pin MAT1.0
#define     EMC1_TOGGLE  T1EMR|=(0x3 << 6) //  Configurado como TOGGLE el pin MAT1.1
#define     ISE_TIMER0  ISER[0] |= (0x00000001)<<  1   //ISER0->bit1 pongo un 1 en el bit 1 para habilitar la INT TIMER0
#define     ISE_TIMER1  ISER[0] |= (0x00000001)<<  2   //ISER0->bit2 pongo un 1 en el bit 2 para habilitar la INT TIMER1
#define     ISE_TIMER2  ISER[0] |= (0x00000001)<<  3   //ISER0->bit2 pongo un 1 en el bit 2 para habilitar la INT TIMER1
#define     ISE_TIMER3  ISER[0] |= (0x00000001)<<  4   //ISER0->bit2 pongo un 1 en el bit 2 para habilitar la INT TIMER1

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
//Estructura para manejar los Timers:
typedef struct
{
	uint32_t	IR;
	uint32_t	TCR;
	uint32_t 	TC;
	uint32_t 	PR;
	uint32_t 	PC;
	uint32_t 	MCR;
	uint32_t	MR0;
	uint32_t	MR1;
	uint32_t	MR2;
	uint32_t	MR3;
	uint32_t	CCR;
	uint32_t	CR0;
	uint32_t	CR1;       // 0x40004030 para el T0
	uint32_t    RESERVE[2];// 0x40004034 para el T0
						   // 0x40004038 para el T0
	uint32_t    EMR;       // 0x4000403C para el T0

} timer_t;
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/


/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

#endif /* DRIVERLPC_DR_INICIALIZATIMER0_1_H_ */
