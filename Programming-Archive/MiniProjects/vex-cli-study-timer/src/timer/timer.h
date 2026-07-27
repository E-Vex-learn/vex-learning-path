#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdint.h>
#include <sys/syscall.h>

typedef struct
{
    int session;      /* total number of study sessions */
    int session_time; /* duration of each session, in minutes */
    int break_time;   /* duration of break between sessions, in minutes */
    /*-------------------------------------------------------------------------------*/
    int sec;
    int min;
    int ses;

    uint8_t validator_status;

} timer_config_t;

void vext_controller(timer_config_t *timer_config);

#endif