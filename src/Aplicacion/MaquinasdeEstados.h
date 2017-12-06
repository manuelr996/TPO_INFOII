/*
 * MaquinasdeEstados.h
 *
 *  Created on: 4 de dic. de 2017
 *      Author: alumno
 */

#ifndef APLICACION_MAQUINASDEESTADOS_H_
#define APLICACION_MAQUINASDEESTADOS_H_

#include "PR_SoftTimers.h"
#include "DR_tipos.h"
#include "PR_Boton.h"
#include "PR_Leds.h"


typedef enum
{
	MANUAL = 0,
	CONFIGURACION,
	TEMPORIZADO,
	AUTOMATICO,
	RESET_G
}EstadosGenerales;

typedef enum
{
	AGUARDANDO_OK = 0,
	RIEGO_TEMPORIZADO
}EstadosTemporizado;

typedef enum
{
	RIEGO_OFF = 0,
	RIEGO_ON
}EstadosManual;

typedef enum
{
	REGANDO = 0,
	NO_REGANDO
}EstadosAutomatico;

typedef enum
{
	INIT_CONFIGURACION = 0,
	HUMEDADMINIMA,
	HUMEDADMAXIMA,
	TIEMPOTEMPORIZADOR,
	CERRAR_CONFIGURACION
}EstadosConfiguracion;

extern EstadosGenerales Estado;

void MaquinaGeneral (void);

#endif /* APLICACION_MAQUINASDEESTADOS_H_ */

