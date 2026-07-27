#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "timer/timer.h"
#include "cli/cli.h"

int main(int argc, char *argv[])
{
    timer_config_t timer_config = {0};

    int parse_result = parse_timer_args(argc, argv, &timer_config);

    if (parse_result == 1)
    {
        vext_controller(&timer_config);
    }
    else
    {
        /*run the error handler*/
    }

    return 0;
}
