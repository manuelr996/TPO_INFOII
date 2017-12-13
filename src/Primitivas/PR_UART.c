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
void Mensaje ( void )
{
	int16_t dato;
	static char aux [10];
	static uint8_t Contador_Datos_Recibidos;
	static uint8_t Contador_Datos_Esperados;
	static uint8_t Comando_En_Cuestion;

	typedef enum
	{
		ESPERANDO_INICIO_DE_TRAMA = 0,
		ESPERANDO_COMANDO,
		ESPERANDO_DATOS,
		ESPERANDO_FIN_DE_TRAMA
	} ESTADOS_TRAMA;

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

				//	LISTADO DE COMANDOS CONOCIDOS:
				//
				//  - 'I' = INTERVALOS DE LUBRICACION. TIENE ASOCIADOS 3 DATOS.
				//
				//			EL PRIMERO INDICA EL TIEMPO DE LUBRICACION. [2 BYTES]
				//			EL SEGUNDO INDICA EL INTERVALO ENTRE LUBRICACIONES. [2 BYTES]
				//			EL TERCERO INDICA LA CANTIDAD DE REPETICIONES. [1 BYTE]
				//
				//	- 'C' = COMENZAR LUBRICACION. NO TIENE DATOS ASOCIADOS.
				//
				//	- 'F' = FINALIZAR LUBRICACION. NO TIENE DATOS ASOCIADOS.
				//
				//

				if ( dato == CONFIGURAR_INTERVALOS )
				{
					Contador_Datos_Recibidos = 0;
					Contador_Datos_Esperados = 6;
					Comando_En_Cuestion = CONFIGURAR_INTERVALOS;
					trama = ESPERANDO_DATOS;
				}
				else if ( dato == CONTINUAR_LUBRICACION )
				{
					trama = ESPERANDO_FIN_DE_TRAMA;
					Comando_En_Cuestion = CONTINUAR_LUBRICACION;
				}
				else if ( dato == FINALIZAR_LUBRICACION )
				{
					trama = ESPERANDO_FIN_DE_TRAMA;
					Comando_En_Cuestion = FINALIZAR_LUBRICACION;
				}
				else
				{	strcpy(Buffer_Auxiliar, "ERROR\r\n\0");

					TransmitirString(Buffer_Auxiliar);
					trama = ESPERANDO_INICIO_DE_TRAMA;
				}
				break;

			case ESPERANDO_DATOS:

				if ( dato >= '0' && dato <= '9' )
				{
					aux[Contador_Datos_Recibidos] = dato;
					Contador_Datos_Recibidos++;

					if (Contador_Datos_Recibidos >= Contador_Datos_Esperados)
					{
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
					if ( Comando_En_Cuestion == CONFIGURAR_INTERVALOS )
					{
						T0 = (aux[0] - '0') * 10;
						T0 += (aux[1] - '0');

						T1 = (aux[2] - '0') * 10;
						T1 += (aux[3] - '0');

						T2 = (aux[4] - '0') * 10;
						T2 += (aux[5] - '0');
						CargaTiempos();

						TransmitirString("OK\r\n\0");
						trama = ESPERANDO_INICIO_DE_TRAMA;
					}
					else if ( Comando_En_Cuestion == CONTINUAR_LUBRICACION )
					{	Buffer_Disparo_Por_Trama = 1;
						TransmitirString("OK\r\n\0");
						trama = ESPERANDO_INICIO_DE_TRAMA;
					}
					else if ( Comando_En_Cuestion == FINALIZAR_LUBRICACION )
					{	Buffer_Freno_Por_Trama = 1;
						TransmitirString("OK\r\n\0");
						trama = ESPERANDO_INICIO_DE_TRAMA;
					}
					else
					{	TransmitirString("ERROR\r\n\0");
						trama = ESPERANDO_INICIO_DE_TRAMA;
					}
					break;
				}
				else
				{	TransmitirString("ERROR\r\n\0");
				}
		}
	}
}

