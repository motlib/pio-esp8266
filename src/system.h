#pragma once

typedef struct {
    int sens_cycle_time;
} sys_config_t;

extern sys_config_t sys_config;


void system_main(void);
void system_request_reset(void);
