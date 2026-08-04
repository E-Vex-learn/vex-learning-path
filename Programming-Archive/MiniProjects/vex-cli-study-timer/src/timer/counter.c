#define _POSIX_C_SOURCE 200809L

#include <stdint.h>
#include <sys/syscall.h>
#include <time.h>

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The old counter (v1) relied on the program's own sleep to advance                                         //
// time, so drift accumulated from per-tick overhead (syscalls,                                              //
// printf, alarms) made it inaccurate over long sessions.                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Counter v2 reads the kernel's monotonic clock directly instead,                                           //
// so it stays accurate regardless of per-tick overhead.                                                     //
// Note: vext_sleeper() here only paces the display refresh rate;                                            //
// it has no effect on the actual remaining time calculation.                                                //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint_fast8_t vext_counter_v2(timer_config_t *timer_config)
{
    timer_config->time_now.sec = vext_start_time();

    uint32_t remaining;
    if (timer_config->time_now.sec >= timer_config->target_end_sec) /* Prevent negative remaining time */
    {
        remaining = 0;
    }
    else
    {
        remaining = timer_config->target_end_sec - timer_config->time_now.sec;
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////