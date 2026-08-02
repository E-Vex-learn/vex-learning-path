#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include "alarm.h"
#include "../../sounds/tone.h"

typedef struct
{
    unsigned int sec;
    unsigned int min;
    unsigned int remaining_sec;

} started_time_t;

typedef struct
{
    unsigned int sec;
    unsigned int min;
    unsigned int remaining_sec;

} time_now_t;

typedef struct
{
    unsigned int session;      /* total number of study sessions */
    unsigned int session_time; /* duration of each session, in minutes */
    unsigned int break_time;   /* duration of break between sessions, in minutes */
    /*-------------------------------------------------------------------------------*/
    unsigned int sec;
    unsigned int min;
    unsigned int ses;

    started_time_t started_time;
    time_now_t time_now;

    unsigned int target_end_sec; /* absolute monotonic time at which the current phase (study/break) must end */

    uint8_t validator_status;

} timer_config_t;

void vext_controller(timer_config_t *timer_config);

#include "print.h"
#endif