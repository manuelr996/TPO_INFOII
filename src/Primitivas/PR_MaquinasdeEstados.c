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
 * 		-"#tnn$" en donde nn representara el entero de la temperatura actual en °C
 * 		-"#hnn$" en donde nn representara el entero de la humedad actual en %
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
uint8_t vUnidad = 0; //variable utilizada para definir la unidad a la hora de editar
int32_t vConfig = 0; //variable utilizada para almacenar los valores que se cargan en la estructura de configuracion
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
uint8_t EstadoAnterior;

char vString[9]; //variable para mostrar valores por el display

RTC_t AlarmTime; //RTC para mostrar la alarma por el display
RTC_t TemporizadoTime; //RTC para mostrar el temporizador por el display
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
		Estado = MANUAL;
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

	GuardarConfiguracion(&config);
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
void CloseEstados(void)
{
	if( ESTADOVALVULA == ON )
	{
		EV_RIEGO_OFF;
		TransmitirString( C_REGANDO_O );
	}
	ApagarLeds();
	TimerStop(E_Riego);
	TimerStop(E_Print);
}

void InitAutomatico(void)
{
	CloseEstados();
	PrenderLed(VERDE);
	TransmitirString(C_AUTOMATICO);
	Display_LCD("Modo Automatico " , RENGLON_1 , 0 );
	Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
	TimerStart(E_Print,T_Print,PrintHumMin,B_Print);
	config.ultEst = AUTOMATICO;
}

void InitManual(void)
{
	CloseEstados();
	PrenderLed(AZUL);
	TransmitirString(C_MANUAL);
	Display_LCD("   Modo Manual  " , RENGLON_1 , 0 );
	Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
	config.ultEst = MANUAL;
}

void InitTemporizado(void)
{
	CloseEstados();
	PrenderLed(ROJO);
	PrenderLed(VERDE);
	TransmitirString(C_TEMPORIZADO);
	Display_LCD("Modo Temporizado" , RENGLON_1 , 0 );
	TimerStart(E_Print,T_Print,PrintCurrentTime,B_Print);
	config.ultEst = TEMPORIZADO;
}

void InitConfiguracion( void )
{
	CloseEstados();
	PrenderLed(ROJO);
	TransmitirString(C_CONFIGURACION);
	Display_LCD(" Configuracion  ", RENGLON_1, 0);
	Display_LCD(" OK p/continuar ", RENGLON_2, 0);
	PushLCD(0x0D,LCD_CONTROL);
	EstadoAnterior = Estado;
	config.ultEst = CONFIGURACION;
}

void CloseConfiguracion(void)
{
	PushLCD(0x0C, LCD_CONTROL);
	SwitchEstados(EstadoAnterior);
}

void ParsearConfiguracion(const char *src) //recibe las configuraciones en un formato hhHHhhmmHHMM
{
	RTC_t aux = {0}; //aux.DayOfWeek=0; aux.DayofMonth=0; aux.Hours=0; aux.Minutes=0; aux.Month=0; aux.Seconds=0; aux.Year=0;

	EV_RIEGO_ON;
	config.humMin 		= (src[0] - '0')*10;
	config.humMin		+= (src[1] - '0');
	config.humMax 		= (src[2] - '0')*10;
	config.humMax 		+= (src[3] - '0');
	aux.Hours			= (src[4] - '0')*10;
	aux.Hours			+= (src[5] - '0');
	//un espacio por los ':'
	aux.Minutes			= (src[7] - '0')*10;
	aux.Minutes			+= (src[8] - '0');
	config.vTempo = ToTimer(&aux,MIN);

	aux.Hours		= (src[9] - '0')*10;
	aux.Hours 	+= (src[10] - '0');
	//un espacio por los ':'
	aux.Minutes	= (src[12] - '0')*10;
	aux.Minutes 	+= (src[13] - '0');
	config.vAlarm = ToTimer(&aux,MIN);

	SetAlarm(aux);

	GuardarConfiguracion(&config);

	EV_RIEGO_OFF;
}

void ComponerHumedad(uint8_t pot, char *dest)
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

void ComponerTemporizadorCorto(const RTC_t *timer, char *dest)
{
	dest[0] = (timer->Hours/10) + '0';
	dest[1] = (timer->Hours%10) + '0';
	dest[2] = ':';
	dest[3] = (timer->Minutes/10) + '0';
	dest[4] = (timer->Minutes%10) + '0';
	dest[5] = 0;
	
}

void ComponerTemporizador(const RTC_t *timer, char *dest)
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

//////////////////////////PRINTS AUTOMATICO//////////////////////////////////////////
void PrintHumMin (void)
{
	Display_LCD("Humedad Min=   %", RENGLON_2 , 0 );
	ComponerHumedad(config.humMin, vString);
	Display_LCD(vString, RENGLON_2, 12);
	TimerStart(E_Print,T_Print_largo,PrintHumMax,B_Print);
}

void PrintHumMax (void)
{
	Display_LCD("Humedad Max=   %", RENGLON_2 , 0 );
	ComponerHumedad(config.humMax, vString);
	Display_LCD(vString, RENGLON_2, 12);
	TimerStart(E_Print,T_Print_largo,PrintEstadoAutomatico,B_Print);
}

void PrintEstadoAutomatico (void)
{
	if(EstadoAutomatico == NO_REGANDO)
		Display_LCD("   Riego: OFF   " , RENGLON_2 , 0 );
	else
		Display_LCD("   Riego: ON    " , RENGLON_2 , 0 );

	TimerStart(E_Print,T_Print_largo,PrintHumMin,B_Print);
}

