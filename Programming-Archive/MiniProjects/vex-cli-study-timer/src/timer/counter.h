#ifndef COUNTER_H
#define COUNTER_H

#include <stdint.h>
#include <sys/syscall.h>
#include <time.h>

#include "timer.h"

long vext_start_time();

uint_fast8_t vext_counter_v2(timer_config_t *timer_config);
uint_fast8_t vext_counter(timer_config_t *timer_config);

#endif