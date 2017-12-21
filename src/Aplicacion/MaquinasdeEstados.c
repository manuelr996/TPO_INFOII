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
#define T_Print 3

#define Hs24_en_Seg 86399
#define Hr_en_Seg 	3600
#define Min_en_Seg	60


#define Hr 	 0
#define MinD 1
#define MinU 2
#define SegU 3
#define SegD 4

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

uint32_t T_Riego;
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
uint8_t btnConfig;
uint8_t vUnidad = 0;
int32_t vConfig = 0;
char vString[9];
RTC_t AlarmTime;
RTC_t TemporizadoTime;
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
#if (1)
void RiegoOn ( void )
{
	EV_RIEGO_ON;
	if(btn == B_OK || UartOk)
	{
		UartOk = 0;
		Display_LCD("OFF" , RENGLON_2 , 10 );
		TransmitirString("#O$");
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
		TransmitirString("#O$");
		EstadoManual = RIEGO_ON;
	}
}
#endif
/////////////////////////////////AUTOMATICO/////////////////////////////////
#if (1)
void RiegoAutomaticoOn( void )
{
	EV_RIEGO_ON;
	if(CondicionesFin())
	{
		Display_LCD("OFF   " , RENGLON_2 , 10 );
		TransmitirString("#O$");
		EstadoAutomatico = NO_REGANDO;
	}
}

void RiegoAutomaticoOff( void )
{
	EV_RIEGO_OFF;
	if(CondicionesInicio())
	{
		Display_LCD("ON    " , RENGLON_2 , 10 );
		EstadoAutomatico = REGANDO;
	}
}
#endif
////////////////////////////////CONFIGURACION///////////////////////////////
#if (1)
void PrintHumedad(void)
{
	ComponerPotenciometro(vConfig,vString);
	Display_LCD(vString, RENGLON_2, 12);

	if(vUnidad == Decena)
		MoverCursorLCD(13,RENGLON_2);
	else
		MoverCursorLCD(14,RENGLON_2);

	SetTimer(E_Potenciometro,T_Potenciometro);
}

void PrintHour(void)
{
	AlarmTime = FromGetTimer(vConfig, SEG); //1440 = 24*60 => 4095/2.8 = 1462 -> Valor maximo del Potenciometro

	ComponerTemporizador(&AlarmTime,vString);

	Display_LCD(vString, RENGLON_2, 8);

	if(vUnidad == Hr)
		MoverCursorLCD(9,RENGLON_2);
	else if(vUnidad == MinD)
		MoverCursorLCD(11,RENGLON_2);
	else if(vUnidad == MinU)
		MoverCursorLCD(12,RENGLON_2);
	else if(vUnidad == SegD)
		MoverCursorLCD(14,RENGLON_2);
	else
		MoverCursorLCD(15,RENGLON_2);

	SetTimer(E_Potenciometro,T_Potenciometro);
}

void ConfiguracionInicializada (void)
{
	btnConfig = getTecla();
	if(btnConfig == ADELANTE)
	{
		Display_LCD(" Humedad Minima ", RENGLON_1 , 0 );
		Display_LCD("Humedad Min=   %", RENGLON_2 , 0 );
		vUnidad = Decena;
		TimerStart(E_Potenciometro,T_Potenciometro,PrintHumedad,B_Potenciometro);
		EstadoConfiguracion = HUMEDADMINIMA_D;
	}
}

void SetHumedadMinimaDecenas (void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+10)<100) //si no nos pasamos del limite maximo sumamos
			vConfig += 10;
		else
			vConfig = 100;
		break;
	case RESTAR:
		if((vConfig-10)>0)
			vConfig -= 10; //si no nos vamos al negativo restamos
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = Unidad;	//editamos unidades
		EstadoConfiguracion = HUMEDADMINIMA_U;
		break;
	case SALIR:
		HumedadMinima = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
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
    	if(vConfig<100)
    		vConfig++;
    	break;
    case RESTAR:
    	if(vConfig>0)
    		vConfig--;
    	break;
    case ADELANTE:
		HumedadMinima = vConfig;
    	vUnidad = Decena; //editamos decenas
    	Display_LCD(" Humedad Maxima ", RENGLON_1, 0);
    	Display_LCD("Humedad Max=   %", RENGLON_2, 0);
    	EstadoConfiguracion = HUMEDADMAXIMA_D;
    	break;
    case ATRAS:
		vUnidad = Decena; //editamos decenas
		EstadoConfiguracion = HUMEDADMINIMA_D;
		break;
	case SALIR:
		HumedadMinima = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
    }
}

