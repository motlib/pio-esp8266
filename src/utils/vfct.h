#ifndef VFCT_H
#define VFCT_H


#include <stdint.h>
#include <stddef.h>
/* atof */
#include <stdlib.h>


#define VFCT_ERR_OK     0
#define VFCT_ERR_ERR    1
#define VFCT_ERR_RANGE  2
#define VFCT_ERR_NO_INIT 3
#define VFCT_ERR_NO_OP   4


typedef uint8_t vfct_err_t;


typedef union
{
    vfct_err_t (*get_float)(float * const f);
    vfct_err_t (*set_float)(float * const f);
    vfct_err_t (*get_u32)(uint32_t * const u);
    vfct_err_t (*set_u32)(uint32_t * const u);
    vfct_err_t (*get_string)(char const ** const s);
    vfct_err_t (*set_string)(char const * const s);
} vfct_fct_t;


typedef enum
{
    vfct_type_float,
    vfct_type_u32,
    vfct_type_string,
} vfct_type_t;


typedef struct
{
    vfct_type_t type;
    vfct_fct_t get_fct;    
    vfct_fct_t set_fct;
} vfct_t;


/**
 * Helper macro to generate a vfct_t definition.
 *
 * @param TYPE The function type, one of get_f, get_u32, set_f, set_u32.
 * @param FUNC The function pointer to the get / set function.
 */
#define VFCT_DEF(TYPE,GET_FCT,SET_FCT)          \
    {                                           \
        .type = vfct_type_##TYPE,               \
        .get_fct = { .get_##TYPE = GET_FCT },         \
        .set_fct = { .set_##TYPE = SET_FCT },         \
    }

/**
 * Format a value, i.e. convert to a string.
 *
 * @param[out] buf The buffer to which the string is written.
 * @param[in] buflen The size of buf.
 * @param[in] The pointer to the vfct structure to confert to string.
 *
 * @return The number of bytes written to the buffer.
 */
int vfct_fmt(char * const buf, size_t const buflen, vfct_t const * const vfct);


/**
 * Format a error code returned from a vfct function, i.e. convert to a string.
 *
 * @param[out] buf The buffer to which the string is written.
 * @param[in] buflen The size of buf.
 * @param[in] The error code to convert to string.
 *
 * @return The number of bytes written to the buffer.
 */
int vfct_fmt_err(char * const buf, int const buflen, vfct_err_t const stat);


/**
 * Convert a string to a vfct value.
 *
 * @param[inout] vfct The vfct structure to write the converted value to.
 * @param[in] buf The buffer from which to read the string to convert. The string must be \0-terminated.
 *
 * @return The error status of the conversion.
 */
vfct_err_t vfct_parse(vfct_t const * const vfct, char const * const buf);


#endif /* VFCT_H */
