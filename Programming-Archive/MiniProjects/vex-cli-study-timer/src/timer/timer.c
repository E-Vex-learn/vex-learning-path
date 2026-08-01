#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include "alarm.h"
#include "print.h"

#include "timer.h" /*to get the stucts*/

typedef struct
{
    long seconds;
    long nanoseconds;
} vex_timespec_t;

void vext_sleeper(int sec, int n_sec)
{

    vex_timespec_t time_to_sleep = {sec, n_sec};

    register long syscall_number __asm__("rax") = SYS_nanosleep;
    register vex_timespec_t *rdi __asm__("rdi") = &time_to_sleep;
    register void *rsi __asm__("rsi") = NULL;

    __asm__ volatile(
        "syscall"
        :
        : "r"(syscall_number), "r"(rdi), "r"(rsi)
        : "rcx", "r11", "memory");
}
long vext_start_time()
{
    vex_timespec_t t = {0, 0};

    register long syscall_number __asm__("rax") = SYS_clock_gettime;
    register long rdi __asm__("rdi") = CLOCK_MONOTONIC;
    register vex_timespec_t *rsi __asm__("rsi") = &t;

    __asm__ volatile(
        "syscall"
        :
        : "r"(syscall_number), "r"(rdi), "r"(rsi)
        : "rcx", "r11", "memory");

    return t.seconds;
}

uint_fast8_t vext_counter(timer_config_t *timer_config)
{
    vext_sleeper(1, 0);

    /* decrease one second if the sec is greater than 00 */
    if (timer_config->sec > 0)
    {
        timer_config->sec -= 1;
    }
    /*---------------------------------------------------*/

    /* when sec become zero and the min is greater than 00, decrease the minutes by 1 */
    if (timer_config->sec <= 0 && timer_config->min > 0)
    {
        timer_config->min -= 1;
        timer_config->sec = 60;
        return 0x01; /*End of a minute*/
    }
    /*--------------------------------------------------------------------------------*/

    /* end of counting operation when sec and min are both 0 */
    if (timer_config->sec <= 0 && timer_config->min <= 0)
    {
        return 0x0; /*End of the timer*/
    }
    /*-------------------------------------------------------*/

    return 0xFF; /*Time is working*/
}
uint_fast8_t vext_counter_v2(timer_config_t *timer_config)
{
    timer_config->time_now.sec = vext_start_time();

    unsigned int remaining = timer_config->target_end_sec - timer_config->time_now.sec;
    if (remaining < 0)
    {
        remaining = 0;
    }

    timer_config->min = remaining / 60;
    timer_config->sec = remaining % 60;

    /* end of counting operation */
    if (remaining <= 0)
    {
        return 0x0; /*End of the timer*/
    }

    vext_sleeper(1, 0); /*the sleep here dose not effect on the timer. Just for the display*/

    return 0xFF; /*Time is working*/
}

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

    while (vext_counter_v2(timer_config) != 0x0)
    {
        vext_printer(timer_config);
    }

    timer_config->ses += 1;

    vext_alarm();

    return 0xAA; /*End of a study session*/
}
uint_fast8_t vext_break(timer_config_t *timer_config)
{
    timer_config->sec = 0;
    timer_config->min = timer_config->break_time;

    timer_config->started_time.sec = vext_start_time();
    timer_config->target_end_sec = timer_config->started_time.sec + timer_config->break_time * 60;

    while (vext_counter_v2(timer_config) != 0)
    {
        vext_printer(timer_config);
    }

    vext_alarm();

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

    display_start_screen(timer_config);

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
}
