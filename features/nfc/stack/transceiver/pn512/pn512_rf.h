/**
 * \file pn512_rf.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef PN512_RF_H_
#define PN512_RF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"
#include "pn512_callback.h"
#include "pn512.h"
#include "pn512_rf.h"

typedef struct __pn512 pn512_t;

nfc_err_t pn512_framing_set(pn512_t* pPN512, nfc_framing_t framing);

nfc_err_t pn512_framing_crc_set(pn512_t* pPN512, bool out, bool in);

nfc_err_t pn512_framing_rx_multiple_enable(pn512_t* pPN512);

#define PN512_FRAMING_IS_TARGET( framing ) ((framing) <= nfc_framing_target_f_424)

void pn512_rf_field_switch_off(pn512_t* pPN512);

void pn512_rf_field_nfcip1_rf_collision_avoidance(pn512_t* pPN512, pn512_cb_t cb);

void pn512_rf_field_wait_for_external(pn512_t* pPN512, int timeout, pn512_cb_t cb);


#ifdef __cplusplus
}
#endif

#endif /* PN512_RF_H_ */
