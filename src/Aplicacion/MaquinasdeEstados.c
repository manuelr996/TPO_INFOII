/*******************************************************************************************************************************//**
 *
 * @file		MaquinasdeEstados.c
 * @brief		Descripcion del modulo
 * @date		Dec 8, 2017
 * @author		Manuel A. Rafaele
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "MaquinasdeEstados.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/

#define Hs24_en_Seg 86399
#define Hs24_en_Min 1439
#define Hr_en_Min 	60
#define Hr_en_Seg 	3600
#define Min_en_Seg	60

#define HrD	 0
#define HrU	 1
#define MinD 2
#define MinU 3
#define SegU 4
#define SegD 5

#define Decena 0
#define Unidad 1
/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/
typedef enum
{
	SALIR = B_MANUAL,
	RESTAR = B_TEMPORIZADO,
	SUMAR = B_AUTOMATICO,
	ADELANTE = B_OK,
	ATRAS = B_CONFIGURACION
}BotoneraConfig;
/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
EstadosConfiguracion EstadoConfiguracion;
EstadosManual EstadoManual;
EstadosTemporizado EstadoTemporizado;
EstadosAutomatico EstadoAutomatico;

CONFIG_t config;
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

uint8_t btnConfig; //lectura del boton en modo configuracion
uint8_t vUnidad = 0; //variable utilizada para definir la unidad a la hora de editar
int32_t vConfig = 0; //variable utilizada para almacenar los valores que se cargan en la estructura de configuracion
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
/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Manuel A. Rafaele
 	\date Dec 8, 2017
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/


///////////////////////////////////MANUAL///////////////////////////////////

void RiegoOn ( void )
{
	EV_RIEGO_ON;
	if(btn == B_OK || UartOk)
	{
		UartOk = 0;
		Display_LCD("OFF" , RENGLON_2 , 10 );
		TransmitirString(C_REGANDO_O);
		EstadoManual = RIEGO_OFF;
	}
}

void RiegoOff ( void )
{
	EV_RIEGO_OFF;
	if(btn == B_OK || UartOk)
	{
		UartOk = 0;
		Display_LCD("ON " , RENGLON_2 , 10 );
		TransmitirString(C_REGANDO_I);
		EstadoManual = RIEGO_ON;
	}
}

/////////////////////////////////AUTOMATICO/////////////////////////////////

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

void RiegoAutomaticoOn( void )
{
	EV_RIEGO_ON;
	if(CondicionesFin())
	{
		TransmitirString(C_REGANDO_O);
		EstadoAutomatico = NO_REGANDO;
	}
}

void RiegoAutomaticoOff( void )
{
	EV_RIEGO_OFF;
	if(CondicionesInicio())
	{
		TransmitirString(C_REGANDO_I);
		EstadoAutomatico = REGANDO;
	}
}

////////////////////////////////CONFIGURACION///////////////////////////////

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

void ConfiguracionInicializada (void)
{
	btnConfig = getTecla();

	if(btnConfig == ADELANTE)
	{
		Display_LCD(" Humedad Maxima ", RENGLON_1 , 0 );
		Display_LCD("Humedad Max=   %", RENGLON_2 , 0 );
		vUnidad = Decena;
		vConfig = config.humMax;
		TimerStart(E_Print,T_Print,PrintHumedad,B_Print);
		EstadoConfiguracion = HUMEDADMAXIMA_D;
	}
	else if( btnConfig == SALIR )
	{
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
	}
}

void SetHumedadMaximaDecenas (void)
{
	btnConfig = getTecla();

	switch(btnConfig)
	{
		case SUMAR:
		{
			if((vConfig+10)<100)
				vConfig += 10;
			else
				vConfig = 100;
			break;
		}
		case RESTAR:
		{
			if( (vConfig-10) > config.humMin)
				vConfig -= 10;
			else
				vConfig = config.humMin + 1;
			break;
		}
		case ADELANTE:
		{
			vUnidad = Unidad; //editamos unidades
			EstadoConfiguracion = HUMEDADMAXIMA_U;
			break;
		}
		case SALIR:
		{
			config.humMax = vConfig;
			TimerStop(E_Print);
			Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
			Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
			EstadoConfiguracion = CERRAR_CONFIGURACION;
			break;
		}
	}
}

void SetHumedadMaximaUnidades(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
    case SUMAR:
    	if(vConfig < 100)
    		vConfig++;
    	break;
	case RESTAR:
    	if( (vConfig-1) > config.humMin )
    		vConfig--;
    	break;
	case ADELANTE:
		config.humMax = vConfig;
		vConfig = config.humMin;
		vUnidad = Decena;

		Display_LCD(" Humedad Minima ", RENGLON_1 , 0 );
		Display_LCD("Humedad Min=   %", RENGLON_2 , 0 );
		EstadoConfiguracion = HUMEDADMINIMA_D;
		break;
    case ATRAS:
    	vUnidad = Decena;
    	EstadoConfiguracion = HUMEDADMAXIMA_D;
    	break;
	case SALIR:
		config.humMax = vConfig;
		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHumedadMinimaDecenas (void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+10) < config.humMax) //si no nos pasamos del limite maximo sumamos
			vConfig += 10;
		else
			vConfig = config.humMax - 1;
		break;
	case RESTAR:
		if((vConfig-10) > 0) //si no nos vamos al negativo restamos
			vConfig -= 10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = Unidad;	//editamos unidades
		EstadoConfiguracion = HUMEDADMINIMA_U;
		break;
	case ATRAS:
		vUnidad = Unidad; //editamos unidades

		config.humMin = vConfig; //cambiamos la variable que estamos editando actualmente
		vConfig = config.humMax;

    	Display_LCD(" Humedad Maxima ", RENGLON_1, 0);
    	Display_LCD("Humedad Max=   %", RENGLON_2, 0);
    	EstadoConfiguracion = HUMEDADMAXIMA_U;
		break;
	case SALIR:
		config.humMax = vConfig;
		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHumedadMinimaUnidades (void)
{
	btnConfig = getTecla();

    switch(btnConfig)
    {
    case SUMAR:
    	if(vConfig < config.humMax)
    		vConfig++;
    	break;
    case RESTAR:
    	if(vConfig > 0)
    		vConfig--;
    	break;
    case ADELANTE:
    	config.humMin = vConfig;
    	vConfig = config.vTempo; //la multiplicamos por 60 para obtener los segundos
		vUnidad = HrD;

		TimerStart(E_Print,T_Print,PrintHour,B_Print);
		Display_LCD( " Temp. de Riego " , RENGLON_1 , 0 );
		Display_LCD( "Tiempo=         " , RENGLON_2 , 0 );
		EstadoConfiguracion = TEMPORIZADOR_HHD;
    	break;
    case ATRAS:
		vUnidad = Decena; //editamos decenas
		EstadoConfiguracion = HUMEDADMINIMA_D;
		break;
	case SALIR:
		config.humMin = vConfig;
		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
    }
}

void SetTemporizadorHHD(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+Hr_en_Min*10) < Hs24_en_Min)
			vConfig += Hr_en_Min*10;
		else
			vConfig = Hs24_en_Min;
		break;
	case RESTAR:
		if((vConfig-Hr_en_Min*10) > 0)
			vConfig -= Hr_en_Min*10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = HrU;
		EstadoConfiguracion = TEMPORIZADOR_HHU;
		break;
	case ATRAS:
		config.vTempo = vConfig;
		vConfig = config.humMin;
		vUnidad = Unidad;

		Display_LCD(" Humedad Minima ", RENGLON_1 , 0 );
		Display_LCD("Humedad Min=   %", RENGLON_2 , 0 );
		TimerStart(E_Print,T_Print,PrintHumedad,B_Print);

		EstadoConfiguracion = HUMEDADMINIMA_U;
		break;
	case SALIR:
		config.vTempo = vConfig;
		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetTemporizadorHHU(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+Hr_en_Min) < Hs24_en_Min)
			vConfig += Hr_en_Min;
		else
			vConfig = Hs24_en_Min;
		break;
	case RESTAR:
		if((vConfig-Hr_en_Min) > 0)
			vConfig -= Hr_en_Min;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = MinD;
		EstadoConfiguracion = TEMPORIZADOR_MMD;
		break;
	case ATRAS:
		vUnidad = HrD;
		EstadoConfiguracion = TEMPORIZADOR_HHD;
		break;
	case SALIR:
		config.vTempo = vConfig;
		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetTemporizadorMMD(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+10) < Hs24_en_Min)
			vConfig += 10;
		else
			vConfig = Hs24_en_Min;
		break;
	case RESTAR:
		if((vConfig-10) > 0)
			vConfig -= 10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = MinU;
		EstadoConfiguracion = TEMPORIZADOR_MMU;
		break;
	case ATRAS:
		vUnidad = HrU;
		EstadoConfiguracion = TEMPORIZADOR_HHU;
		break;
	case SALIR:
		config.vTempo = vConfig;
		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;

	}
}

void SetTemporizadorMMU(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+1) < Hs24_en_Min)
			vConfig++;
		else
			vConfig = Hs24_en_Min;
		break;
	case RESTAR:
		if((vConfig-1) > 0)
			vConfig--;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		config.vTempo = vConfig;
		vConfig = config.estAlrm;
		TimerStop(E_Print);

		//PushLCD(0x0C, LCD_CONTROL);
		Display_LCD( " Prender Alarma?", RENGLON_1, 0);
		if(config.estAlrm)
			Display_LCD("Alarma:   ON    ", RENGLON_2, 0);
		else
			Display_LCD("Alarma:   OFF   ", RENGLON_2, 0);

		EstadoConfiguracion = ALARMA_ON_OFF;
		break;
	case ATRAS:
		vUnidad = MinD;
		EstadoConfiguracion = TEMPORIZADOR_MMD;
		break;
	case SALIR:
		config.vTempo = vConfig;
		TimerStop(E_Print);

		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetAlarmaOn(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if(vConfig)
			vConfig = 0;
		else
			vConfig = 1;
		break;
	case RESTAR:
		if(vConfig)
			vConfig = 0;
		else
			vConfig = 1;
		break;
	case ADELANTE:
		config.vTempo = vConfig;
		vConfig = config.vAlarm;
		vUnidad = HrD;

		TimerStart(E_Print,T_Print,PrintHour,B_Print);
		Display_LCD("   Hora Alarma   ", RENGLON_1, 0);
		Display_LCD( "Hora:           ", RENGLON_2, 0);
		EstadoConfiguracion = HORA_RIEGO_HHD;
		break;
	case ATRAS:
		config.estAlrm = vConfig;
		vConfig = config.vTempo;
		vUnidad = MinU;

		TimerStart(E_Print,T_Print,PrintHour,B_Print);
		Display_LCD( " Temp. de Riego " , RENGLON_1 , 0 );
		Display_LCD( "Tiempo=         " , RENGLON_2 , 0 );
		EstadoConfiguracion = TEMPORIZADOR_MMU;
		break;
	case SALIR:
		config.estAlrm = vConfig;

		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}


}

void SetHoraRiegoHHD(void)
{
	btnConfig = getTecla();

	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+Hr_en_Min*10) < Hs24_en_Min)
			vConfig += Hr_en_Min*10;
		else
			vConfig = Hs24_en_Min;
		break;
	case RESTAR:
		if((vConfig-Hr_en_Min*10) > 0)
			vConfig -= Hr_en_Min*10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = HrU;
		EstadoConfiguracion = HORA_RIEGO_HHU;
		break;
	case ATRAS:
		config.vAlarm = vConfig;
		vConfig = config.estAlrm;
		TimerStop(E_Print);

		Display_LCD( " Prender Alarma?", RENGLON_1, 0);
		if(config.estAlrm)
			Display_LCD("Alarma:   ON    ", RENGLON_2, 0);
		else
			Display_LCD("Alarma:   OFF   ", RENGLON_2, 0);

		EstadoConfiguracion = ALARMA_ON_OFF;
		break;
	case SALIR:
		config.vAlarm = vConfig;
		SetAlarm(FromGetTimer(config.vAlarm, MIN));

		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}

}

void SetHoraRiegoHHU(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+Hr_en_Min) < Hs24_en_Min)
			vConfig += Hr_en_Min;
		else
			vConfig = Hs24_en_Min;
		break;
	case RESTAR:
		if((vConfig-Hr_en_Min) > 0)
			vConfig -= Hr_en_Min;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = MinD;
		EstadoConfiguracion = HORA_RIEGO_MMD;
		break;
	case ATRAS:
		vUnidad = HrD;
		EstadoConfiguracion = HORA_RIEGO_HHD;
		break;
	case SALIR:
		config.vAlarm = vConfig;
		SetAlarm(FromGetTimer(config.vAlarm, MIN));

		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHoraRiegoMMD(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+10) < Hs24_en_Min)
			vConfig += 10;
		else
			vConfig = Hs24_en_Min;
		break;
	case RESTAR:
		if((vConfig-10) > 0)
			vConfig -= 10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = MinU;
		EstadoConfiguracion = HORA_RIEGO_MMU;
		break;
	case ATRAS:
		vUnidad = HrU;
		EstadoConfiguracion = HORA_RIEGO_HHU;
		break;
	case SALIR:
		config.vAlarm = vConfig;
		SetAlarm(FromGetTimer(config.vAlarm, MIN));

		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHoraRiegoMMU(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+1) < Hs24_en_Min)
			vConfig++;
		else
			vConfig = Hs24_en_Min;
		break;
	case RESTAR:
		if((vConfig-1) > 0)
			vConfig--;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		config.vAlarm = vConfig;
		SetAlarm(FromGetTimer(config.vAlarm, MIN));

		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	case ATRAS:
		vUnidad = MinD;
		EstadoConfiguracion = HORA_RIEGO_MMD;
		break;
	case SALIR:
		config.vAlarm = vConfig;
		SetAlarm(FromGetTimer(config.vAlarm, MIN));

		TimerStop(E_Print);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}

}

void ConfiguracionFinalizada (void)
{
	btnConfig = getTecla();
	if(btnConfig == ATRAS)
	{
		vUnidad = MinU;
		vConfig = config.vAlarm;
		TimerStart(E_Print,T_Print,PrintHour,B_Print);
		Display_LCD("   Hora Alarma   ", RENGLON_1, 0);
		Display_LCD( "Hora:           ", RENGLON_2, 0);
		EstadoConfiguracion = HORA_RIEGO_MMU;
	}
	if(btnConfig == ADELANTE)
	{
		GuardarConfiguracion(&config);
		vConfig = 0;
		CloseConfiguracion();
		TransmitirParametros();
	}
}

////////////////////////////////TEMPORIZADO////////////////////////////////

void PrintRemainingTime (void)
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
	Display_LCD("T_Riego:",RENGLON_2,0);
	Display_LCD(vString, RENGLON_2, 10);

	TimerStart(E_Print,T_Print_largo,PrintStatus,B_Print);
}


void AguardandoOk(void)
{
	if((btn == B_OK || Alarma() || UartOk) && config.estAlrm)
	{
		UartOk = 0;
		EV_RIEGO_ON;
		TransmitirString( C_REGANDO_I );
		Display_LCD("Modo Temporizado", RENGLON_1, 0);
		Display_LCD("T_Rest:         ", RENGLON_2, 0);
		TimerStart(E_Riego, (config.vTempo*60) , VolverAguardando, B_Riego);
		TimerStart(E_Print,T_Print,PrintRemainingTime,B_Print);
		EstadoTemporizado = RIEGO_TEMPORIZADO;
	}
}

void VolverAguardando(void)
{
	Display_LCD("Modo Temporizado" , RENGLON_1 , 0 );
	Display_LCD("  Timer:  OFF   " , RENGLON_2 , 0 );
	EstadoTemporizado = AGUARDANDO_OK;
	TimerStop(E_Print);
	TimerStart(E_Print,T_Print_largo,PrintCurrentTime,B_Print);
	EV_RIEGO_OFF;
	TransmitirString( C_REGANDO_O );
}

void RiegoTemporizado(void)
{

}

