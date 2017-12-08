/*******************************************************************************************************************************//**
 *
 * @file		Driver_display_7segm.c
 * @brief		Módulo que contiene el driver de barrido de display
 * @date		Fecha de creacion del archivo XX/XX/XXXX
 * @author		Nombre, Apellido
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_7seg.h"
#include "DR_gpio.h"
#include "DR_pinsel.h"
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
uint8_t Digito_Display[ DIGITOS ];//!< 	Digitos del display

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
 \fn  void BarridoDisplay( void )
 \brief Barrido de Display
 \param void
 \return void
 */
void BarridoDisplay( void )
{
		uint8_t auxiliar;
		static uint8_t digito = 0;

		SetPIN( digito0 , OFF );	SetPIN( digito1 , OFF );	SetPIN( digito2 , OFF );
		SetPIN( digito3 , OFF );	SetPIN( digito4 , OFF );	SetPIN( digito5 , OFF );

		auxiliar = Digito_Display[digito];

		SetPIN( segmento_a , (auxiliar & (uint8_t)0x01));
		SetPIN( segmento_b , ((auxiliar >> 1) & (uint8_t)0x01));
		SetPIN( segmento_c , ((auxiliar >> 2) & (uint8_t)0x01));
		SetPIN( segmento_d , ((auxiliar >> 3) & (uint8_t)0x01));
		SetPIN( segmento_e , ((auxiliar >> 4) & (uint8_t)0x01));
		SetPIN( segmento_f , ((auxiliar >> 5) & (uint8_t)0x01));
		SetPIN( segmento_g , ((auxiliar >> 6) & (uint8_t)0x01));
		SetPIN( segmento_dp, ((auxiliar >> 7) & (uint8_t)0x01));

		switch ( digito )
		{
			case DIGITO_5: SetPIN( digito5 , ON ); break;
			case DIGITO_4: SetPIN( digito4 , ON ); break;
			case DIGITO_3: SetPIN( digito3 , ON ); break;
			case DIGITO_2: SetPIN( digito2 , ON ); break;
			case DIGITO_1: SetPIN( digito1 , ON ); break;
			case DIGITO_0: SetPIN( digito0 , ON ); break;
		}

		digito++;
		digito %= DIGITOS;
}


void InitExp2 ( void )
{
		SetPINSEL ( segmento_a , PINSEL_GPIO );
		SetPINSEL ( segmento_b , PINSEL_GPIO );
		SetPINSEL ( segmento_c , PINSEL_GPIO );
		SetPINSEL ( segmento_d , PINSEL_GPIO );
		SetPINSEL ( segmento_e , PINSEL_GPIO );
		SetPINSEL ( segmento_f , PINSEL_GPIO );
		SetPINSEL ( segmento_g , PINSEL_GPIO );
		SetPINSEL ( segmento_dp, PINSEL_GPIO );

		SetPINSEL ( digito0 , PINSEL_GPIO );
		SetPINSEL ( digito1 , PINSEL_GPIO );
		SetPINSEL ( digito2 , PINSEL_GPIO );
		SetPINSEL ( digito3 , PINSEL_GPIO );
		SetPINSEL ( digito4 , PINSEL_GPIO );
		SetPINSEL ( digito5 , PINSEL_GPIO );

		SetDIR ( segmento_a , SALIDA );
		SetDIR ( segmento_b , SALIDA );
		SetDIR ( segmento_c , SALIDA );
		SetDIR ( segmento_d , SALIDA );
		SetDIR ( segmento_e , SALIDA );
		SetDIR ( segmento_f , SALIDA );
		SetDIR ( segmento_g , SALIDA );
		SetDIR ( segmento_dp, SALIDA );

		SetDIR ( digito0 , SALIDA );
		SetDIR ( digito1 , SALIDA );
		SetDIR ( digito2 , SALIDA );
		SetDIR ( digito3 , SALIDA );
		SetDIR ( digito4 , SALIDA );
		SetDIR ( digito5 , SALIDA );
}
