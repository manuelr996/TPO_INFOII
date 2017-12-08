/*******************************************************************************************************************************//**
 *
 * @file		void Display(unsigned int Val,unsigned char dsp)
 * @brief		valor int a presentar en el display
 * @date
 * @author		Año 2016
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_7seg.h"
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
const uint8_t Tabla_Digitos_BCD_7seg[10] = { 0x3f, 0x06, 0x5B, 0x4f, 0x66, 0x6D, 0x7D, 0x07, 0x7f, 0x67};
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
/**/

/*void Display(unsigned int Val)
{
    uint8_t i;
	uint8_t aux[ DIGITOS ];

	for( i = DIGITOS -1 ; i ; i-- )
	{
		aux[ i ] = Tabla_Digitos_BCD_7seg[ Val % 10 ];
		Val /= 10;
	}
	aux[ i ] = Tabla_Digitos_BCD_7seg[ Val % 10 ];

	TICKINT = 0;
	for ( i = DIGITOS -1 ; i ; i-- )
		Digito_Display[ i ] = aux[ i ];

	Digito_Display[ i ] = aux[ i ];

	TICKINT = 1;
}*/

void Display(unsigned int Val,unsigned char dsp)
{
	unsigned char a ;
	char aux[4];
	aux[0] = aux[1] = aux[2] = aux[3] = 0;

	// Convierto a 7 Seg.
	for(a = 2 ; a ; a-- )
	{
		aux[a] = Tabla_Digitos_BCD_7seg[ Val % 10];
		Val /= 10;
	}

	aux[ 0 ] = Tabla_Digitos_BCD_7seg[ Val ];

	switch(dsp)
	{
		case DSP0:
			Digito_Display[ 2 ] = aux[ 2 ];
			Digito_Display[ 1 ] = aux[ 1 ];
			Digito_Display[ 0 ] = aux[ 0 ];
			break;

		case DSP1:
			Digito_Display[ 5 ] = aux[ 2 ];
			Digito_Display[ 4 ] = aux[ 1 ];
			Digito_Display[ 3 ] = aux[ 0 ];
			break;
	}
}

