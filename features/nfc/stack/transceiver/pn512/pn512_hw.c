/**
 * \file pn512_hw.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details Format and execute PN512 frames
 */

#include "inc/nfc.h"

#include "pn512_hw.h"

//Platform specific
#include "platform/nfc_transport.h"



/** \addtogroup PN512
 *  \internal
 *  @{
 *  \name Hardware
 *  @{
 */

/** \internal Initialize underlying pn512_hw_t structure
 * \param pPN512 pointer to pn512_t structure
 */
void pn512_hw_init(pn512_t* pPN512)
{
  //Nothing to init in this implementation
  (void) pPN512;
}


/**
 * @}
 * @}
 * */

