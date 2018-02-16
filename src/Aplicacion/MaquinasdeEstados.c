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
RTC_t aux;
/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

uint8_t btnConfig; //lectura del boton en modo configuracion
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

void ConfiguracionInicializada (void)
{
	btnConfig = getTecla();

	if(btnConfig == ADELANTE)
	{
		vUnidad = Decena;
		vConfig = config.humMax;
		PrintConfigHumMax();
		EstadoConfiguracion = HUMEDADMAXIMA_D;
	}
	else if( btnConfig == SALIR )
	{
		PrintCerrarConfig();
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
			PrintCerrarConfig();
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

		PrintConfigHumMin();
		EstadoConfiguracion = HUMEDADMINIMA_D;
		break;
    case ATRAS:
    	vUnidad = Decena;
    	EstadoConfiguracion = HUMEDADMAXIMA_D;
    	break;
	case SALIR:
		config.humMax = vConfig;

		PrintCerrarConfig();
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

		PrintConfigHumMax();
    	EstadoConfiguracion = HUMEDADMAXIMA_U;
		break;
	case SALIR:
		config.humMax = vConfig;

		PrintCerrarConfig();
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

		PrintTempDeRiego();
		EstadoConfiguracion = TEMPORIZADOR_HHD;
    	break;
    case ATRAS:
		vUnidad = Decena; //editamos decenas
		EstadoConfiguracion = HUMEDADMINIMA_D;
		break;
	case SALIR:
		config.humMin = vConfig;

		PrintCerrarConfig();
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

		PrintConfigHumMin();
		TimerStart(E_Print,T_Print,PrintHumedad,B_Print);

		EstadoConfiguracion = HUMEDADMINIMA_U;
		break;
	case SALIR:
		config.vTempo = vConfig;

		PrintCerrarConfig();
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
		//TimerStop(E_Print);

		PrintPrenderAlarma();
		EstadoConfiguracion = ALARMA_ON_OFF;
		break;
	case ATRAS:
		vUnidad = MinD;
		EstadoConfiguracion = TEMPORIZADOR_MMD;
		break;
	case SALIR:
		config.vTempo = vConfig;

		PrintCerrarConfig();
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

		config.estAlrm = vConfig;

		if( vConfig )
		{
			vConfig = config.vAlarm;
			vUnidad = HrD;

			PrintConfigAlarma();
			EstadoConfiguracion = HORA_RIEGO_HHD;
		}
		else
		{
			aux = GetTime();
			vConfig = ToTimer(&aux, MIN);
			vUnidad = HrD;

			PrintConfigHora();
			EstadoConfiguracion = HORA_HHD;
		}
		break;
	case ATRAS:
		config.estAlrm = vConfig;
		vConfig = config.vTempo;
		vUnidad = MinU;

		PrintTempDeRiego();
		EstadoConfiguracion = TEMPORIZADOR_MMU;
		break;
	case SALIR:
		config.estAlrm = vConfig;

		PrintCerrarConfig();
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

		PrintPrenderAlarma();
		EstadoConfiguracion = ALARMA_ON_OFF;
		break;
	case SALIR:
		config.vAlarm = vConfig;
		SetAlarm(FromGetTimer(config.vAlarm, MIN));

		PrintCerrarConfig();
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

		PrintCerrarConfig();
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

		PrintCerrarConfig();
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
		aux = GetTime();
		vConfig = ToTimer(&aux, MIN);
		vUnidad = HrD;

		PrintConfigHora();
		EstadoConfiguracion = HORA_HHD;
		break;
	case ATRAS:
		vUnidad = MinD;
		EstadoConfiguracion = HORA_RIEGO_MMD;
		break;
	case SALIR:
		config.vAlarm = vConfig;
		SetAlarm(FromGetTimer(config.vAlarm, MIN));

		PrintCerrarConfig();
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}

}

void SetHoraHHD(void)
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
		EstadoConfiguracion = HORA_HHU;
		break;
	case ATRAS:

		SetTime(FromGetTimer(vConfig, MIN));

		if(config.estAlrm)
		{
			vUnidad = MinU;
			vConfig = config.vAlarm;

			PrintConfigAlarma();
			EstadoConfiguracion = HORA_RIEGO_MMU;
		}
		else
		{
			vConfig = config.estAlrm;

			PrintPrenderAlarma();
			EstadoConfiguracion = ALARMA_ON_OFF;
		}
		break;
	case SALIR:
		SetTime(FromGetTimer(vConfig, MIN));
		PrintCerrarConfig();
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHoraHHU(void)
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
		EstadoConfiguracion = HORA_MMD;
		break;
	case ATRAS:
		vUnidad = HrD;
		EstadoConfiguracion = HORA_HHD;
		break;
	case SALIR:
		SetTime(FromGetTimer(vConfig, MIN));
		PrintCerrarConfig();
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHoraMMD(void)
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
		EstadoConfiguracion = HORA_MMU;
		break;
	case ATRAS:
		vUnidad = HrU;
		EstadoConfiguracion = HORA_HHU;
		break;
	case SALIR:
		SetTime(FromGetTimer(vConfig, MIN));
		PrintCerrarConfig();
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void SetHoraMMU(void)
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
		SetTime(FromGetTimer(vConfig, MIN));

		PrintCerrarConfig();
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	case ATRAS:
		vUnidad = MinD;
		EstadoConfiguracion = HORA_MMD;
		break;
	case SALIR:
		SetTime(FromGetTimer(vConfig, MIN));
		PrintCerrarConfig();
		EstadoConfiguracion = CERRAR_CONFIGURACION;
		break;
	}
}

void ConfiguracionFinalizada (void)
{
	btnConfig = getTecla();
	if(btnConfig == ATRAS)
	{
		aux = GetTime();
		vConfig = ToTimer(&aux, MIN);
		vUnidad = HrD;

		PrintConfigHora();
		EstadoConfiguracion = HORA_MMU;
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

void AguardandoOk(void)
{
	if((btn == B_OK || Alarma() || UartOk))
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

