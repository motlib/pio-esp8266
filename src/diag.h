#pragma once

typedef enum
{
    diag_err_ok = 0,
    diag_err_input,
    diag_err_input_length,
    diag_err_key,
    diag_err_value,
    
} diag_err_t;

void diag_main();
