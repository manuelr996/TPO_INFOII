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

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
uint8_t currentCommand;
uint8_t currentStatus;
uint8_t tConv;

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
	DIR_TIMER3->TC = 0;
	//Seteo el Counter a 0
	*T3CTCR = 0x00;
	// Seteo el Count Control Register para que cuente pasos de CCLK

	T3PR = 1;
	//Prendemos el Prescale para que añada uno a TC cada 2 ciclos

	SetPINSEL(tempData, PINSEL_GPIO);
	// Seteamos el pin para ser GPIO
	/*SetPINSEL(TempData, FUNC_3);*/
	// Seteamos el pin con la funcion para ser salida en Match


	SetDIR(tempData, SALIDA);
	SetMODE(tempData, PULLUP);
	//configuramos el GPIO para que al cambiarlo a input ya este con el
	//Pull-Up Resistor encendido
	ISE_TIMER3;
	//Prendo las interrupciones del Timer 3 en el NVIC
	T3MCR |= 1;
	// Prendemos las interrupciones del Match 0 Timer 3

	T3MR0 = uS*50;
	// Seteamos el match Register para interrumpir dentro de 50uS

	T3TCR |= 1;
	//T3EMR |=  0x31;
	// Seteamos el external match para toggle
}

void TIMER3_IRQHandler(void)
{
	static uint8_t sFlag = 1;

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
				SetPIN(tempData, SALIDA);
				currentCommand = SKIP_ROM;
				sFlag = 0;
			}
			TxCommand();
			if(!currentCommand)
			{
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
				SetPIN(tempData, SALIDA);
				currentCommand = CONVERT_TEMPERATURE;
				sFlag = 1;
			}
			TxCommand();
			if(!currentCommand)
			{
				sFlag = 0;
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
				SetPIN(tempData, SALIDA);
				currentCommand = READ_SCRATCHPAD;
			}
		default:
			currentStatus = RESET_RX;
			currentCommand = 0x00;
			T3MR0 += 800*uS;
			break;
	}
	

}

void ResetTx(void)
{
	SetDIR(tempData, SALIDA);
	SetPIN(tempData, BAJO);
	T3MR0 += (uS*480);
}

void ResetRx(void)
{
	SetDIR(tempData, ENTRADA);
	static uint8_t tiempo_contado = 0;
	
	if(GetPIN(tempData,BAJO))
	{
		currentStatus = ROMSKIP_TX; //comenzamos a transmitir el Skip Rom Command
		T3MR0 += (uS*3);
		tiempo_contado = 0;
	}
	else if(tiempo_contado <= 250)
	{
		tiempo_contado += 30;
		T3MR0 += (uS*30); // si todavia no esta
	}
	else
	{
		tiempo_contado = 0;
		T3MR0 += (uS*2);
		currentStatus = RESET_TX;
	}
}

void ConvRx(void)
{
	SetDIR(tempData, ENTRADA);
	static uint8_t tiempo_contado = 0;
	
	if(GetPIN(tempData,BAJO))
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
		T3MR0 += uS*3;
		currentStatus = RESET_TX;
	}
	
}

void TxCommand(void)
{
	if (!(currentCommand & 0x01))
	{// Escribo un 0
		SetPIN(tempData, ALTO);
		SetPIN(tempData, ALTO);	//Genero un retraso de 1uS
		SetPIN(tempData, ALTO);

		SetPIN(tempData, BAJO); //bajo la señal para generar el cero

		T3MR0 += (uS*60);
	}
	else
	{// Escribo un 1
		SetPIN(tempData, ALTO);
		SetPIN(tempData, ALTO); //Genero un retraso de 1uS
		SetPIN(tempData, ALTO);

		SetPIN(tempData, BAJO);
		SetPIN(tempData, BAJO); //Genero un retraso de 1uS
		SetPIN(tempData, BAJO);

		SetPIN(tempData, ALTO);

		T3MR0 += (uS*60);
	}

	currentCommand = currentCommand >> 1;
}




