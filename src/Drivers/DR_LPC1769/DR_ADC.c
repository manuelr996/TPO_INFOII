/*******************************************************************************************************************************//**
 *
 * @file		DR_ADC.c
 * @brief		Descripcion del modulo
 * @date		9 oct. 2017
 * @author		Tomás Bautista Ordóñez
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_ADC.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define 	FUNCION1 	1
/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/
#define		AD02		0,25
/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
volatile uint32_t 	HumedadSuelo;
volatile uint32_t	HumedadMaxima;
volatile uint32_t	HumedadMinima;

volatile uint32_t	vPotenciometro;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/
/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Tomás Bautista Ordóñez
 	\date 9 oct. 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

void InitADC ( void )
{
	//Power:
	PCONP->bits._PCADC = 1;		//Prendo el periferico ADC

	//Clock:
	PCLKSEL0 |= ~( 0x03 << PCLKADC );		//PCLK del ADC a 25MHz
	ADCCLKDIV = 1;						//Frecuencia muestreo adc en 200KHz

	//Canales:
	ADCSEL = 0x04;					//Selecciono el canal 2 como objeto de conversion
	ADCBURST = 0;					//Conversion controlada por software al canal seleccionado en ADCSEL

	//Pines:
	SetPINSEL( AD02 , FUNCION1 );		//Configuro el pin con el que voy a convertir con funcionADC
	SetMODE( AD02 , NONE );				//Configuro el pin con neither pull-up nor pull-down

	//Interrupciones:
	AD0INTEN &= ~( 1 << ADINTEN_GLOB );		//Interrumpen los canales seleccionados luego
	AD0INTEN |=  ( 1 << ADINTEN_CH2  );		//Interrumpe el canal 2
	AD0INTEN |=  ( 1 << ADINTEN_CH0  );
	ISER0 |= (1 << NVIC_ADC);				//Habilito la interrupcion de ADC en el NVIC

	//Activacion y disparo del ADC:
	ADCPDN = 1;						//Prendido
	ADCSTART = 0;					//Dejo el adc en stop, para que sea disparado con maquinaria de timers
									//para manejar mejor la frecuencia de muestreo
}

void ADC_IRQHandler( void )
{
	uint32_t conversion = 0;

	if( AD0STAT & 0x04 )			//Si interrumpio el canal 2
	{
		conversion = ( (ADDR2) >> ADDR_RESULT ) & 0xFFF;	//Dejo el dato de la conversion bien expresado
		HumedadSuelo = 100 - conversion/41;		//Cargo la conversion al buffer
	}
	if( AD0STAT &0x01 )				//Si interrumpio el canal 0
	{
		conversion = ( (ADDR2) >> ADDR_RESULT ) & 0xFFF;	//Dejo el dato de la conversion bien expresado
		vPotenciometro = conversion;
	}

}

void CambiarCanal (uint8_t ch)
{
	ADCSEL &= ~(0x0);		//Apago todos los caneles
	ADCSEL |= (0x01 << ch);  		//Prendo el Canal ch
}

