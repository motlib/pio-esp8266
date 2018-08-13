#ifndef VALFCT_H
#define VALFCT_H

#include <stdint.h>
#include <stddef.h>


typedef enum
{
    vfct_stat_ok,
    vfct_stat_err,
} vfct_status_t;


typedef union
{
    vfct_status_t (*get_f)(float * f);
    vfct_status_t (*get_u32)(uint32_t * f);
} vfct_fct_t;


typedef enum
{
    vfct_type_get_float,
    vfct_type_get_u32,
} vfct_type_t;


typedef struct
{
    vfct_type_t type;
    vfct_fct_t fct;
} vfct_t;

vfct_status_t vfct_to_string(vfct_t const * const vfct, char * buf, size_t len);



#endif /* VALFCT_H */
