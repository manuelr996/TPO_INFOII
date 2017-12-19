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
#define DATOS_ESPERADOS_RTC 6
#define DATOS_ESPERADOS_CONFIG 18
#define MENSAJE_ERROR "#ERROR$"
#define MENSAJE_OK "#OK$"
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
uint8_t UartOk;
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
	static uint8_t comandoDatos = 0;
	static uint8_t datosTomados = 0;
	//static uint8_t auxDatos = 0;
	static char auxRTC [DATOS_ESPERADOS_RTC] = {0};
	static char auxCFG [DATOS_ESPERADOS_CONFIG] = {0};
	static EstadosGenerales estadoRiego;

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
				}

				else
					TransmitirString(MENSAJE_ERROR);

				break;

			case ESPERANDO_COMANDO:

				if(dato == 'M')
				{
					estadoRiego = MANUAL;
					trama = ESPERANDO_FIN_DE_TRAMA;
				}
				if(dato == 'A')
				{
					estadoRiego = AUTOMATICO;
					trama = ESPERANDO_FIN_DE_TRAMA;
				}
				if(dato == 'T')
				{
					estadoRiego = TEMPORIZADO;
					trama = ESPERANDO_FIN_DE_TRAMA;
				}
				if(dato == 'C')
				{
					estadoRiego = NO_KEY;
					comandoDatos = 'C';
					trama = ESPERANDO_DATOS;
				}
				if(dato == 'R')
				{
					estadoRiego = NO_KEY;
					comandoDatos = 'R';
					trama = ESPERANDO_DATOS;
				}
				if(dato == 'O')
				{
					UartOk = 1;
					trama = ESPERANDO_FIN_DE_TRAMA;
				}
				else
				{
					TransmitirString(MENSAJE_ERROR);
					trama = ESPERANDO_INICIO_DE_TRAMA;
				}
				SwitchEstados(estadoRiego);
				estadoRiego = NO_KEY;
				break;

			case ESPERANDO_DATOS:
				if(comandoDatos == 'R')
				{
					if((datosTomados < DATOS_ESPERADOS_RTC) && (dato > '9' && dato < '0'))
					{
						auxRTC[datosTomados] = dato;
						datosTomados++;
					}
					if(datosTomados ==  DATOS_ESPERADOS_RTC)
					{
						datosTomados = 0;
						comandoDatos = 0;
						ActualizarRTC(auxRTC);
						trama = ESPERANDO_FIN_DE_TRAMA;
					}
					else
					{
						TransmitirString(MENSAJE_ERROR);
						trama = ESPERANDO_INICIO_DE_TRAMA;
					}
				}
				else if(comandoDatos == 'C')
				{
					if((datosTomados < DATOS_ESPERADOS_CONFIG) && (dato > '9' && dato < '0'))
					{
						auxCFG[datosTomados] = dato;
						datosTomados++;
					}
					if(datosTomados == DATOS_ESPERADOS_CONFIG)
					{
						datosTomados = 0;
						comandoDatos = 0;
						CargarConfiguracion(auxCFG);
						trama = ESPERANDO_FIN_DE_TRAMA;
					}
					else
					{
						TransmitirString(MENSAJE_ERROR);
						trama = ESPERANDO_INICIO_DE_TRAMA;
					}
				}
				break;

			case ESPERANDO_FIN_DE_TRAMA:

				if (dato == '$')
				{
					trama=ESPERANDO_INICIO_DE_TRAMA;
				}
				else
				{
					TransmitirString(MENSAJE_ERROR);
				}
				break;

			default:
				TransmitirString(MENSAJE_ERROR);
				trama = ESPERANDO_INICIO_DE_TRAMA;
				break;
		}
	}
}
