/*******************************************************************************************************************************//**
 *
 * @file		DR_ADC.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		9 oct. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_ADC_H_
#define DR_ADC_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_tipos.h"
#include "DR_pinsel.h"
#include "DR_TIMER0123.h"
#include "DR_gpio.h"
#include "DR_PCONP.h"
#include "DR_EINT.h"
#include "PR_SoftTimers.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define 	ADC		( (adc_t *) 0x40034000UL )
	#define		ADCR			ADC->_ADCR
	#define		ADCSEL			ADC->bitsADCR._SEL
	#define 	ADCCLKDIV		ADC->bitsADCR._CLKDIV
	#define 	ADCBURST		ADC->bitsADCR._BURST
	#define 	ADCPDN			ADC->bitsADCR._PDN
	#define 	ADCSTART		ADC->bitsADCR._START
	#define 	ADCEDGE			ADC->bitsADCR._EDGE

#define		ADC0	( (uint32_t *) 0x40034004UL )

#define 	AD0GDR 		ADC0[0]
#define		AD0INTEN	ADC0[2]

#define		ADDR0		ADC0[3]
#define		ADDR1		ADC0[4]
#define		ADDR2		ADC0[5]
#define		ADDR3		ADC0[6]
#define		ADDR4		ADC0[7]
#define		ADDR5		ADC0[8]
#define		ADDR8		ADC0[9]
#define		ADDR7		ADC0[10]

#define		AD0STAT		ADC0[11]

#define		ADTRIM		ADC0[12]

#define		CH2SEL			0x04
#define		CH5SEL			0x20
#define 	CH1SEL 			0x02

#define 	PCONPADC		12
#define 	PCLKADC			24
#define		ADINTEN_GLOB	8
#define		ADINTEN_CH0		0
#define		ADINTEN_CH2		2
#define		NVIC_ADC		22
#define		ADDR_RESULT		4

#define 	E_ADC 			0
#define 	T_ADC			2
#define		B_ADC			CEN

#define		STOP			0
#define		TRIGGER			1

#define 	AD01				PORT0,24
#define		AD02				PORT0,25
#define		AD05				PORT1,31

#define 	CHINBIT 			24
#define 	CHINMASK			0x00000007

#define		ADDR_RESULTBIT		4
#define		ADDR_RESULTMASK		0xFFF

#define 	POTE 				5
#define 	S_HUMEDAD 			2
/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
//!< Tipo de dato específico para manejar el ADC
typedef struct
{
	union{
		__RW uint32_t _ADCR;
		struct{
			__RW uint32_t _SEL:			8;
			__RW uint32_t _CLKDIV:		8;
			__RW uint32_t _BURST:		1;
			__RW uint32_t _RESERVED0:	4;
			__RW uint32_t _PDN:			1;
			__RW uint32_t _RESERVED1:	2;
			__RW uint32_t _START:		3;
			__RW uint32_t _EDGE:		1;
			__RW uint32_t _RESERVED2:	4;
		}bitsADCR;
	};
}adc_t;
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern volatile uint8_t HumedadSuelo;
extern volatile uint8_t	vPotenciometro;
extern volatile uint8_t	Temperatura;
/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void InitADC ( void );
void CargarBuffersADC ( void );
#endif /* DR_ADC_H_ */
