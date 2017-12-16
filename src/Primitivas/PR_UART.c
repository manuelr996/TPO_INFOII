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
#define CONFIGURAR_INTERVALOS	'I'
#define CONTINUAR_LUBRICACION	'C'
#define FINALIZAR_LUBRICACION	'F'

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
extern uint8_t T0;
extern uint8_t T1;
extern uint8_t T2;

uint8_t Buffer_Disparo_Por_Trama = 0;
uint8_t Buffer_Freno_Por_Trama = 0;

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

 	\param void
	\return void
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
			ESPERANDO_FIN_DE_TRAMA,
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
					{

						trama = ESPERANDO_COMANDO;
						TransmitirString("A\r\n\0");
					}

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
						InitAutomatico();
						trama = ESPERANDO_DATOS;
					}
					if(dato == 'T')
					{
						estadoRiego = 'T';
						InitTemporizado();
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
							Display_LCD("   Riego: APAGOMA   " , RENGLON_2 , 0 );
							trama = ESPERANDO_FIN_DE_TRAMA;
						}
						else if(dato == 'I')
						{
							EV_RIEGO_ON;
							Display_LCD("   Riego: PRENDOMA    " , RENGLON_2 , 0 );
							trama = ESPERANDO_FIN_DE_TRAMA;
						}
					}
					if(estadoRiego == 'T')
					{
						if(dato == 'O')
						{
							EV_RIEGO_OFF;
							Display_LCD("Riego: APAGOTEMP   " , RENGLON_2 , 0 );
							trama = ESPERANDO_FIN_DE_TRAMA;
						}
						else if(dato == 'I')
						{
							EV_RIEGO_ON;
							Display_LCD("Riego: PRENDOTEMP    " , RENGLON_2 , 0 );
							trama = ESPERANDO_FIN_DE_TRAMA;
						}
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
						Display_LCD("Fin trama       " , RENGLON_2 , 0 );
						trama=ESPERANDO_INICIO_DE_TRAMA;

					}
					else
					{
						TransmitirString("ERROR\r\n\0");
					}
					break;

				default:
					Display_LCD("#ERROR\r\n\0", RENGLON_2 , 0 );
					break;
			}
		}
}
