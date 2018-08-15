#include "vfct.h"

/* snprintf */
#include <stdio.h>


int vfct_fmt_err(char * const buf, int const buflen, uint8_t const stat)
{
    return snprintf(buf, buflen, "E:%i", stat);
}


static char const * const vfct_fmt_f = "%.2f";
static char const * const vfct_fmt_u32 = "%u";


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

