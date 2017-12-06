/*******************************************************************************************************************************//**
 *
 * @file		DR_EINT.c
 * @brief		Descripcion del modulo
 * @date		24 de oct. de 2017
 * @author		Ing. Marcelo Trujillo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_EINT.h"

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
volatile uint8_t Lluvia;
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
 	\author Ing. Marcelo Trujillo
 	\date 24 de oct. de 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void SetRising( uint8_t puerto, uint8_t bit, uint8_t estado)
{
    uint32_t *p = DIRIO0IntEnR;

    if( puerto )
    	p = DIRIO2IntEnR;			//Posiciono correctamente a p apuntando a DIRIO0IntEnR o DIRIO2IntEnR

    if( estado )
         *p |=  ( 0x01 << bit );    //Pongo el 1
    else                            //    o
    	*p &= ~( 0x01 << bit );    	//Pongo el 0
}

void SetFalling( uint8_t puerto, uint8_t bit, uint8_t estado)
{
    uint32_t *p = DIRIO0IntEnF;

    if( puerto )
        p = DIRIO2IntEnF;

    if( estado )
        *p |=  ( 0x01 << bit );    //Pongo el 1
    else                            //    o
        *p &= ~( 0x01 << bit );     //Pongo el 0
}

uint8_t GetRFStatus( uint8_t puerto, uint8_t bit )
{
    uint32_t *p = DIRIO0IntStatR;

    if( puerto )
    	p = DIRIO2IntStatR;			//Posiciono correctamente a p apuntando a DIRIO0IntStatR o DIRIO2IntStatR

    if( *p & ( 0x01 << bit ) )      //Si fue por Rising retorno
        return  RISING;

    p++;                      		//p apunta a IO0IntStatF o IO2IntStatF

    if( *p & ( 0x01 << bit ) )      //Si fue por Falling retorno
    	return FALLING;

    return 0;						//Si no interrumpio por Rising o Falling retorno un 0
}

void ClearGPIOInt( uint8_t puerto, uint8_t bit )
{
    uint32_t *p = DIRIO0IntClr;

    if( puerto )
    	p = DIRIO2IntClr;         	//Posiciono correctamente a p apuntando a DIRIO0IntClr o DIRIO2IntClr

    *p |= ( 0x01 << bit );          //Escribo un 1 en el bit y borro la interrupcion
}

void InitExtIntGPIO ( void )
{
    //Pin:
    SetPINSEL( SENSOR_LLUVIA , FUNC0 );     	//Configuro el pin como GPIO
    SetDIR( SENSOR_LLUVIA , ENTRADA );			//Entrada
    SetMODE( SENSOR_LLUVIA , PULLUP );			//Configuro el pin con una resistencia de pull-up

    //Configuracion:
    SetRising( SENSOR_LLUVIA, ACTIVADO );     	//Entrada digital 2 interrumpe por Rising
    SetFalling( SENSOR_LLUVIA, ACTIVADO );    	//Entrada digital 2 interrumpe por Falling

    //Interrupciones:
    ISER0 |= ( 0x01 << ISE_EINT3 );            	//Habilito la interrupcion para EINT3
}

void EINT3_IRQHandler( void )
{
	uint8_t flanco;

	flanco = GetRFStatus( SENSOR_LLUVIA );

	if( flanco == FALLING )						//Si hubo interrupcion por Falling significa que comenzo a llover
			Lluvia = ON;
	if( flanco == RISING )						//Si hubo interrupcion por Rising significa que paro de llover
		Lluvia = OFF;

	ClearGPIOInt( SENSOR_LLUVIA );				//Borro los flags de interrupcion de ENTDIG2
}
