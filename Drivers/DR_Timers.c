/*
 * DR_Timers.c
 *
 *  Created on: 6 de jul. de 2017
 *      Author: admin
 */

#include"DR_Timers.h"

extern volatile	uint32_t Tmr_Run[ N_TIMERS ];
extern volatile	uint8_t  TMR_Events[ N_TIMERS ];
extern void 	 	(* TMR_handlers [N_TIMERS]) (void);
extern volatile uint8_t  TMR_StandBy[ N_TIMERS ];
extern volatile uint8_t  Tmr_Base[ N_TIMERS ];

void TimerEvent (void)
{
    uint8_t i;

    for(i=0 ; i < N_TIMERS ; i++)
        if(TMR_Events[i])
        {
            TMR_handlers[i]();
            TMR_Events[i] = 0;
        }
}

void AnalizarTimer (void)
{
    uint8_t i;

    for(i=0 ; i< N_TIMERS ; i++)
        if(Tmr_Run[i] && !TMR_StandBy[i])
        {
            Tmr_Run[i]--;
            if(!Tmr_Run[i])
                TMR_Events[i] = 1;
        }
}
