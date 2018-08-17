/**
 * \file type4_target.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef TECH_TYPE4_TYPE4_TARGET_H_
#define TECH_TYPE4_TYPE4_TARGET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"

#include "tech/iso7816/iso7816.h"
#include "tech/iso7816/iso7816_app.h"
#include "ndef/ndef.h"

typedef struct nfc_tech_type4_target nfc_tech_type4_target_t;

typedef void (*nfc_tech_type4_cb)(nfc_tech_type4_target_t* pType4Target, nfc_err_t ret, void* pUserData);

struct nfc_tech_type4_target
{
  nfc_tech_iso7816_app_t app;

  ndef_msg_t* pNdef;

  uint8_t ccFileBuf[15];
  buffer_builder_t ccFileBldr;

  uint8_t ndefFileBuf[2];
  buffer_builder_t ndefFileBldr;

  uint16_t selFile;

  bool written;
};

void nfc_tech_type4_target_init(nfc_tech_type4_target_t* pType4Target, nfc_tech_iso7816_t* pIso7816, ndef_msg_t* pNdef);

#ifdef __cplusplus
}
#endif

#endif /* TECH_TYPE4_TYPE4_TARGET_H_ */
