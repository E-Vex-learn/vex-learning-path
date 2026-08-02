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
    uint32_t sec;
    uint32_t min;
    uint32_t remaining_sec;

} started_time_t;

typedef struct
{
    uint32_t sec;
    uint32_t min;
    uint32_t remaining_sec;

} time_now_t;

typedef struct
{
    uint32_t session;      /* total number of study sessions */
    uint32_t session_time; /* duration of each session, in minutes */
    uint32_t break_time;   /* duration of break between sessions, in minutes */
    /*-------------------------------------------------------------------------------*/
    uint32_t sec;
    uint32_t min;
    uint32_t ses;

    started_time_t started_time;
    time_now_t time_now;

    uint32_t target_end_sec; /* absolute monotonic time at which the current phase (study/break) must end */

    uint8_t validator_status;

} timer_config_t;

void vext_controller(timer_config_t *timer_config);

#include "print.h"
#endif