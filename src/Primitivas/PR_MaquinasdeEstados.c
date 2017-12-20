/*******************************************************************************************************************************//**
 *
 * @file		PR_MaquinasdeEstados.c
 * @brief		Descripcion del modulo
 * @date		Dec 8, 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/


/*
 * Logica de Mensajes a la UART:
 * La aplicacion va a recibir informacion sobre el estado actual de la maquina asi poder reflejarlo
 * y cada dos segundos enviara la informacion que obtuvo de los sensores...
 *
 * La informacion de los sensores se enviara en dos mensajes de la siguiente manera:
 * 		-"#Tnn$" en donde nn representara el entero de la temperatura actual en °C
 * 		-"#Hnn$" en donde nn representara el entero de la humedad actual en %
 * 		-"#Pnn$" en donde nn representara el entero del Potenciometro actual en %
 * La informacion de el estado actual se  enviara de la misma forma que se recibe.
 * Es decir, "#[Estado][Accion]$" como se realiza la recepcion.
 *
 * En este archivo se encuentran las las cargas en el buffer para los estados. dentro de
 * PR_Sensores.c se encontraran las llamadas hacia la UART para el envio de los datos.
 *
 */


/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_MaquinasdeEstados.h"

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
uint8_t EstadoAnterior;
/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/

void SwitchEstados(EstadosGenerales nuevoEstado)
{
	switch(nuevoEstado)
	{
	case RESET_G:
		Estado = RESET_G;
	case AUTOMATICO:
		InitAutomatico();
		Estado = AUTOMATICO;
		EstadoAutomatico = NO_REGANDO;
		btn = NO_KEY;
		break;
	case CONFIGURACION:
		InitConfiguracion();
		Estado = CONFIGURACION;
		EstadoConfiguracion = INIT_CONFIGURACION;
		btn = NO_KEY;
		break;
	case MANUAL:
		InitManual();
		Estado = MANUAL;
		EstadoManual = RIEGO_OFF;
		btn = NO_KEY;
		break;
	case TEMPORIZADO:
		InitTemporizado();
		Estado = TEMPORIZADO;
		EstadoTemporizado = AGUARDANDO_OK;
		btn = NO_KEY;
		break;
	}
}

/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Manuel A. Rafaele
 	\date Dec 8, 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void InitConfiguracion( void )
{
	CloseEstados();
	PrenderLed(ROJO);
	TransmitirString("#C$");
	Display_LCD(" Configuracion  ", RENGLON_1, 0);
	Display_LCD(" OK p/continuar ", RENGLON_2, 0);
	PushLCD(0x0D,LCD_CONTROL);
	EstadoAnterior = Estado;
	IniciarPotenciometro();
}

void CloseConfiguracion(void)
{
	DetenerPotenciometro();
	SwitchEstados(EstadoAnterior);
}

void CargarConfiguracion(const char *src) //recibe las configuraciones en un formato hhHHhhmmHHMM
{
	RTC_t aux;
	EV_RIEGO_ON;
	HumedadMinima 		= (src[0] - '0')*10;
	HumedadMinima 		+= (src[1] - '0');
	HumedadMaxima 		= (src[2] - '0')*10;
	HumedadMaxima 		+= (src[3] - '0');
	aux.Hours			= (src[4] - '0')*10;
	aux.Hours			+= (src[5] - '0');
	//un espacio por los ':'
	aux.Minutes			= (src[7] - '0')*10;
	aux.Minutes			+= (src[8] - '0');
	T_Riego = ToTimer(&aux,SEG);
	AlarmTime.Hours		= (src[9] - '0')*10;
	AlarmTime.Hours 	+= (src[10] - '0');
	//un espacio por los ':'
	AlarmTime.Minutes	= (src[12] - '0')*10;
	AlarmTime.Minutes 	+= (src[13] - '0');
	EV_RIEGO_OFF;
}

void ComponerPotenciometro(uint8_t pot, char *dest)
{
	if(pot == 100)
	{
		dest[0] = '1';
		pot = 0;
	}
	else
	{
		dest[0] = '0';
	}

	dest[1] = pot/10 + '0';
	dest[2] = pot%10 + '0';
	dest[3] = '\0';
}

void InitAutomatico(void)
{
	CloseEstados();
	PrenderLed(VERDE);
	TransmitirString("#A$");
	Display_LCD("Modo Automatico " , RENGLON_1 , 0 );
	Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
}

void InitManual(void)
{
	CloseEstados();
	PrenderLed(AZUL);
	TransmitirString("#M$");
	Display_LCD("   Modo Manual  " , RENGLON_1 , 0 );
	Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
}

void InitTemporizado(void)
{
	CloseEstados();
	PrenderLed(ROJO);
	PrenderLed(VERDE);
	TransmitirString("#T$");
	Display_LCD("Modo Temporizado" , RENGLON_1 , 0 );
	TimerStart(E_Potenciometro,T_Potenciometro,PrintCurrentTime,B_Potenciometro);
}

void CloseEstados(void)
{
	EV_RIEGO_OFF;
	ApagarLeds();
	TimerStop(E_Riego);
	TimerStop(E_Potenciometro);
	DetenerPotenciometro();
}

void ComponerTemporizador(RTC_t *timer, char *dest)
{
	dest[0] = (timer->Hours/10) + '0';
	dest[1] = (timer->Hours%10) + '0';
	dest[2] = ':';
	dest[3] = (timer->Minutes/10) + '0';
	dest[4] = (timer->Minutes%10) + '0';
	dest[5] = ':';
	dest[6] = (timer->Seconds/10) + '0';
	dest[7] = (timer->Seconds%10) + '0';
	dest[8] = 0;
}

