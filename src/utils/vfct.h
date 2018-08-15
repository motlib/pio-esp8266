#ifndef VALFCT_H
#define VALFCT_H

#include <stdint.h>
#include <stddef.h>


#define VFCT_STAT_OK 0
#define VFCT_STAT_ERR 1


typedef union
{
    uint8_t (*get_f)(float * f);
    uint8_t (*get_u32)(uint32_t * f);
} vfct_fct_t;


typedef enum
{
    vfct_type_get_f,
    vfct_type_get_u32,
} vfct_type_t;


typedef struct
{
    vfct_type_t type;
    vfct_fct_t fct;
} vfct_t;

int vfct_fmt(char * const buf, size_t const buflen, vfct_t const * const vfct);
int vfct_fmt_err(char * const buf, int const buflen, uint8_t const stat);



#endif /* VALFCT_H */
