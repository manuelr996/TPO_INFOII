/*
 * MaquinasDeEstados.c
 *
 *  Created on: 4 de dic. de 2017
 *      Author: alumno
 */
#include "MaquinasdeEstados.h"

EstadosConfiguracion EstadoConfiguracion;
EstadosManual EstadoManual;
EstadosTemporizado EstadoTemporizado;
EstadosAutomatico EstadoAutomatico;
EstadosGenerales Estado;

void (*MaquinaConfiguracion[])(void) = {};
void (*MaquinaManual[])(void) = {};
void (*MaquinaTemporizado[])(void) = {};
void (*MaquinaAutomatico[])(void) = {};

void MaquinaGeneral (void)
{
	uint8_t btn = NO_KEY;

	btn = getTecla();

	switch(Estado)
	{
	case RESET_G:
		Estado = MANUAL;
	case CONFIGURACION:
		//MaquinaConfiguracion[EstadoConfiguracion];
		break;
	case MANUAL:
		//MaquinaManual[EstadoManual];
		break;
	case TEMPORIZADO:
		//MaquinaConfiguracion[EstadoConfiguracion];
		break;
	case AUTOMATICO:
		//MaquinaAutomatico[EstadoAutomatico];
		break;
	default:
		Estado = RESET_G;
		break;
	}

	switch(btn)
	{
	case B_AUTOMATICO:
		ApagarLeds();
		PrenderLed(VERDE);
		Estado = AUTOMATICO;
		btn = NO_KEY;
		break;
	case B_CONFIGURACION:
		ApagarLeds();
		PrenderLed(ROJO);
		Estado = CONFIGURACION;
		btn = NO_KEY;
		break;
	case B_MANUAL:
		ApagarLeds();
		PrenderLed(AZUL);
		//to-do: InitTemporizado();
		Estado = MANUAL;
		btn = NO_KEY;
		break;
	case B_TEMPORIZADO:
		ApagarLeds();
		PrenderLed(ROJO);
		PrenderLed(VERDE);
		//to-do: InitTemporizado();
		Estado = TEMPORIZADO;
		btn = NO_KEY;
		break;
	}
}
