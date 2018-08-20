#include "vfct.h"

/* snprintf */
#include <stdio.h>

static char const * const vfct_fmt_f = "%.2f";
static char const * const vfct_fmt_u32 = "%u";

/* Format error codes */
int vfct_fmt_err(char * const buf, int const buflen, uint8_t const stat)
{
    return snprintf(buf, buflen, "E:%i", stat);
}

/* Parse value from string */
vfct_result_t vfct_parse(vfct_t const * const vfct, char * const buf)
{
    uint8_t stat;

    switch(vfct->type)
    {
    case vfct_type_set_f:
        float val_f;

        val_f = atof(buf);
        stat = vfct->fct.set_f(&val_f);
        
        break;

    case vfct_type_set_u32:
        uint32_t val_u32;

        /* TODO: This is not exactly correct... */
        val_u32 = (uint32_t)atoi(buf);
        
        stat = vfct->fct.set_u32(&val_u32);
        
        break;

    default:
        stat = VFCT_STAT_ERR;
        break;
    }        

    return stat;
}


/* Convert value to string. */
int vfct_fmt(char * const buf, size_t const buflen, vfct_t const * const vfct)
{
    int len = 0;
    uint8_t stat;
    
    switch(vfct->type)
    {
    case vfct_type_get_f:
        float val_f;

        stat = vfct->fct.get_f(&val_f);
        if(stat == VFCT_STAT_OK)
        {
            len = snprintf(buf, buflen, vfct_fmt_f, val_f);
        }
        
        break;

    case vfct_type_get_u32:
        uint32_t val_u32;

        stat = vfct->fct.get_u32(&val_u32);
        if(stat == VFCT_STAT_OK)
        {
            len = snprintf(buf, buflen, vfct_fmt_u32, val_u32);
        }
        
        break;

    default:
        stat = VFCT_STAT_ERR;
        break;
    }

    if(stat != VFCT_STAT_OK)
    {
        len = vfct_fmt_err(buf, buflen, stat);
    }    

    return len;
}