void SetHumedadMaximaDecenas (void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+10)<100)
			vConfig += 10;
		else
			vConfig = 100;
		break;
	case RESTAR:
		if((vConfig-10)>HumedadMinima)
			vConfig -= 10;
		else
			vConfig = HumedadMinima+1;
		break;
    case ADELANTE:
		vUnidad = Unidad; //editamos unidades
		EstadoConfiguracion = HUMEDADMAXIMA_U;
		break;
    case ATRAS:
    	vUnidad = Unidad; //editamos unidades
		Display_LCD(" Humedad Minima ", RENGLON_1 , 0 );
		Display_LCD("Humedad Min=   %", RENGLON_2 , 0 );
		EstadoConfiguracion = HUMEDADMINIMA_U;
		break;
	case SALIR:
		HumedadMaxima = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
    }
}

void SetHumedadMaximaUnidades(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
    case SUMAR:
    	if(vConfig<100)
    		vConfig++;
	case RESTAR:
    	if(vConfig > HumedadMinima)
    		vConfig--;
    	break;
    	break;
	case ADELANTE:
		HumedadMaxima = vConfig;
		vUnidad = Hr;
		TimerStart(E_Potenciometro,T_Potenciometro,PrintHour,B_Potenciometro);
		Display_LCD( " Temp. de Riego " , RENGLON_1 , 0 );
		Display_LCD( "Tiempo=         " , RENGLON_2 , 0 );
		EstadoConfiguracion = TEMPORIZADOR_HH;
		break;
    case ATRAS:
    	vUnidad = Decena;
    	EstadoConfiguracion = HUMEDADMAXIMA_D;
    	break;
	case SALIR:
		HumedadMaxima = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetTemporizadorHH(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+Hr_en_Seg) < Hs24_en_Seg)
			vConfig += Hr_en_Seg;
		else
			vConfig = Hs24_en_Seg;
		break;
	case RESTAR:
		if((vConfig-Hr_en_Seg) > 0)
			vConfig -= Hr_en_Seg;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = MinD;
		EstadoConfiguracion = TEMPORIZADOR_MMD;
		break;
	case ATRAS:
    	Display_LCD(" Humedad Maxima ", RENGLON_1, 0);
    	Display_LCD("Humedad Max=   %", RENGLON_2, 0);
		TimerStart(E_Potenciometro,T_Potenciometro,PrintHumedad,B_Potenciometro);
    	vUnidad = Unidad;
		vConfig = 0;
    	EstadoConfiguracion = HUMEDADMAXIMA_U;
		break;
	case SALIR:
		T_Riego = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
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
		if((vConfig+Min_en_Seg) < Hs24_en_Seg)
			vConfig += Min_en_Seg*10;
		else
			vConfig = Hs24_en_Seg;
		break;
	case RESTAR:
		if((vConfig-Min_en_Seg) > 0)
			vConfig -= Min_en_Seg*10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = MinU;
		EstadoConfiguracion = TEMPORIZADOR_MMU;
		break;
	case ATRAS:
		vUnidad = Hr;
		EstadoConfiguracion = TEMPORIZADOR_HH;
		break;
	case SALIR:
		T_Riego = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
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
		if((vConfig+Min_en_Seg) < Hs24_en_Seg)
			vConfig += Min_en_Seg;
		else
			vConfig = Hs24_en_Seg;
		break;
	case RESTAR:
		if((vConfig-Min_en_Seg) > 0)
			vConfig -= Min_en_Seg;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = SegD;
		EstadoConfiguracion = TEMPORIZADOR_SSD;
		break;
	case ATRAS:
		vUnidad = MinD;
		EstadoConfiguracion = TEMPORIZADOR_MMD;
		break;
	case SALIR:
		T_Riego = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetTemporizadorSSD(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+10) < Hs24_en_Seg)
			vConfig += 10;
		else
			vConfig = Hs24_en_Seg;
		break;
	case RESTAR:
		if((vConfig+Min_en_Seg) > 0)
			vConfig -= 10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = SegU;
		EstadoConfiguracion = TEMPORIZADOR_SSU;
		break;
	case ATRAS:
		vUnidad = MinU;
		EstadoConfiguracion = TEMPORIZADOR_MMU;
		break;
	case SALIR:
		T_Riego = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetTemporizadorSSU(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig) < Hs24_en_Seg)
			vConfig++;
		break;
	case RESTAR:
		if((vConfig) > 0)
			vConfig--;
		break;
	case ADELANTE:
		T_Riego = vConfig;
		vUnidad = Hr;
		Display_LCD("   Hora Alarma   ", RENGLON_1, 0);
		Display_LCD( "Hora:           ", RENGLON_2, 0);
		EstadoConfiguracion = HORA_RIEGO_HH;
		break;
	case ATRAS:
		vUnidad = SegD;
		EstadoConfiguracion = TEMPORIZADOR_SSD;
		break;
	case SALIR:
		T_Riego = vConfig;
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHoraRiegoHH(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+Hr_en_Seg) < Hs24_en_Seg)
			vConfig += Hr_en_Seg;
		else
			vConfig = Hs24_en_Seg;
		break;
	case RESTAR:
		if((vConfig-Hr_en_Seg) > 0)
			vConfig -= Hr_en_Seg;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = MinD;
		EstadoConfiguracion = HORA_RIEGO_MMD;
		break;
	case ATRAS:
		vUnidad = SegU;
		Display_LCD( " Temp. de Riego " , RENGLON_1 , 0 );
		Display_LCD( "Tiempo=         " , RENGLON_2 , 0 );
		EstadoConfiguracion = TEMPORIZADOR_SSU;
		break;
	case SALIR:
		SetAlarm(&AlarmTime);
		vConfig = 0;
		TimerStop(E_Potenciometro);
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
		if((vConfig+Min_en_Seg) < Hs24_en_Seg)
			vConfig += Min_en_Seg*10;
		else
			vConfig = Hs24_en_Seg;
		break;
	case RESTAR:
		if((vConfig-Min_en_Seg) > 0)
			vConfig -= Min_en_Seg*10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = MinU;
		EstadoConfiguracion = HORA_RIEGO_MMU;
		break;
	case ATRAS:
		vUnidad = Hr;
		EstadoConfiguracion = HORA_RIEGO_HH;
		break;
	case SALIR:
		SetAlarm(&AlarmTime);
		vConfig = 0;
		TimerStop(E_Potenciometro);
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
		if((vConfig+Min_en_Seg) < Hs24_en_Seg)
			vConfig += Min_en_Seg;
		else
			vConfig = Hs24_en_Seg;
		break;
	case RESTAR:
		if((vConfig-Min_en_Seg) > 0)
			vConfig -= Min_en_Seg;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = SegD;
		EstadoConfiguracion = HORA_RIEGO_SSD;
		break;
	case ATRAS:
		vUnidad = MinD;
		EstadoConfiguracion = HORA_RIEGO_MMD;
		break;
	case SALIR:
		SetAlarm(&AlarmTime);
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}

}

