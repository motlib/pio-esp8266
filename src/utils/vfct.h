#ifndef VALFCT_H
#define VALFCT_H

#include <stdint.h>
#include <stddef.h>
/* atof */
#include <stdlib.h>

#define VFCT_STAT_OK     0
#define VFCT_STAT_ERR    1
#define VFCT_STAT_RANGE  2
#define VFCT_STAT_NO_INIT 3

typedef uint8_t vfct_result_t;


typedef union
{
    vfct_result_t (*get_f)(float * f);
    vfct_result_t (*set_f)(float * f);
    vfct_result_t (*get_u32)(uint32_t * f);
    vfct_result_t (*set_u32)(uint32_t * f);
} vfct_fct_t;


typedef enum
{
    vfct_type_get_f,
    vfct_type_get_u32,
    vfct_type_set_f,
    vfct_type_set_u32,
} vfct_type_t;


typedef struct
{
    vfct_type_t type;
    vfct_fct_t fct;
} vfct_t;

int vfct_fmt(char * const buf, size_t const buflen, vfct_t const * const vfct);
int vfct_fmt_err(char * const buf, int const buflen, uint8_t const stat);
vfct_result_t vfct_parse(vfct_t const * const vfct, char * const buf);



#endif /* VALFCT_H */
