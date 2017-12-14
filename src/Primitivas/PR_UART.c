/*
* @file		PR_UART.c
 * @brief		Descripcion del modulo
 * @date		5 de oct. de 2017
 * @author		Ing. Marcelo Trujillo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_UART.h"

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
	\fn void TransmitirString ( char * s)
	\brief despacha una string al buffer de transmicion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param [in] uint8_t *s string a transmitir
	\return void
*/
void TransmitirString ( char * s)
{
	while ( *s )
	{
		PushTX( *s );
		s++;
	}
	return;
}

/**
	\fn void Mensaje ( void )
	\brief epera tramas del remoto
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param void
	\return void
*/
//TODO adaptar a funcionamiento

/*Formato de Trama "#[E][A]$"
 * [E]: Estado Actual -> 'M': Manual, 'A': Automatico, 'T': Temporizado
 * [A]: Accion a Tomar -> 	Manual		: 'O': EV_RIEGO_OFF 'I': EV_RIEGO_ON
 * 							Automatico 	: ?????
 * 							Temporizado	: 'S': IniciarRiego 'F': Cancelar Temporizador
 *
 */
void Mensaje ( void )
{
	int16_t dato;
	static char estadoRiego = 0;

	typedef enum
	{
		ESPERANDO_INICIO_DE_TRAMA = 0,
		ESPERANDO_COMANDO,
		ESPERANDO_DATOS,
		ESPERANDO_FIN_DE_TRAMA
	}ESTADOS_TRAMA;

	static ESTADOS_TRAMA trama = ESPERANDO_INICIO_DE_TRAMA;
	char Buffer_Auxiliar[10];

	dato = PopRX();

	if ( dato != -1 )
	{
		switch ( trama )
		{
			case ESPERANDO_INICIO_DE_TRAMA:

				if ( dato == '#')
					trama = ESPERANDO_COMANDO;
				else
					TransmitirString("ERROR\r\n\0");

				break;

			case ESPERANDO_COMANDO:

				if(dato == 'M')
				{
					estadoRiego = 'M';
					InitManual();
					trama = ESPERANDO_DATOS;
				}
				if(dato == 'A')
				{
					estadoRiego = 'A';
					trama = ESPERANDO_DATOS;
				}
				if(dato == 'T')
				{
					estadoRiego = 'T';
					trama = ESPERANDO_DATOS;
				}
				else
				{
					strcpy(Buffer_Auxiliar, "ERROR\r\n\0");
					TransmitirString(Buffer_Auxiliar);
					trama = ESPERANDO_INICIO_DE_TRAMA;
				}
				break;

			case ESPERANDO_DATOS:

				if(estadoRiego == 'M')
				{
					if(dato == 'O')
					{
						EV_RIEGO_OFF;
						Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
					}
					else if(dato == 'I')
					{
						EV_RIEGO_ON;
						Display_LCD("   Riego: ON    " , RENGLON_2 , 0 );
					}
				}
				if(estadoRiego == 'T')
				{

				}
				else
				{	strcpy(Buffer_Auxiliar, "ERROR\r\n\0");
					TransmitirString(Buffer_Auxiliar);
					trama = ESPERANDO_INICIO_DE_TRAMA;
				}
				break;

			case ESPERANDO_FIN_DE_TRAMA:

				if (dato == '$')
				{
					break;
				}
				else
				{
					TransmitirString("ERROR\r\n\0");
				}
		}
	}
}

