#include <stdio.h>
#include <string.h>
#include "timer.h"

/*-----------------------------------------------------------------------------------------------------------------------------*/
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_CYAN "\033[96m"
#define RESET "\033[0m"

/* لون لكل حالة: دراسة = أخضر، استراحة = سماوي، انتهاء = أصفر (نفس لون الشعار) */
#define STUDY_COLOR BRIGHT_GREEN
#define BREAK_COLOR BRIGHT_CYAN
#define FINISH_COLOR BRIGHT_YELLOW

/* أسطر الشعار (banner) بألوان جاهزة، تجنّبًا لتكرار BRIGHT_YELLOW BOLD ... RESET في كل سطر */
#define LOGO_LINE(txt) BRIGHT_YELLOW BOLD txt RESET "\n"

#define STATUS_BREAK_RUNNING 0xB
#define STATUS_FINISHED 0xA

#define BAR_WIDTH 20

#define BOX_INNER_WIDTH 65

static void print_box_border(const char *left_corner, const char *right_corner)
{
    int i;

    printf("%s", left_corner);
    for (i = 0; i < BOX_INNER_WIDTH; i++)
        printf("═");
    printf("%s\n", right_corner);
}

static void print_box_line(const char *text)
{
    int len = (int)strlen(text);
    int pad = BOX_INNER_WIDTH - len;

    if (pad < 0)
        pad = 0;

    printf("║%s%*s║\n", text, pad, "");
}

void display_info(const timer_config_t *timer_config)
{
    printf("Sessions:  %d\n", timer_config->session);
    printf("Study   :  %d minutes\n", timer_config->session_time);
    printf("Break   :  %d minutes\n", timer_config->break_time);
    printf("Total   :  %d minutes\n\n",
           (timer_config->session_time + timer_config->break_time) * timer_config->session);
}

void display_banner(void)
{
    printf(LOGO_LINE("  ██╗   ██╗███████╗██╗  ██╗      ████████╗██╗███╗   ███╗███████╗██████╗ "));
    printf(LOGO_LINE("  ██║   ██║██╔════╝╚██╗██╔╝      ╚══██╔══╝██║████╗ ████║██╔════╝██╔══██╗"));
    printf(LOGO_LINE("  ██║   ██║█████╗   ╚███╔╝ █████╗   ██║   ██║██╔████╔██║█████╗  ██████╔╝"));
    printf(LOGO_LINE("  ╚██╗ ██╔╝██╔══╝   ██╔██╗ ╚════╝   ██║   ██║██║╚██╔╝██║██╔══╝  ██╔══██╗"));
    printf(LOGO_LINE("   ╚████╔╝ ███████╗██╔╝ ██╗         ██║   ██║██║ ╚═╝ ██║███████╗██║  ██║"));
    printf(LOGO_LINE("    ╚═══╝  ╚══════╝╚═╝  ╚═╝         ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝"));
    printf("\n");
}

void display_finish(const timer_config_t *timer_config)
{
    char sessions_line[80];
    char minutes_line[80];
    int total_minutes = timer_config->session * (timer_config->session_time + timer_config->break_time);

    snprintf(sessions_line, sizeof(sessions_line), "   Sessions completed :  %d", timer_config->session);
    snprintf(minutes_line, sizeof(minutes_line), "   Total focus time   :  %d minutes", total_minutes);

    printf("\n" FINISH_COLOR BOLD);
    print_box_border("╔", "╗");
    print_box_line("");
    print_box_line("   ★  Well done! Session complete  ★                             ");
    print_box_line("");
    print_box_line(sessions_line);
    print_box_line(minutes_line);
    print_box_line("");
    print_box_border("╚", "╝");
    printf(RESET "\n");
}

static void print_progress_bar(unsigned int remaining_seconds, unsigned int total_seconds, const char *color)
{
    unsigned int elapsed_seconds;
    unsigned int filled;
    unsigned int percent;
    unsigned int i;

    if (total_seconds == 0)
        total_seconds = 1;

    elapsed_seconds = (remaining_seconds >= total_seconds) ? 0 : (total_seconds - remaining_seconds);
    percent = (elapsed_seconds * 100) / total_seconds;
    filled = (elapsed_seconds * BAR_WIDTH) / total_seconds;

    printf("[%s", color);
    for (i = 0; i < filled; i++)
        printf("█");
    printf(RESET DIM);
    for (i = filled; i < BAR_WIDTH; i++)
        printf("░");
    printf(RESET "] %3u%%", percent);
}

void display_study_running(const timer_config_t *timer_config, unsigned int s, unsigned int m, unsigned int h,
                           unsigned int remaining_seconds, unsigned int total_seconds)
{
    printf("\r\033[2K" STUDY_COLOR BOLD "Study" RESET "  %02u:%02u:%02u | session %u/%u  ",
           h, m, s, timer_config->ses, timer_config->session);
    print_progress_bar(remaining_seconds, total_seconds, STUDY_COLOR);
    fflush(stdout);
}

void display_break_running(unsigned int s, unsigned int m, unsigned int h,
                           unsigned int remaining_seconds, unsigned int total_seconds)
{
    printf("\r\033[2K" BREAK_COLOR BOLD "Break" RESET "  %02u:%02u:%02u  ", h, m, s);
    print_progress_bar(remaining_seconds, total_seconds, BREAK_COLOR);
    fflush(stdout);
}

void vext_printer(const timer_config_t *timer_config)
{
    unsigned int s, m, h;
    unsigned int remaining_seconds, total_seconds;

    s = timer_config->sec;
    m = timer_config->min;
    remaining_seconds = (m * 60) + s;
    h = 0;

    while (m >= 60)
    {
        h += 1;
        m -= 60;
    }

    if (timer_config->validator_status == STATUS_BREAK_RUNNING)
    {
        total_seconds = (unsigned int)timer_config->break_time * 60;
        display_break_running(s, m, h, remaining_seconds, total_seconds);
    }
    else if (timer_config->validator_status == STATUS_FINISHED)
    {
        printf("\033[H\033[J"); /* clear screen */
        display_banner();
        display_finish(timer_config);
    }
    else
    {
        total_seconds = (unsigned int)timer_config->session_time * 60;
        display_study_running(timer_config, s, m, h, remaining_seconds, total_seconds);
    }
}
/*-----------------------------------------------------------------------------------------------------------------------------*/