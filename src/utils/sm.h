#ifndef STATEMACHINE_H
#define STATEMACHINE_H

/* integer data types */
#include <stdint.h>
/* definition of NULL */
#include <stddef.h>


/* Macro to calculate the number of states in the state table. */
#define SM_NUM_STATES(tbl) (sizeof(tbl) / sizeof(tbl[0]))


#define SM_TBL_ENTRY(DO, ENTRY, EXIT) \
    { .do_fct = (DO), .entry_fct = (ENTRY), .exit_fct = (EXIT) }

/* Helper to initialize a statemachine config structure. */
#define SM_DEF_CFG(INIT_STATE, TBL)             \
    {                                           \
        .init_state = INIT_STATE,               \
        .num_states = SM_NUM_STATES(TBL),       \
        .tbl = TBL,                             \
    }

/** Helper to initialize the statemachine data structure. */
#define SM_DEF_DATA()                           \
    {                                           \
        .state = 0,                             \
        .needs_init = 1,                        \
    }


/**
 * The type to hold a state id of the state machine (e.g. active state, etc.).
 *
 * Currently implemented as uint8_t, so up to 255 states are possible.
 */
typedef uint8_t sm_state_t;

/** Function pointer for state implementation (do function). */
typedef sm_state_t (*sm_do_fct_t)(void);
/** Function pointer for state entry implementation (entry function). */
typedef void (*sm_entry_fct_t)(void);
/** Function pointer for state exit implementation (exit function). */
typedef void (*sm_exit_fct_t)(void);


/**
 * State table entry type. Holds the function pointers for do, entry and exit
 * function of one statemachine state.
 */
typedef struct {
    /* State handler implementation. */
    sm_do_fct_t do_fct;
    
    /* State entry handler function or NULL. */
    sm_entry_fct_t entry_fct;
    
    /** State exit handler function or NULL. */
    sm_exit_fct_t exit_fct;
} sm_tbl_entry_t;


/**
 * Statemachone configuration type.
 *
 * Use SM_DEF_CFG(INIT_STATE, TBL)  macro to initialize.
 */
typedef struct
{
    /** Initial state of the statemachine. */
    sm_state_t init_state;

    /** Number of states in the state table. Should be assigned by using the
     * SM_NUM_STATES macro. */
    sm_state_t num_states;
    
    /** State table, containing all states of the statemachine and their handler
     * functions. */
    sm_tbl_entry_t *tbl;
} sm_cfg_t;
  

/**
 * Statemachine runtime data type.
 */
typedef struct
{
    /** Current state of the statemachine. */
    sm_state_t state;
    
    uint8_t needs_init;
} sm_data_t;


/**
 * Execute a single step of the statemachine.
 *
 * @param sm_cfg Pointer to the statemachine configuration.
 * @param sm_data Pointer to the statemachine runtime data.
 */
void sm_step(sm_cfg_t const * const cm_cfg, sm_data_t * const sm_data);

#endif /* STATEMACHINE_H */
