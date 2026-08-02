#include <stdio.h>
#include "timer.h"

/*-----------------------------------------------------------------------------------------------------------------------------*/
#define BOLD "\033[1m"
#define BRIGHT_YELLOW "\033[93m"
#define RESET "\033[0m"

void display_info(timer_config_t *timer_config)
{
    printf("Sessions:  %d\n", timer_config->session);
    printf("Study   :  %d minutes\n", timer_config->session_time);
    printf("Break   :  %d minutes\n", timer_config->break_time);
    printf("Total   :  %d minutes\n\n", ((timer_config->session_time + timer_config->break_time) * (timer_config->session)));
}
void display_banner()
{
    printf(BRIGHT_YELLOW BOLD "  ██╗   ██╗███████╗██╗  ██╗      ████████╗██╗███╗   ███╗███████╗██████╗ " RESET "\n");
    printf(BRIGHT_YELLOW BOLD "  ██║   ██║██╔════╝╚██╗██╔╝      ╚══██╔══╝██║████╗ ████║██╔════╝██╔══██╗" RESET "\n");
    printf(BRIGHT_YELLOW BOLD "  ██║   ██║█████╗   ╚███╔╝ █████╗   ██║   ██║██╔████╔██║█████╗  ██████╔╝" RESET "\n");
    printf(BRIGHT_YELLOW BOLD "  ╚██╗ ██╔╝██╔══╝   ██╔██╗ ╚════╝   ██║   ██║██║╚██╔╝██║██╔══╝  ██╔══██╗" RESET "\n");
    printf(BRIGHT_YELLOW BOLD "   ╚████╔╝ ███████╗██╔╝ ██╗         ██║   ██║██║ ╚═╝ ██║███████╗██║  ██║" RESET "\n");
    printf(BRIGHT_YELLOW BOLD "    ╚═══╝  ╚══════╝╚═╝  ╚═╝         ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝" RESET "\n");

    printf("\n");
}
void display_finish(timer_config_t *timer_config)
{
    printf(BRIGHT_YELLOW BOLD "\n███████████████████████████████████████████████████████████████████" RESET "\n");
    printf("%d sessions, %d minutes of deep working\n", timer_config->session, (timer_config->session * (timer_config->session_time + timer_config->break_time)));
}
void diplay_study_runing(timer_config_t *timer_config, unsigned int s, unsigned int m, unsigned int h)
{
    printf("\r\e[2KStudy  %02u:%02u:%02u | session : %u of %u", h, m, s, timer_config->ses, timer_config->session);
    fflush(stdout);
}
void diplay_break_runing(timer_config_t *timer_config, unsigned int s, unsigned int m, unsigned int h)
{
    printf("\r\e[2KBreak  %02u:%02u:%02u", h, m, s);
    fflush(stdout);
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

    if (timer_config->validator_status == 0xB) /*break runing*/
    {
        diplay_break_runing(timer_config, s, m, h);
    }
    else if (timer_config->validator_status == 0xA) /*finish*/
    {
        printf("\033[H\033[J"); /*clear screen*/
        display_banner();
        display_finish(timer_config);
    }
    else /*study runing*/
    {
        diplay_study_runing(timer_config, s, m, h);
    }
}
/*-----------------------------------------------------------------------------------------------------------------------------*/
