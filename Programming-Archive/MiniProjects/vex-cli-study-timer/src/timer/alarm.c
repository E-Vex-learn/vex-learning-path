#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include "../../sounds/tone.h"

extern int stop_playback;

void vext_alarm(void)
{
    stop_playback = 0;
    printf("\nAlarm ringing! Press [ENTER] to stop...\n");
    play_megalovania();
}