void SetHoraRiegoSSD(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig+10) < Hs24_en_Seg)
			vConfig += 10;
		else
			vConfig = Hs24_en_Seg;
		break;
	case RESTAR:
		if((vConfig+Min_en_Seg) > 0)
			vConfig -= 10;
		else
			vConfig = 0;
		break;
	case ADELANTE:
		vUnidad = SegU;
		EstadoConfiguracion = HORA_RIEGO_SSU;
		break;
	case ATRAS:
		vUnidad = MinU;
		EstadoConfiguracion = HORA_RIEGO_MMU;
		break;
	case SALIR:
		SetAlarm(&AlarmTime);
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHoraRiegoSSU(void)
{
	btnConfig = getTecla();
	switch(btnConfig)
	{
	case SUMAR:
		if((vConfig) < Hs24_en_Seg)
			vConfig++;
		break;
	case RESTAR:
		if((vConfig) > 0)
			vConfig--;
		break;
	case ADELANTE:
		SetAlarm(&AlarmTime);
		vConfig = 0;
		TimerStop(E_Potenciometro);
		Display_LCD( "Cerrando config." , RENGLON_1 , 0 );
		Display_LCD( " Ok p/continuar " , RENGLON_2 , 0 );
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	case ATRAS:
		vUnidad = SegD;
		EstadoConfiguracion = HORA_RIEGO_SSD;
		break;
	case SALIR:
		SetAlarm(&AlarmTime);
		vConfig = 0;
		TimerStop(E_Potenciometro);
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
		vUnidad = SegU;
		TimerStart(E_Potenciometro,T_Potenciometro,PrintHour,B_Potenciometro);
		Display_LCD("   Hora Alarma   ", RENGLON_1, 0);
		Display_LCD( "Hora:           ", RENGLON_2, 0);
		EstadoConfiguracion = HORA_RIEGO_SSU;
	}
	if(btnConfig == ADELANTE)
	{
		vConfig = 0;
		CloseConfiguracion();
	}
}

