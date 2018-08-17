/**
 * \file transceiver_internal.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef TRANSCEIVER_INTERNAL_H_
#define TRANSCEIVER_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"
#include "transceiver.h"

static inline void transceiver_callback(nfc_transceiver_t* pTransceiver, nfc_err_t ret)
{
  pTransceiver->cb(pTransceiver, ret, pTransceiver->pUserData);
}


#ifdef __cplusplus
}
#endif

#endif /* TRANSCEIVER_INTERNAL_H_ */
