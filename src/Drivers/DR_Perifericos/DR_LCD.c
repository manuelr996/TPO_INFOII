/*
 * DR_LCD.c
 *
 *  Created on: Oct 22, 2017
 *      Author: USER
 */

#ifndef SRC_DRIVERINFOTRONIC_DR_LCD_C_
#define SRC_DRIVERINFOTRONIC_DR_LCD_C_


/***********************************************************************************************************
 *
 * @file		Template.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Fecha de creacion del archivo XX/XX/XXXX
 * @author		Nombre, Apellido
 *
 *********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_LCD.h"
#include"PR_LCD.h"
#include"DR_Inicializacion.h"
#include"DR_gpio.h"
#include"DR_pinsel.h"
/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
extern uint8_t Buffer_LCD[ TOPE_BUFFER_LCD ]; /*!< Buffer de datos relacionados con el LCD*/
extern uint8_t inxOutLCD; /*!< Indice de output del LCD (Capa drivers)*/
extern uint8_t inxInLCD; /*!< Indice de input del LCD (Capa primitivas)*/
extern uint8_t cantidadColaLCD; /*!< Cantidad de datos que faltan convertir del buffer del LCD*/
uint8_t	Demora_LCD; /*!< Variable para realizar las demoras necesarias en la inicializacion del LCD*/

/**
 * @brief Funcion encargada de inicializar el LCD
 */

void InitLCD( void ){
	uint16_t i;

	SetPINSEL( LCD_D4 , 0 );
	SetPINSEL( LCD_D5 , 0 );
	SetPINSEL( LCD_D6 , 0 );
	SetPINSEL( LCD_D7 , 0 );
	SetPINSEL( LCD_RS , 0 );
	SetPINSEL( LCD_E , 0 );

	SetDIR( LCD_D4 , SALIDA );
	SetDIR( LCD_D5 , SALIDA );
	SetDIR( LCD_D6 , SALIDA );
	SetDIR( LCD_D7 , SALIDA );
	SetDIR( LCD_RS , SALIDA );
	SetDIR( LCD_E , SALIDA );

	SetPIN ( LCD_E , OFF );
	Demora_LCD = 50;
	while( Demora_LCD );

	for( i = 0 ; i < 3 ; i++ ){
		SetPIN ( LCD_D4 , ON );
		SetPIN ( LCD_D5 , ON );
		SetPIN ( LCD_D6 , OFF );
		SetPIN ( LCD_D7 , OFF );
		SetPIN ( LCD_RS , OFF );
		SetPIN ( LCD_E , ON );
		SetPIN ( LCD_E , OFF );

		Demora_LCD = 10;
		while( Demora_LCD );
	}

	SetPIN ( LCD_D4 , OFF );
	SetPIN ( LCD_D5 , ON );
	SetPIN ( LCD_D6 , OFF );
	SetPIN ( LCD_D7 , OFF );
	SetPIN ( LCD_RS , OFF );
	SetPIN ( LCD_E , ON );
	SetPIN ( LCD_E , OFF );

	Demora_LCD = 5;
	while( Demora_LCD );

	PushLCD( 0x28 , LCD_CONTROL );	//N = 1; 2 lineas; 5x7 puntos
	Demora_LCD = 4;
	while( Demora_LCD );
	PushLCD( 0x08 , LCD_CONTROL );	//Cursor off
	Demora_LCD = 4;
	while( Demora_LCD );
	PushLCD( 0x01 , LCD_CONTROL );	//Clear display
	Demora_LCD = 4;
	while( Demora_LCD );
	PushLCD( 0x06 , LCD_CONTROL );	//incrementa puntero
	Demora_LCD = 4;
	while( Demora_LCD );
	PushLCD( 0x0C , LCD_CONTROL );	//Activo LCD
	Demora_LCD = 4;
	while( Demora_LCD );
}

/**
 * @brief Funcion encargada de poner un dato en el LCD desde el buffer
 */

void Dato_LCD( void )
{
	int16_t data;

	if( ( data = PopLCD( ) ) == -1 )
		return;

	SetPIN ( LCD_D4 , ( ( (uint8_t) data ) >> 0 ) & 0x01 );
	SetPIN ( LCD_D5 , ( ( (uint8_t) data ) >> 1 ) & 0x01 );
	SetPIN ( LCD_D6 , ( ( (uint8_t) data ) >> 2 ) & 0x01 );
	SetPIN ( LCD_D7 , ( ( (uint8_t) data ) >> 3 ) & 0x01 );

	if( ( (uint8_t) data ) & 0x80 )
		SetPIN( LCD_RS , OFF );
	else
		SetPIN( LCD_RS , ON );

	SetPIN ( LCD_E , ON );
	SetPIN ( LCD_E , OFF );
}

/**
 * @brief Funcion encargada de sacar un dato del buffer del LCD
 * @return Devuelve el dato o -1 si no hay dato en el buffer
 */

int16_t PopLCD( void )
{
	int16_t dato;

	if( cantidadColaLCD == 0 )
		return -1;

	dato = Buffer_LCD[ inxOutLCD ];

	cantidadColaLCD--;

	inxOutLCD++;
	inxOutLCD %= TOPE_BUFFER_LCD;

	return dato;
}



#endif /* SRC_DRIVERINFOTRONIC_DR_LCD_C_ */
