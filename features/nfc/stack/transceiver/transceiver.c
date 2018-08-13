/**
 * \file transceiver.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details Transceiver
 */

/** \addtogroup Transceiver
 *  @{
 *  \name Initialization
 *  @{
 */

#include "inc/nfc.h"
#include "transceiver.h"
//#include "event/transaction_event.h"

/** Initialize nfc_transceiver_t structure
 * \param pTransceiver pointer to nfc_transceiver_t structure to initialize
 * \param pTransport pointer to already initialized nfc_transport_t structure
 * \param pImpl pointer to the structure implementing the transceiver interface (eg pn512_t or pn532_t)
 */
void transceiver_init(nfc_transceiver_t* pTransceiver, nfc_transport_t* pTransport, nfc_scheduler_timer_t* pTimer)
{
  pTransceiver->pTransport = pTransport;
  //pTransceiver->pImpl = pImpl;
//  transaction_event_init(pTransceiver);

  //pTransceiver->result = NFC_OK;
  scheduler_init(&pTransceiver->scheduler, pTimer);
}


/**
 * @}
 * @}
 * */
