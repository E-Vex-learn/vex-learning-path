#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "counter.h"
#include "alarm.h"
#include "print.h"
#include "../../sounds/tone.h"

#include "timer.h" /*to get the stucts*/

uint_fast8_t vext_validator(timer_config_t *timer_config)
{
    /* This function return 2 types of conditions
       Status A : Finish
       Status B : Break Phase */

    /*------------------------------------Break or End of all sessions----------------------------------*/
    if (timer_config->ses > timer_config->session && timer_config->min <= 0 && timer_config->sec <= 0)
    {
        return 0xA; /*Finish: end of all sessions*/
    }
    else /* Break  Phase  */
    {
        return 0xB; /*Break Phase: end of a study session*/
    }
    /*-------------------------------------------------------------------------------------------------*/
}

uint_fast8_t vext_study(timer_config_t *timer_config)
{
    timer_config->sec = 0;
    timer_config->min = timer_config->session_time;
    timer_config->validator_status = 0x0;

    timer_config->started_time.sec = vext_start_time();
    timer_config->target_end_sec = timer_config->started_time.sec + timer_config->session_time * 60;

    while (1)
    {
        if (vext_counter_v2(timer_config) == 0x0)
        {
            vext_printer(timer_config);
            break;
        }
        vext_printer(timer_config);
    }

    timer_config->ses += 1;

    if (vext_validator(timer_config) == 0xB)
    {
        vext_alarm();
        printf("\033[H\033[J");
        display_banner();
        display_info(timer_config);
    }
    return 0xAA; /*End of a study session*/
}
uint_fast8_t vext_break(timer_config_t *timer_config)
{
    timer_config->sec = 0;
    timer_config->min = timer_config->break_time;

    timer_config->started_time.sec = vext_start_time();
    timer_config->target_end_sec = timer_config->started_time.sec + timer_config->break_time * 60;

    while (1)
    {
        if (vext_counter_v2(timer_config) == 0x0)
        {
            vext_printer(timer_config);
            break;
        }
        vext_printer(timer_config);
    }

    vext_alarm();
    printf("\033[H\033[J");
    display_banner();
    display_info(timer_config);

    return 0xBB; /*End of a break*/
}

uint_fast8_t vext_input_validator(timer_config_t *timer_config)
{
    if (timer_config->session > 255 || timer_config->session_time > 255 || timer_config->break_time > 255)
    {
        return 0x1F; /*Out of range*/
    }
    return 0x0; /*Valid*/
}

void vext_controller(timer_config_t *timer_config)
{

    timer_config->ses = 1;
    timer_config->validator_status = 0x0;

    if (vext_input_validator(timer_config) == 0x1F)
    {
        printf("Error: Out of range\n");
        return;
    }

    printf("\033[H\033[J"); /*clear the screen*/

    display_banner();
    display_info(timer_config);

    while (timer_config->validator_status != 0xA)
    {
        vext_study(timer_config);

        timer_config->validator_status = vext_validator(timer_config);
        if (timer_config->validator_status == 0xB)
        {
            vext_break(timer_config);
        }
    }
    vext_printer(timer_config);
    play_finish_music();
}
