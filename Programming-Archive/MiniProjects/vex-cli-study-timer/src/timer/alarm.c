#include <stdio.h>
#include <stdlib.h>

void vext_alarm(void)
{
    system("aplay -q /home/elia/Documents/vex-learning-archive/Programming-Archive/MiniProjects/vex-cli-study-timer/sounds/Death-Note-Alarm.wav &");
    printf("End of the session");

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    system("pkill aplay > /dev/null 2>&1");
}
