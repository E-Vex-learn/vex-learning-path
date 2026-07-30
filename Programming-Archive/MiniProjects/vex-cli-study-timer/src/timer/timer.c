#include <stdio.h>
#include <stdint.h>
#include <sys/syscall.h>
#include "timer.h"

typedef struct
{
    long seconds;
    long nanoseconds;
} vex_timespec_t;

#define BOLD "\033[1m"
#define BRIGHT_YELLOW "\033[93m"
#define RESET "\033[0m"

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

/*-----------------------------------------------------------------------------------------------------------------------------*/
void display_start_screen(timer_config_t *timer_config)
{
    printf(BRIGHT_YELLOW BOLD "  ██╗   ██╗███████╗██╗  ██╗      ████████╗██╗███╗   ███╗███████╗██████╗ " RESET "\n");
    printf(BRIGHT_YELLOW BOLD "  ██║   ██║██╔════╝╚██╗██╔╝      ╚══██╔══╝██║████╗ ████║██╔════╝██╔══██╗" RESET "\n");
    printf(BRIGHT_YELLOW BOLD "  ██║   ██║█████╗   ╚███╔╝ █████╗   ██║   ██║██╔████╔██║█████╗  ██████╔╝" RESET "\n");
    printf(BRIGHT_YELLOW BOLD "  ╚██╗ ██╔╝██╔══╝   ██╔██╗ ╚════╝   ██║   ██║██║╚██╔╝██║██╔══╝  ██╔══██╗" RESET "\n");
    printf(BRIGHT_YELLOW BOLD "   ╚████╔╝ ███████╗██╔╝ ██╗         ██║   ██║██║ ╚═╝ ██║███████╗██║  ██║" RESET "\n");
    printf(BRIGHT_YELLOW BOLD "    ╚═══╝  ╚══════╝╚═╝  ╚═╝         ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝" RESET "\n");

    printf("\n");

    printf("Sessions:  %d\n", timer_config->session);
    printf("Study   :  %d minutes\n", timer_config->session_time);
    printf("Break   :  %d minutes\n", timer_config->break_time);
    printf("Total   :  %d minutes\n", ((timer_config->session_time + timer_config->break_time) * (timer_config->session)));
}
void vext_printer(timer_config_t *timer_config)
{
    unsigned int s, m, h;
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
        printf("Break  %02d:%02d:%02d\n", h, m, s);
    }
    else if (timer_config->validator_status == 0xA)
    {
        printf("Finish\n");
    }
    else
    {
        printf("Study  %02d:%02d:%02d | session : %d of %d\n", h, m, s, timer_config->ses, timer_config->session);
    }
}
/*-----------------------------------------------------------------------------------------------------------------------------*/

uint_fast8_t vext_study(timer_config_t *timer_config)
{
    timer_config->sec = 0;
    timer_config->min = timer_config->session_time;
    timer_config->validator_status = 0x0;

    while (vext_counter(timer_config) != 0x0)
    {
        vext_printer(timer_config);
    }

    timer_config->ses += 1;
    return 0x0; /*End of a study session*/
}
uint_fast8_t vext_break(timer_config_t *timer_config)
{
    timer_config->sec = 0;
    timer_config->min = timer_config->break_time;

    while (vext_counter(timer_config) != 0)
    {
        vext_printer(timer_config);
    }

    return 0x0; /*End of a break*/
}

void vext_controller(timer_config_t *timer_config)
{

    timer_config->ses = 1;
    timer_config->validator_status = 0x0;

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
