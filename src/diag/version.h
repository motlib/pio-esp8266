#ifndef VERSION_H
#define VERSION_H


/**
 * Structure holding all version related information.
 */
typedef struct
{
    /* firmware version. format 00.00.00[-dev] */
    char fw_version[13];

    /* Build date in yyyymmdd-HHMM format */
    char build_date[14];

    /* The git hash code to identify the commit. */
    char git_hash[41];
} version_data_t;


extern version_data_t version_data;

#endif /* VERSION_H */
