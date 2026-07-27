#include <stdio.h>
#include <stdint.h>
#include <sys/syscall.h>
#include "timer.h"

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

void vext_counter(timer_config_t *timer_config)
{
    vext_sleeper(0, 10);
    timer_config->sec -= 1;
}

uint8_t vext_validator(timer_config_t *timer_config)
{
    /*
    This function return 4 types of conditions
    Status A : end of a minute
    Status B : end of a session
    Status C : end of all sessions
    status FF : defult
    */

    /* when sec become zero decrease the minutes by 1 */
    if (timer_config->sec <= 0 && timer_config->min > 0)
    {
        timer_config->min -= 1;
        timer_config->sec = 60;
        return 0xA; /*end of a minute*/
    }
    /*------------------------------------------------*/

    /*-------when min become zero increase the session number by 1-------*/
    /*--------------return 404 at the end of the session-----------------*/
    if (timer_config->min <= 0 && timer_config->sec <= 0 && timer_config->ses < timer_config->session)
    {
        timer_config->ses += 1;
        timer_config->min = timer_config->break_time;
        return 0xB; /*end of a session*/
    }
    /*--------------------------------------------------------------------*/

    /*----------------------------------------End of all sessions--------------------------------------*/
    if (timer_config->ses >= timer_config->session && timer_config->min <= 0 && timer_config->sec <= 0)
    {
        return 0xC; /*end of all sessions*/
    }
    /*-------------------------------------------------------------------------------------------------*/

    return 0xFF;
}

uint8_t vext_break(timer_config_t *timer_config)
{
}

void vext_printer(timer_config_t *timer_config)
{
    uint8_t s, m, h;
    s = timer_config->sec;
    m = timer_config->min;
    h = 0;

    while (m >= 60)
    {
        h += 1;
        m -= 60;
    }

    if (timer_config->validator_status == 0xB)
    {
        printf("Break  %02d:%02d:%02d | session : %d of %d\n", h, m, s, timer_config->ses, timer_config->session);
    }
    else if (timer_config->validator_status == 0xC)
    {
        printf("Finish\n");
    }
    else
    {
        printf("Study  %02d:%02d:%02d | session : %d of %d\n", h, m, s, timer_config->ses, timer_config->session);
    }
}

void vext_controller(timer_config_t *timer_config)
{
    timer_config->sec = 0;
    timer_config->min = timer_config->session_time;
    timer_config->ses = 1;

    timer_config->validator_status = 0x0;

    while (timer_config->validator_status != 0xC)
    {
        vext_counter(timer_config);
        timer_config->validator_status = vext_validator(timer_config);
        vext_printer(timer_config);
    }
}
