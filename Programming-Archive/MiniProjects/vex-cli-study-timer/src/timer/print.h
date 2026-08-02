#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include "timer.h"

void display_info(const timer_config_t *timer_config);
void display_banner();
void display_finish(const timer_config_t *timer_config);
void vext_printer(const timer_config_t *timer_config);

#endif