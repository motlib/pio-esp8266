#include "vfct.h"

/* snprintf */
#include <stdio.h>

static char const * const vfct_fmt_f = "%.2f";
static char const * const vfct_fmt_u32 = "%u";
static char const * const vfct_fmt_s = "%s";


/* Format error codes */
int vfct_fmt_err(char * const buf, int const buflen, uint8_t const stat)
{
    return snprintf(buf, buflen, "E:%i", stat);
}


/* Parse value from string */
vfct_err_t vfct_parse(vfct_t const * const vfct, char const * const buf)
{
    uint8_t stat;

    /* Can check any member for NULL, as this is a pointer union. Return error
     * if no set-function is given. */
    if(vfct->set_fct.set_float == NULL)
    {
        return VFCT_ERR_NO_OP;
    }
    
    switch(vfct->type)
    {
        
    case vfct_type_float:
    {
        float val_f;

        val_f = atof(buf);
        stat = vfct->set_fct.set_float(&val_f);
        
        break;
    }
    
    case vfct_type_u32:
    {
        uint32_t val_u32;

        /* TODO: This is not exactly correct... */
        val_u32 = (uint32_t)atoi(buf);
        
        stat = vfct->set_fct.set_u32(&val_u32);
        
        break;
    }
    
    case vfct_type_string:
    {
        stat = vfct->set_fct.set_string(buf);
        break;
    }
        
    default:
        stat = VFCT_ERR_ERR;
        break;
    }        

    return stat;
}


/* Convert value to string. */
int vfct_fmt(char * const buf, size_t const buflen, vfct_t const * const vfct, char const * fmt)
{
    int len = 0;
    uint8_t stat;
    
    switch(vfct->type)
    {
    case vfct_type_float:
        float val_f;

        stat = vfct->get_fct.get_float(&val_f);
        if(stat == VFCT_ERR_OK)
        {
            if(fmt == NULL)
            {
                fmt = vfct_fmt_f;
            }
            len = snprintf(buf, buflen, fmt, val_f);
        }
        
        break;

    case vfct_type_u32:
        uint32_t val_u32;

        if(fmt == NULL)
        {
            fmt = vfct_fmt_u32;
        }
        
        stat = vfct->get_fct.get_u32(&val_u32);
        if(stat == VFCT_ERR_OK)
        {
            len = snprintf(buf, buflen, fmt, val_u32);
        }
        
        break;

    case vfct_type_string:
        char const * val_s;
        
        stat = vfct->get_fct.get_string(&val_s);
        if(stat == VFCT_ERR_OK)
        {
            if(fmt == NULL)
            {
                fmt = vfct_fmt_s;
            }
            len = snprintf(buf, buflen, fmt, val_s);
        }

        break;
        
        
    default:
        stat = VFCT_ERR_ERR;
        break;
    }

    if(stat != VFCT_ERR_OK)
    {
        len = vfct_fmt_err(buf, buflen, stat);
    }    

    return len;
}

