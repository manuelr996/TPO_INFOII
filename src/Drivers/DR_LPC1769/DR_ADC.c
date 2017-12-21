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
volatile uint32_t	vPotenciometro;
volatile uint16_t	bufferTemp[ TAMBUFFERTEMP ]={250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250};
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
	/*Esta inicializacion esta hecha apuntando a que el ADC se maneje sin BURST y con un timer activamos canal a canal*/

	//Power:
	PCONP->bits._PCADC = 1;					//Prendo el periferico ADC

	//Clock:
	PCLKSEL0 |= ~( 0x03 << PCLKADC );		//PCLK del ADC a 25MHz
	ADCCLKDIV = 1;							//Frecuencia muestreo adc en 200KHz

	//Canales:
	ADCSEL = 0;								//Limpio ADCSEL
	ADCBURST = 0;							//Sin BURST

	//Pines:
	SetPINSEL( AD02 , PINSEL_FUNC1 );		//Configuro el pin con el que voy a convertir con funcionADC (Sensor de Humedad)
	SetMODE( AD02 , NONE );					//Configuro el pin con neither pull-up nor pull-down
	SetPINSEL( AD05 , PINSEL_FUNC3 );		//Configuro el pin con el que voy a convertir con funcionADC (Sensor de Temperatura) ex potenciometro
	SetMODE( AD05 , NONE );					//Configuro el pin con neither pull-up nor pull-down
	//SetPINSEL( AD01 , PINSEL_FUNC1 );		//Configuro el pin con el que voy a convertir con funcionADC (Sensor de Temperatura)
	//SetMODE( AD01 , NONE );					//Configuro el pin con neither pull-up nor pull-down

	//Interrupciones:
	AD0INTEN = 1 << ADINTEN_GLOB;			//Interrumpe el flag DONE del registro global
	ISER0 |= 1 << NVIC_ADC;					//Deshabilito la interrupcion de ADC en el NVIC

	//Activacion y disparo del ADC:
	ADCPDN = OFF;							//Prendo el ADC
	ADCSTART = STOP;						//Lo dejo pausado para despues activar las conversiones "manualmente"
}

void ADC_IRQHandler( void )
{
	uint32_t lectura = AD0GDR;
	uint32_t conversion;
	static uint8_t indTemp = 0;

	conversion = ( lectura >> ADDR_RESULTBIT ) & ADDR_RESULTMASK;	//Dejo el dato de la conversion bien expresado

	switch ( (lectura >> CHINBIT) & CHINMASK  )						//Hago un switch con el CHIN (ultimo canal convertido)
		{
			case S_HUMEDAD:
				HumedadSuelo = 	 ConvHum(conversion);			//Cargo la conversion al buffer
				break;
			/*
			 * case POTE:
				vPotenciometro = conversion;						//Cargo la conversion al buffer
				break;
			*/
			case S_TEMPERATURA:
				bufferTemp[ indTemp ] = ConvTemp(conversion);		//Cargo la conversion al array buffer				4095_____3300mV    LM35=>   10mV____1ºC				Temperatura = (ADC*330)/4095																					//	 ADC_____x=ADCmV		   ADCmV____x=Temperatura
				indTemp++;
				indTemp %= TAMBUFFERTEMP;
				break;
			default:
				break;
		}
}
/**
	\fn  CambiarCanal
	\brief Cambia el canal objto de conversion
 	\author Tomás Bautista Ordóñez
 	\param uint8_t ch: canal a convertir
 	\date 9 de dic. de 2017
*/
void CambiarCanal ( uint8_t ch )
{
	ADCSTART = STOP;						//Detendo el ADC

	ADCPDN = OFF;							//Apago el ADC

	ADCSEL = 1 << ch;						//Selecciono el canal ch como objeto de conversion
}

/**
	\fn  DispararConversion
	\brief Dispara una conversion y se resetea
 	\author Tomás Bautista Ordóñez
 	\date 9 de dic. de 2017
*/
void DispararConversion ( void )
{
	ADCPDN = ON;							//Prendido
	ADCSTART = STOP;						//Disparo
	ADCSTART = START;
}
