#pragma once

typedef enum
{
    diag_err_ok = 0,
    diag_err_input = 1,
    diag_err_key = 2,
    diag_err_value = 3,
    
} diag_err_t;

void diag_main();
