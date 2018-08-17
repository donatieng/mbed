/**
 * \file pn512_timer.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef PN512_TIMER_H_
#define PN512_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"

typedef struct __pn512 pn512_t;

void pn512_timer_config(pn512_t* pPN512, bool autostart, uint16_t prescaler, uint16_t countdown_value);

void pn512_timer_start(pn512_t* pPN512);
void pn512_timer_stop(pn512_t* pPN512);

#ifdef __cplusplus
}
#endif

#endif /* PN512_TIMER_H_ */
