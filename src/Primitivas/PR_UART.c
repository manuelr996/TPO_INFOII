/*
+* @file		PR_UART.c
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
#define DATOS_ESPERADOS_CONFIG 14
#define MENSAJE_ERROR "#ERROR$\n"
#define MENSAJE_OK "#OK$\n"
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
	\fn void TransmitirValvula ( void )
	\brief despacha el estado de la valvula al buffer de transmision
	\return void
*/
void TransmitirValvula ( void )
{
	if( ESTADOVALVULA == ON )
		TransmitirString( C_REGANDO_I );
	else
		TransmitirString( C_REGANDO_O );
}
/**
	\fn void TransmitirEstado ( void )
	\brief Agrega el estado actual de la maquina al buffer de transmision
	\return void
*/
void TransmitirEstado ( void )
{
	switch( Estado )
	{
		case MANUAL:
			TransmitirString(C_MANUAL);
			break;
		case TEMPORIZADO:
			TransmitirString(C_TEMPORIZADO);
			break;
		case AUTOMATICO:
			TransmitirString(C_AUTOMATICO);
			break;
		case CONFIGURACION:
			TransmitirString(C_CONFIGURACION);
			break;
		default:
			break;
	}
}
/**
	\fn void TransmitirParametros ( void )
	\brief despacha los parametros configurados al buffer de transmision
	\return void
*/
void TransmitirParametros ( void )
{
	uint8_t parametros[17];

	RTC_t tiempoAux = FromGetTimer(config.vTempo, MIN );

	parametros[0] = '#';
	parametros[1] = 'c';

	parametros[2] = config.humMin/10 + '0';
	parametros[3] = config.humMin%10 + '0';

	parametros[4] = config.humMax/10 + '0';
	parametros[5] = config.humMax%10 + '0';

	parametros[6] = tiempoAux.Hours/10 + '0';
	parametros[7] = tiempoAux.Hours%10 + '0';
	parametros[8] = tiempoAux.Minutes/10 + '0';
	parametros[9] = tiempoAux.Minutes%10 + '0';

	tiempoAux = GetAlarm();

	parametros[10] = tiempoAux.Hours/10 + '0';
	parametros[11] = tiempoAux.Hours%10 + '0';
	parametros[12] = tiempoAux.Minutes/10 + '0';
	parametros[13] = tiempoAux.Minutes%10 + '0';
	parametros[14] = '$';
	parametros[15] = '\r';
	parametros[16] = '\n';

	TransmitirString( parametros );
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
	static EstadosGenerales estadoRiego = NO_KEY;

	static ESTADOS_TRAMA trama = ESPERANDO_INICIO_DE_TRAMA;
	//char Buffer_Auxiliar[10];

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
				break;

			case ESPERANDO_COMANDO:

				switch( dato )
				{
					case 'M':				//Comando de manual, se cambia la maquina al estado manual
						estadoRiego = MANUAL;
						trama = ESPERANDO_FIN_DE_TRAMA;
						break;

					case 'A':		//Comando de automatico, se cambia la maquina al estado automatico
						estadoRiego = AUTOMATICO;
						trama = ESPERANDO_FIN_DE_TRAMA;
						break;

					case 'T':		//Comando de temporizado, se cambia la maquina al estado temporizado
						estadoRiego = TEMPORIZADO;
						trama = ESPERANDO_FIN_DE_TRAMA;
						break;

					case 'C':		//Comando de configuracion, se reciben datos de la configuracion
						estadoRiego = NO_KEY;
						comandoDatos = 'C';
						trama = ESPERANDO_DATOS;
						break;

					case 'R':		//Comando de RTC, se recibe la hora actal
						estadoRiego = NO_KEY;
						comandoDatos = 'R';
						trama = ESPERANDO_DATOS;
						break;

					case 'O':		//Comando de ok
						UartOk = 1;
						trama = ESPERANDO_FIN_DE_TRAMA;
						break;

					case 'S':		//Comando de status, se contesta con estado de la maquina y estado de la valvula
						TransmitirEstado();
						TransmitirValvula();
						TransmitirParametros();
						trama = ESPERANDO_FIN_DE_TRAMA;
						break;

					default:
						TransmitirString(MENSAJE_ERROR);
						trama = ESPERANDO_INICIO_DE_TRAMA;
						break;
				}

				if(estadoRiego!=NO_KEY)
				{
					SwitchEstados(estadoRiego);
					estadoRiego = NO_KEY;
				}

				break;

			case ESPERANDO_DATOS:
				if(comandoDatos == 'R')
				{
					if((datosTomados < DATOS_ESPERADOS_RTC) && (dato <= '9' && dato >= '0'))
					{
						auxRTC[datosTomados] = (char) dato;
						datosTomados++;
					}
					else if(datosTomados ==  DATOS_ESPERADOS_RTC)
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
					if((datosTomados < DATOS_ESPERADOS_CONFIG) && ((dato <= '9' && dato >= '0') || dato == ':'))
					{
						auxCFG[datosTomados] = (char)dato;
						datosTomados++;
					}
					if(datosTomados == DATOS_ESPERADOS_CONFIG)
					{
						datosTomados = 0;
						comandoDatos = 0;
						CargarConfiguracion(auxCFG);
						trama = ESPERANDO_FIN_DE_TRAMA;
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

				datosTomados = 0;
				comandoDatos = 0;
				break;

			default:
				TransmitirString(MENSAJE_ERROR);
				trama = ESPERANDO_INICIO_DE_TRAMA;
				break;
		}
	}
}
