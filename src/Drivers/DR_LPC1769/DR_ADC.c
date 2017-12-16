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
volatile uint8_t 	HumedadSuelo;
volatile uint8_t	vPotenciometro;
volatile uint8_t	Temperatura;
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
	\fn  InitADC
	\brief Inicializa lo necesario para el funcionamiento del ADC
 	\author Tomás Bautista Ordóñez
 	\date 9 oct. 2017
*/

void InitADC ( void )
{
	/*Esta inicializacion esta hecha apuntando a que el ADC no interrumpa
	 *y que un timer cargue los buffers correspondiente cuando es debido*/

	//Power:
	PCONP->bits._PCADC = 1;					//Prendo el periferico ADC

	//Clock:
	PCLKSEL0 |= ~( 0x03 << PCLKADC );		//PCLK del ADC a 25MHz
	ADCCLKDIV = 1;							//Frecuencia muestreo adc en 200KHz

	//Canales:
	ADCSEL = 0;								//Limpio ADCSEL
	ADCSEL |= CH2SEL;						//Selecciono el canal 2 (    Sensor Humedad   ) como objeto de conversion
	ADCSEL |= CH5SEL;						//Selecciono el canal 5 (    Potenciometro    ) como objeto de conversion
	ADCSEL |= CH1SEL;						//Selecciono el canal 1 (Sensor de Temperatura) como objeto de conversion
	ADCBURST = 1;							//Activo el BURST

	//Pines:
	SetPINSEL( AD02 , PINSEL_FUNC1 );		//Configuro el pin con el que voy a convertir con funcionADC (Sensor de Humedad)
	SetMODE( AD02 , NONE );					//Configuro el pin con neither pull-up nor pull-down
	SetPINSEL( AD05 , PINSEL_FUNC3 );		//Configuro el pin con el que voy a convertir con funcionADC (Potenciometro)
	SetMODE( AD05 , NONE );					//Configuro el pin con neither pull-up nor pull-down
	SetPINSEL( AD01 , PINSEL_FUNC1 );		//Configuro el pin con el que voy a convertir con funcionADC (Sensor de Temperatura)
	SetMODE( AD01 , NONE );					//Configuro el pin con neither pull-up nor pull-down

	//Interrupciones:
	/*No hay interrupciones*/
	AD0INTEN = 0;							//No interrumpe ningun canal
	ISER0 &= ~(1 << NVIC_ADC);				//Deshabilito la interrupcion de ADC en el NVIC

	//Activacion y disparo del ADC:
	ADCPDN = 1;								//Prendido
	ADCSTART = 0;							//STAR=0 para que inicie las conversiones
}
/**
	\fn  CargarBuffersADC
	\brief Llena los buffers de las entradas analogicas
 	\author Tomás Bautista Ordóñez
 	\date 9 oct. 2017
*/
void CargarBuffersADC ( void )
{
	uint32_t lecturaHumedad = ADDR2;
	uint32_t lecturaPotenciometro = ADDR5;
	uint32_t lecturaTemperatura = ADDR1;

	HumedadSuelo = 100-	( ( lecturaHumedad 		 >> ADDR_RESULTBIT ) & ADDR_RESULTMASK ) / 41;
	vPotenciometro =	( ( lecturaPotenciometro >> ADDR_RESULTBIT ) & ADDR_RESULTMASK ) / 41;
	Temperatura = 	  ( ( ( lecturaTemperatura	 >> ADDR_RESULTBIT ) & ADDR_RESULTMASK ) * 330 ) / 4095;		/*4095_____3300mV    LM35=> 10mV____1ºC				Temperatura = (ADC*330)/4095
																									   	   	   	   ADC_____x=ADCmV		   ADCmV____x=Temperatura								*/
	SetTimer( E_ADC , T_ADC );
}
