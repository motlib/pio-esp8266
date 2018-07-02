/**
 * @file
 *
 * "Implementation" of the development error tracer.
 *
 * There is no c module for the det, as it only consists of macros.
 */
#ifndef DET_H
#define DET_H

#include <Arduino.h>

#include "det_cfg.h"

#ifdef DET_ENABLED

/**
 * Check an assert condition.
 *
 * If the condition is violated, print the location to the serial interface and
 * stop execution by entering an infinite loop. */
#define DET_ASSERT(COND)                                 \
    do {                                                \
        if(!COND) {                                     \
            Serial.print(F("ASSERT_FAILED:"));          \
            Serial.print(F(__FILE__));                  \
            Serial.print(F(":"));                       \
            Serial.println(__LINE__);                   \
            Serial.flush();                             \
            /* Halt. Watchdog will cause a reset. */    \
            while(1);                                   \
        }                                               \
    } while(0)

#else /* DET_ENABLED */

#define DET_ASSERT(COND)

#endif /* DET_ENABLED */

#endif /* DET_H */