//////////////////////////PRINTS CONFIGURACION///////////////////////////////////////
void PrintHumedad(void)
{
	ComponerHumedad(vConfig,vString);
	Display_LCD(vString, RENGLON_2, 12);

	if(vUnidad == Decena)
		MoverCursorLCD(13,RENGLON_2);
	else
		MoverCursorLCD(14,RENGLON_2);

	SetTimer(E_Print,T_Print);
}

void PrintHour(void)
{
	AlarmTime = FromGetTimer(vConfig, MIN); //FromGetTimer toma un entero y lo convierte a la estructura rtc

	ComponerTemporizadorCorto(&AlarmTime,vString);

	Display_LCD(vString, RENGLON_2, 10);

	if(vUnidad == HrD)
		MoverCursorLCD(10,RENGLON_2);
	else if(vUnidad == HrU)
		MoverCursorLCD(11,RENGLON_2);
	else if(vUnidad == MinD)
		MoverCursorLCD(13,RENGLON_2);
	else if(vUnidad == MinU)
		MoverCursorLCD(14,RENGLON_2);

	SetTimer(E_Print,T_Print);
}

void PrintAlarmaOnOff(void)
{
	if( vConfig )
		Display_LCD("ON ", RENGLON_2, 10);
	else
		Display_LCD("OFF", RENGLON_2, 10);

	SetTimer(E_Print,T_Print);
}

void PrintConfigHumMin(void)
{
	Display_LCD(" Humedad Minima ", RENGLON_1 , 0 );
	Display_LCD("Humedad Min=   %", RENGLON_2 , 0 );
	TimerStart(E_Print,T_Print,PrintHumedad,B_Print);
}

void PrintConfigHumMax(void)
{
	Display_LCD(" Humedad Maxima ", RENGLON_1 , 0 );
	Display_LCD("Humedad Max=   %", RENGLON_2 , 0 );
	TimerStart(E_Print,T_Print,PrintHumedad,B_Print);
}

void PrintTempDeRiego(void)
{
	PushLCD(0x0D, LCD_CONTROL);
	TimerStart(E_Print,T_Print,PrintHour,B_Print);
	Display_LCD( " Temp. de Riego " , RENGLON_1 , 0 );
	Display_LCD( "Tiempo=         " , RENGLON_2 , 0 );
}

void PrintPrenderAlarma(void)
{
	TimerStart(E_Print,T_Print,PrintAlarmaOnOff,B_Print);
	PushLCD(0x0C, LCD_CONTROL);
	Display_LCD( " Prender Alarma?", RENGLON_1, 0);
	if(config.estAlrm)
		Display_LCD("Alarma:   ON    ", RENGLON_2, 0);
	else
		Display_LCD("Alarma:   OFF   ", RENGLON_2, 0);
}

void PrintConfigAlarma(void)
{
	PushLCD(0x0D, LCD_CONTROL);
	TimerStart(E_Print,T_Print,PrintHour,B_Print);
	Display_LCD("   Hora Alarma   ", RENGLON_1, 0);
	Display_LCD( "Hora:           ", RENGLON_2, 0);
}

void PrintConfigHora(void)
{
	PushLCD(0x0D, LCD_CONTROL);
	TimerStart(E_Print,T_Print,PrintHour,B_Print);
	Display_LCD(" Configurar Hora ", RENGLON_1, 0);
	Display_LCD( "Hora:           ", RENGLON_2, 0);
}

void PrintCerrarConfig(void)
{
	TimerStop(E_Print);
	Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
	Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
}

//////////////////////////PRINTS TEMPORIZADO/////////////////////////////////////////
void PrintRemainingTime(void)
{
	TemporizadoTime = FromGetTimer(GetTimer(E_Riego),B_Riego);

	ComponerTemporizador(&TemporizadoTime,vString);
	//Display_LCD("T_Rest:",RENGLON_2,0);
	Display_LCD(vString, RENGLON_2, 8);

	SetTimer(E_Print,T_Print);
}

void PrintCurrentTime(void)
{
	TemporizadoTime = GetTime();

	ComponerTemporizador(&TemporizadoTime,vString);

	Display_LCD("Hora:   ",RENGLON_2,0);
	Display_LCD(vString, RENGLON_2, 8);

	if(config.estAlrm)
		TimerStart(E_Print,T_Print_largo,PrintAlarm,B_Print);
	else
		TimerStart(E_Print,T_Print_largo,PrintTimer,B_Print);
}

void PrintAlarm(void)
{
	RTC_t aux = GetAlarm();

	Display_LCD("Alarma:         ",RENGLON_2,0);

	ComponerTemporizadorCorto(&aux,vString);

	Display_LCD(vString, RENGLON_2, 10);

	TimerStart(E_Print,T_Print_largo,PrintTimer,B_Print);
}

void PrintStatus(void)
{
	Display_LCD("Valvula:   OFF  ", RENGLON_2,0);

	TimerStart(E_Print,T_Print_largo,PrintCurrentTime,B_Print);
}

void PrintTimer(void)
{
	TemporizadoTime = FromGetTimer(config.vTempo, MIN);

	ComponerTemporizadorCorto(&TemporizadoTime,vString);
	Display_LCD("T_Riego:         ",RENGLON_2,0);
	Display_LCD(vString, RENGLON_2, 10);

	TimerStart(E_Print,T_Print_largo,PrintStatus,B_Print);
}

