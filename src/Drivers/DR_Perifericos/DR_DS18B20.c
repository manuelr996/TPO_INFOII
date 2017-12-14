/*******************************************************************************************************************************//**
 *
 * @file		DR_DS18B20.c
 * @brief		Descripcion del modulo
 * @date		10 de oct. de 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_DS18B20.h"
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
volatile uint32_t tempBuffer;
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
ESTADOS_DS18B20 currentStatus;
uint8_t currentCommand;
uint8_t bitsSent;
uint8_t bitsRead;
uint8_t tConv;
uint8_t sFlag;

uint8_t OWireDelay;
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
	\fn  OWire_Init
	\brief Inicializacion de un Modulo One Wire
 	\author Manuel A. Rafaele
 	\date 30 de Nov. de 2017
 	\param void
	\return void
*/
void OWire_Init(void)							//Inicializa el Timer y GPIO para One Wire
{
	PCONP->bits._PCTIM3 = 1;
	//Prendemos el TIMER3

	PCLKSEL1 |= (2<<14);
	// Elegimos la base de clock para el TIMER3 igual a CCLK/2

	T3TCR &= ~(0x1);
	//Apago el Timer mientras configuro
	*T3CTCR = 0x00;
	// Seteo el Count Control Register para que cuente pasos de CCLK

	T3PR = 1;
	//Prendemos el Prescale para que añada uno a TC cada 2 ciclos

	SetPINSEL(tempData, PINSEL_GPIO);
	// Seteamos el pin para ser GPIO

	SetDIR(tempData, SALIDA);
	//configuramos el GPIO para que al cambiarlo a input ya este con el
	SetMODE(tempData, PULLUP);
	//Pull-Up Resistor encendido

	T3MCR |= 1;
	// Prendemos las interrupciones del Match 0 Timer 3

	T3MR0 = uS*50;
	// Seteamos el match Register para interrumpir dentro de 50uS

	T3TCR |= 1;
	//Ponemos a correr el timer nuevamente

	DIR_TIMER3->TC = 0;
	//Seteo el Counter a 0

	ISE_TIMER3;
	//Prendo las interrupciones del Timer 3 en el NVIC
}

void TIMER3_IRQHandler(void)
{
	switch(currentStatus)
	{
		case RESET_TX:
			ResetTx();
			currentStatus = RESET_RX;
			break;
		case RESET_RX:
			ResetRx();
			break;
		case ROMSKIP_TX:
			if(sFlag)
			{
				TempDataOutput;
				currentCommand = SKIP_ROM;
				sFlag = 0;
			}
			TxCommand();
			if(bitsSent == 8)
			{
				bitsSent = 0;
				sFlag = 1;
				if(tConv)
					currentStatus = R_SCRP_TX;
				else
					currentStatus = CONV_TX;
				T3MR0 += 3*uS;
			}
			break;
		case CONV_TX:
			if(sFlag)
			{
				TempDataOutput;
				currentCommand = CONVERT_TEMPERATURE;
				sFlag = 0;
			}
			TxCommand();
			if(bitsSent == 8)
			{
				bitsSent = 0;
				sFlag = 1;
				currentStatus = CONV_RX;
				T3MR0 += 3*uS;
			}
			break;
		case CONV_RX:
			ConvRx();
			break;
		case R_SCRP_TX:
			if(sFlag)
			{
				TempDataOutput;
				currentCommand = READ_SCRATCHPAD;
				sFlag = 0;
			}
			TxCommand();
			if(bitsSent == 8)
			{
				bitsSent = 0;
				sFlag = 1;
				currentStatus = R_SCRP_RX;
				T3MR0 += 3*uS;	//Una vez que se envia el byte se esperan 3 uS antes de intentar nuevamente
			}
		case R_SCRP_RX:
			if(sFlag)
			{
				TempDataInput;
				tempBuffer = 0;
				sFlag = 0;
			}
			ReadScratchpad();
			if(bitsRead == 16)
			{
				bitsRead = 0;
				tConv = 0;
				currentStatus = RESET_TX;
				T3MR0 += 2*S; //Una vez conseguimos una Conversion Correcta esperamos al menos dos segundos antes de hacer la proxima conversion
			}
		default:
			currentStatus = RESET_TX;
			currentCommand = 0x00;
			T3MR0 += 2*S;	//Si ocurre un fallo por ruido entonces esperamos dos segundos antes de volver a intentar
			break;
	}
}

void ResetTx(void)
{
	sFlag = 1;
	bitsSent = 0;
	bitsRead = 0;
	currentCommand = 0x00;
	TempDataOutput;
	W0TempData;
	T3MR0 += (uS*480);
}

void ResetRx(void)
{
	TempDataInput;
	static uint8_t tiempo_contado = 0;
	
	if(GetTempData)
	{
		currentStatus = ROMSKIP_TX; //comenzamos a transmitir el Skip Rom Command
		T3MR0 += (uS*3);
		tiempo_contado = 0;
	}
	else if(tiempo_contado <= 250)
	{
		tiempo_contado += 30;
		T3MR0 += (uS*30); // Si el sensor todavia no dio señales de vida entonces seguimos esperando
	}
	else
	{
		tiempo_contado = 0;
		T3MR0 += (2*S); //Esperamos dos segundos antes de volver a intentar si es que falla la conexion
		currentStatus = RESET_TX;
	}
}

void ConvRx(void)
{
	TempDataInput;
	static uint8_t tiempo_contado = 0;
	
	if(!(GetTempData))
	{
		currentStatus = RESET_TX;
		tConv = 1;
		tiempo_contado = 0;
		T3MR0 += uS*3;
	}
	else if(tiempo_contado <= 500)
	{
		tiempo_contado  += 40;
		T3MR0 = uS*40;
	}
	else
	{
		tiempo_contado = 0;
		tConv = 0;
		T3MR0 += uS*10;
		currentStatus = RESET_TX;
	}
	
}

void ReadScratchpad(void)
{
	tempBuffer |= GetTempData;
	T3MR0 += 30*uS;
	tempBuffer = tempBuffer << 1;
	bitsRead++;
}

void TxCommand(void)
{
	if (!(currentCommand & 0x01))
	{// Escribo un 0
		W1TempData;
		for(OWireDelay = uS ; OWireDelay ; OWireDelay-- );
		W0TempData; //bajo la señal para generar el cero

		T3MR0 += (uS*60);
	}
	else
	{// Escribo un 1
		W1TempData;
		for(OWireDelay = uS ; OWireDelay ; OWireDelay-- );
		W0TempData;
		for(OWireDelay = uS ; OWireDelay ; OWireDelay-- );

		W1TempData;
		T3MR0 += (uS*60);
	}

	currentCommand = currentCommand >> 1;
	bitsSent++;
}




