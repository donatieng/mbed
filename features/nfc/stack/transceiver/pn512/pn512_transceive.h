/**
 * \file pn512_transceive.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef PN512_TRANSCEIVE_H_
#define PN512_TRANSCEIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"

//nfc_err_t pn512_transceive(nfc_transceiver_t* pTransceiver);
void pn512_transceive_hw(pn512_t* pPN512, pn512_transceive_mode_t mode, pn512_cb_t cb);

void pn512_transceive_hw_tx_task(uint32_t events, void* pUserData);
void pn512_transceive_hw_tx_iteration(pn512_t* pPN512, bool start);

void pn512_transceive_hw_rx_start(pn512_t* pPN512);
void pn512_transceive_hw_rx_task(uint32_t events, void* pUserData);


#ifdef __cplusplus
}
#endif

#endif /* PN512_TRANSCEIVE_H_ */
