/**
 * @file
 * 
 * Generic implementation of a simple finite statemachine handler. 
 */

#include <stddef.h>

#include "sm.h"
#include "det.h"

/**
 * Execute one step of the statemachine defined by sm_cfg and sm_data.
 */
void sm_step(sm_cfg_t const * const sm_cfg, sm_data_t * const sm_data)
{
    sm_do_fct_t do_fct;
    sm_state_t new_state;

    /* re-initialize the statemachine to the init state when needed. */
    if(sm_data->needs_init)
    {
        sm_data->state = sm_cfg->init_state;
        sm_data->needs_init = 0;
    }
    
    /* Assert here, to be sure, state was not modified externally to the
     * statemachine implementation. */
    DET_ASSERT(sm_data->state < sm_cfg->num_states);
    
    /* Get and execute the 'do' function of the current state. */
    do_fct = sm_cfg->tbl[sm_data->state].do_fct;
    new_state = do_fct();

    /* Assert, that the new state will be in range. */
    DET_ASSERT(new_state < sm_cfg->num_states);
    
    /* Did we do a transition to a new state? */
    if(new_state != sm_data->state)
    {
        /* Run exit function (if it exists) of last state. */
        sm_exit_fct_t exit_fct = sm_cfg->tbl[sm_data->state].exit_fct;
        if(exit_fct != NULL)
        {
            exit_fct();
        }

        /* Run entry function (if it exists) of new state. */
        sm_entry_fct_t entry_fct = sm_cfg->tbl[new_state].entry_fct;
        if(entry_fct != NULL)
        {
            entry_fct();
        }
    }

    sm_data->state = new_state;
}
