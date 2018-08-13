/**
 * \file iso7816.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */

#ifndef ISO7816_H_
#define ISO7816_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "acore/fwk.h"
#include "transceiver/protocols.h"

#include "tech/isodep/isodep_target.h"

struct nfc_tech_iso7816_c_apdu
{
  uint8_t cla;
  uint8_t ins;
  uint8_t p1;
  uint8_t p2;
  buffer_t dataIn;
  size_t maxRespLength;
};

struct nfc_tech_iso7816_r_apdu
{
  buffer_t dataOut;
  uint16_t sw;
};

#define ISO7816_RX_BUFFER_SIZE 256

typedef struct nfc_tech_iso7816_c_apdu nfc_tech_iso7816_c_apdu_t;
typedef struct nfc_tech_iso7816_r_apdu nfc_tech_iso7816_r_apdu_t;

typedef struct nfc_tech_iso7816 nfc_tech_iso7816_t;

typedef void (*nfc_tech_iso7816_disconnected_cb)(nfc_tech_iso7816_t* pIso7816, bool deselected, void* pUserData);

struct nfc_tech_iso7816_app;
typedef struct nfc_tech_iso7816_app nfc_tech_iso7816_app_t;

struct nfc_tech_iso7816
{
  nfc_tech_isodep_target_t isoDepTarget;

  nfc_tech_iso7816_app_t* pAppList;
  nfc_tech_iso7816_app_t* pSelectedApp;

  bool disconnected;

  nfc_tech_iso7816_c_apdu_t cApdu;
  nfc_tech_iso7816_r_apdu_t rApdu;

  bool responseReady;

  nfc_tech_iso7816_disconnected_cb disconnectedCb;
  void* pUserData;

  buffer_t hist; //Historical bytes

  istream_t inputStream;
  ostream_t outputStream;

  //PDU buffer (tx)
  uint8_t txBuf[2];
  buffer_builder_t txBldr;

  //Receive buffer
  uint8_t rxBuf[ISO7816_RX_BUFFER_SIZE];
  buffer_builder_t rxBldr;
};

void nfc_tech_iso7816_init(nfc_tech_iso7816_t* pIso7816, nfc_transceiver_t* pTransceiver, nfc_tech_iso7816_disconnected_cb disconnectedCb, void* pUserData);
void nfc_tech_iso7816_add_app(nfc_tech_iso7816_t* pIso7816, nfc_tech_iso7816_app_t* pIso7816App);
void nfc_tech_iso7816_connect(nfc_tech_iso7816_t* pIso7816);
void nfc_tech_iso7816_disconnect(nfc_tech_iso7816_t* pIso7816);
nfc_err_t nfc_tech_iso7816_reply(nfc_tech_iso7816_t* pIso7816);

inline static nfc_tech_iso7816_c_apdu_t* nfc_tech_iso7816_c_apdu(nfc_tech_iso7816_t* pIso7816)
{
  return &pIso7816->cApdu;
}

inline static nfc_tech_iso7816_r_apdu_t* nfc_tech_iso7816_r_apdu(nfc_tech_iso7816_t* pIso7816)
{
  return &pIso7816->rApdu;
}

//#include "tech/iso7816/iso7816_app.h"

#ifdef __cplusplus
}
#endif

#endif /* ISO7816_H_ */
