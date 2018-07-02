#ifndef STATEMACHINE_H
#define STATEMACHINE_H

/* integer data types */
#include <stdint.h>
/* definition of NULL */
#include <stddef.h>

typedef uint8_t sm_state_t;

typedef sm_state_t (*sm_do_fct_t)(void);
typedef void (*sm_entry_fct_t)(void);
typedef void (*sm_exit_fct_t)(void);


typedef struct {
    sm_do_fct_t do_fct;
    sm_entry_fct_t entry_fct;
    sm_exit_fct_t exit_fct;
} sm_tbl_entry_t;


typedef struct
{
    sm_state_t init_state;
    sm_tbl_entry_t *tbl;
} sm_cfg_t;
  

typedef struct
{
    sm_state_t state;
} sm_data_t;


sm_state_t sm_step(sm_cfg_t const * const cfg, sm_data_t * const sm_data);
    

#endif /* STATEMACHINE_H */