#endif
////////////////////////////////TEMPORIZADO////////////////////////////////
#if (1)
void PrintTimer (void)
{
	TemporizadoTime = FromGetTimer(GetTimer(E_Riego),B_Riego);

	ComponerTemporizador(&TemporizadoTime,vString);
	Display_LCD("T_Rest:",RENGLON_2,0);
	Display_LCD(vString, RENGLON_2, 8);

	SetTimer(E_Potenciometro,T_Potenciometro);
}

void PrintCurrentTime(void)
{
	TemporizadoTime = GetTime();

	ComponerTemporizador(&TemporizadoTime,vString);

	Display_LCD("Hora:   ",RENGLON_2,0);
	Display_LCD(vString, RENGLON_2, 8);

	TimerStart(E_Potenciometro,T_Print,PrintAlarm,B_Potenciometro);
}

void PrintAlarm(void)
{
	Display_LCD("Alarma: ",RENGLON_2,0);

	ComponerTemporizador(&AlarmTime,vString);

	Display_LCD(vString, RENGLON_2, 8);

	TimerStart(E_Potenciometro,T_Print,PrintStatus,B_Potenciometro);
}

void PrintStatus(void)
{
	Display_LCD("Valvula:   OFF  ", RENGLON_2,0);

	TimerStart(E_Potenciometro,T_Print,PrintCurrentTime,B_Potenciometro);
}

void AguardandoOk(void)
{
	if(btn == B_OK || Alarma() || UartOk)
	{
		UartOk = 0;
		EV_RIEGO_ON;
		Display_LCD("Modo Temporizado", RENGLON_1, 0);
		Display_LCD("T_Rest:         ", RENGLON_2, 0);
		TimerStart(E_Riego, T_Riego, VolverAguardando, B_Riego);
		TimerStart(E_Potenciometro,T_Potenciometro,PrintTimer,B_Potenciometro);
		EstadoTemporizado = RIEGO_TEMPORIZADO;
	}
}

void VolverAguardando(void)
{
	Display_LCD("Modo Temporizado" , RENGLON_1 , 0 );
	Display_LCD("  Timer:  OFF   " , RENGLON_2 , 0 );
	EstadoTemporizado = AGUARDANDO_OK;
	TimerStop(E_Potenciometro);
	TimerStart(E_Potenciometro,T_Potenciometro,PrintCurrentTime,B_Potenciometro);
	EV_RIEGO_OFF;
}

void RiegoTemporizado(void)
{

}
#endif
