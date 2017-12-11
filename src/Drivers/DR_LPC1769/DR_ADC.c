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
volatile uint32_t	vPotenciometro;

volatile uint8_t 	CanalAConvertir;
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
	PCONP->bits._PCADC = 1;					//Prendo el periferico ADC

	//Clock:
	PCLKSEL0 |= ~( 0x03 << PCLKADC );		//PCLK del ADC a 25MHz
	ADCCLKDIV = 1;							//Frecuencia muestreo adc en 200KHz

	//Canales:
	ADCSEL = 0;								//Limpio ADCSEL
	ADCSEL |= CH2SEL;						//Selecciono el canal 2 (Sensor Humedad) como objeto de conversion
	ADCBURST = 0;							//Conversion controlada por software al canal seleccionado en ADCSEL

	//Pines:
	SetPINSEL( AD02 , PINSEL_FUNC1 );		//Configuro el pin con el que voy a convertir con funcionADC (Sensor de Humedad)
	SetMODE( AD02 , NONE );					//Configuro el pin con neither pull-up nor pull-down
	SetPINSEL( AD05 , PINSEL_FUNC3 );		//Configuro el pin con el que voy a convertir con funcionADC (Potenciometro)
	SetMODE( AD05 , NONE );					//Configuro el pin con neither pull-up nor pull-down

	//Interrupciones:
	AD0INTEN = ( 1 << ADINTEN_GLOB );		//Interrumpen el bit DONE del registro general
	//AD0INTEN |=  ( 1 << ADINTEN_CH2  );		//Interrumpe el canal 2	(Sensor de Humedad)
	//AD0INTEN |=  ( 1 << ADINTEN_CH5  );		//Interrumpe el canal 5 (Potenciometro)
	ISER0 |= (1 << NVIC_ADC);				//Habilito la interrupcion de ADC en el NVIC

	//Activacion y disparo del ADC:
	ADCPDN = 1;								//Prendido
	ADCSTART = 0;							/*Dejo el adc en stop, para que sea disparado con maquinaria de timers
											//para manejar mejor la frecuencia de muestreo*/
}

void ADC_IRQHandler( void )
{
	uint32_t lectura = AD0GDR;
	uint32_t conversion = 0;

	if( ( (lectura >> CHIN) & 0x00000007 ) == S_HUMEDAD )	//Si interrumpio el canal 2
	{
		conversion = ( lectura >> ADDR_RESULT ) & 0xFFF;	//Dejo el dato de la conversion bien expresado
		HumedadSuelo = 100 - (conversion / 409);					//Cargo la conversion al buffer
	}
	if( ( (lectura >> CHIN) & 0x00000007 ) == POTE )		//Si interrumpio el canal 5
	{
		conversion = ( lectura >> ADDR_RESULT ) & 0xFFF;	//Dejo el dato de la conversion bien expresado
		vPotenciometro = conversion / 409;					//Cargo la conversion al buffer
	}

}

void CambiarCanal (uint8_t ch)
{
	ADCSTART = STOP;						//Pausado
	ADCPDN = 0;								//Apagado

	ADCSEL = 0;								//Ningun canal es objeto de conversion
	ADCSEL |= (0x01 << ch);  				//El canal ch es objeto de conversion
}

/**
	\fn  DispararConversion
	\brief Habilita el canal a convertir segun el flag CanalAConvertir, dispara una conversion y se resetea
 	\author Tomás Bautista Ordóñez
 	\date 9 de dic. de 2017
*/
void DispararConversion ( void )
{
	ADCPDN = 1;								//Prendido
	ADCSTART = STOP;						//Disparo
	ADCSTART = TRIGGER;

	SetTimer(ADCevent, ADCtime);			//Reseteo
}
