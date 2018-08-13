/**
 * \file isodep.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef ISODEP_H_
#define ISODEP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "acore/fwk.h"
#include "transceiver/transceiver.h"

struct nfc_tech_isodep;
typedef struct nfc_tech_isodep nfc_tech_isodep_t;

typedef void (*nfc_tech_isodep_cb_t)(nfc_tech_isodep_t* pIsodep, nfc_err_t ret, void* pUserData);
typedef void (*nfc_tech_isodep_disconnected_cb)(nfc_tech_isodep_t* pIsodep, bool deselected, void* pUserData);


struct nfc_tech_isodep
{

};

#ifdef __cplusplus
}
#endif

#endif /* ISODEP_H_ */
