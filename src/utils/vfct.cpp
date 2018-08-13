#include "vfct.h"

/* snprintf */
#include <stdio.h>


vfct_status_t vfct_to_string(vfct_t const * const vfct, char * buf, size_t len)
{
    vfct_status_t stat;
    switch(vfct->type)
    {
    case vfct_type_get_float:
        float val_f;
        stat = vfct->fct.get_f(&val_f);
        if(stat == vfct_stat_ok)
        {
            snprintf(buf, len, "%f", val_f);
        }
        break;

    case vfct_type_get_u32:
        uint32_t val_u32;
        stat = vfct->fct.get_u32(&val_u32);
        if(stat == vfct_stat_ok)
        {
            snprintf(buf, len, "%u", val_u32);
        }
        break;

    default:
        buf[0] = 0;
        return vfct_stat_err;
        break;
    }

    return vfct_stat_ok;
    
}

