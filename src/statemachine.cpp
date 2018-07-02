#include "statemachine.h"
#include <stddef.h>

sm_state_t sm_step(sm_cfg_t const * const cfg, sm_data_t * const sm_data)
{
    sm_do_fct_t do_fct;
    sm_entry_fct_t entry_fct;
    sm_exit_fct_t exit_fct;
    sm_state_t new_state;
    
        
    do_fct = cfg->tbl[sm_data->state].do_fct;

    new_state = do_fct();

    if(new_state != sm_data->state)
    {
        exit_fct = cfg->tbl[sm_data->state].exit_fct;
        if(exit_fct != NULL)
        {
            exit_fct();
        }

        entry_fct = cfg->tbl[new_state].entry_fct;
        if(entry_fct != NULL)
        {
            entry_fct();
        }
    }

    sm_data->state = new_state;

    return sm_data->state;
}
