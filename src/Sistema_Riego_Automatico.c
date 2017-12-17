/*
===============================================================================
 Name        : Sistema_Riego_Automatico.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include "PR_UART.h"
#include "PR_Leds.h"
#include "DR_Inicializacion.h"
#include "Funciones.h"
#include "MaquinasdeEstados.h"

int main(void)
{
	Inicializacion();

	while(1)
    {
		Mensaje();
		MaquinaGeneral();
		TimerEvent();
		MostrarSensores();
    }
	return 0;
}
