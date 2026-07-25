#include <stdio.h>
#include <sys/syscall.h>
#include "timer.h"

typedef struct
{
    long seconds;
    long nanoseconds;
} vex_timespec_t;

void vex_sleep(int sec, int n_sec)
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

void study_timer(timer_config_t *timer_config)
{
}