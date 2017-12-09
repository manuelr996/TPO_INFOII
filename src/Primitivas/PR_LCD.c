/*******************************************************************************************************************************//**
 *
 * @file		Template.c
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Fecha de creacion del archivo XX/XX/XXXX
 * @author		Nombre, Apellido
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_LCD.h"
#include"PR_LCD.h"
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
 \fn  int main(void)
 \brief funcion principal
 \param void
 \return void
 */

/*void Display_lcd( char *msg , char renglon , char posicion )
{
	unsigned char i ;

	if( renglon )
        posicion = posicion + 0xc0 ;
	else
		posicion = posicion + 0x80 ;

	Dato_LCD( posicion , LCD_CONTROL );
	for( i = 0 ; msg[ i ] != '\0' ; i++ )
		Dato_LCD( msg[ i ] , LCD_DATA );

}*/



uint8_t Buffer_LCD[ TOPE_BUFFER_LCD ]; /*!< Buffer de datos relacionados con el LCD*/
uint8_t inxOutLCD; /*!< Indice de output del LCD (Capa drivers)*/
uint8_t inxInLCD; /*!< Indice de input del LCD (Capa primitivas)*/
uint8_t cantidadColaLCD = 0; /*!< Cantidad de datos que faltan convertir del buffer del LCD*/

/**
 * @brief Funcion para poner un dato en el buffer del LCD
 * @param dato Dato a poner en el buffer del LCD
 * @param control Indica si es un dato de status o de control
 * @return Devuelve -1 en caso de error, o 0 en caso contrario
 */

int8_t PushLCD ( uint8_t dato , uint8_t control )
{

	if( cantidadColaLCD >= TOPE_BUFFER_LCD )
		return -1;

	Buffer_LCD[ inxInLCD ] = ( dato >> 4 ) & 0x0f;
	if( control == LCD_CONTROL )
		Buffer_LCD[ inxInLCD ] |= 0x80;

	inxInLCD++;

	Buffer_LCD[ inxInLCD ] = dato & 0x0f;
	if( control == LCD_CONTROL )
		Buffer_LCD[ inxInLCD ] |= 0x80;

	cantidadColaLCD += 2;

	inxInLCD++;
	inxInLCD %= TOPE_BUFFER_LCD;

	return 0;
}

/**
 * @brief Funcion para escribir un mensaje en el LCD
 * @param msg Puntero donde comienza el mensaje a escribir
 * @param r Renglon a escribir el texto
 * @param p Posicion desde la cual se empieza a escribir el mensaje
 */

void Display_LCD( char *msg , char r , char p )
{
	uint8_t i;

	switch( r ){
		case RENGLON_1:
			PushLCD( p + 0x80 , LCD_CONTROL );
			break;

		case RENGLON_2:
			PushLCD( p + 0xC0 , LCD_CONTROL );
			break;
	}

	for( i = 0 ; msg[ i ] != '\0' ; i++ ){
		PushLCD( msg[ i ] , LCD_DATA );
	}
}

/**
 * @brief Funcion que rellena el mensaje a enviar con espacios vacios asi se 'borran' los anteriores lugares del LCD
 * @param mensaje Puntero al mensaje a escribir
 * @param lugar Puntero al lugar donde se quiere guardar el mensaje pasado
 */

void GuardarMensajeLCD( char *mensaje, char *lugar ){
	uint8_t i;

	for( i = 0; mensaje[i] != 0; i++ ){
		lugar[i] = mensaje[i];
	}

	for( ; i < 16; i++ ){
		lugar[i] = ' ';
	}

	lugar[17] = 0;

}
