/**
 * \file pn512_poll.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef PN512_POLL_H_
#define PN512_POLL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"


//nfc_err_t pn512_target_loop(nfc_transceiver_t* pTransceiver);
void pn512_poll_setup(pn512_t* pPN512);
void pn512_poll_hw(pn512_t* pPN512, pn512_cb_t cb);


#ifdef __cplusplus
}
#endif

#endif /* PN512_POLL_H_ */
