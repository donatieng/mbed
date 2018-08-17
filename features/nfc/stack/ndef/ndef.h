/**
 * \file ndef.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

/** \addtogroup NDEF
 *  @{
 *  \name Generic NDEF Tag
 *  @{
 */

#ifndef NDEF_H_
#define NDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"

//Generic interface for NDEF messages

typedef struct __ndef_msg ndef_msg_t;

/** Function called to generate the tag's content on read (target mode)
 * \param pTag pointer to ndef_tag_t instance
 * \param type pMem buffer in which to store the generated content
 */
typedef nfc_err_t (*ndef_encode_fn_t)(ndef_msg_t* pTag, buffer_builder_t* pBufferBldr, void* pUserData);

/** Function called to decode the tag's content on write (target mode) or read (reader mode)
 * \param pTag pointer to ndef_tag_t instance
 * \param type pMem buffer containing the tag's content
 */
typedef nfc_err_t (*ndef_decode_fn_t)(ndef_msg_t* pTag, buffer_t* pBuffer, void* pUserData);

typedef struct __ndef_msg
{
  ndef_encode_fn_t encode;
  ndef_decode_fn_t decode;
  buffer_builder_t bufferBldr;
  void* pUserData;
} ndef_msg_t;

void ndef_msg_init( ndef_msg_t* pNdef, ndef_encode_fn_t encode, ndef_decode_fn_t decode, uint8_t* data, size_t size, void* pUserData );

static inline nfc_err_t ndef_msg_encode(ndef_msg_t* pNdef)
{
  if(pNdef->encode == NULL)
  {
    return NFC_OK;
  }
  return pNdef->encode(pNdef, &pNdef->bufferBldr, pNdef->pUserData);
}

static inline nfc_err_t ndef_msg_decode(ndef_msg_t* pNdef)
{
  if(pNdef->decode == NULL)
  {
    return NFC_OK;
  }
  return pNdef->decode(pNdef, buffer_builder_buffer(&pNdef->bufferBldr), pNdef->pUserData);
}

static inline buffer_builder_t* ndef_msg_buffer_builder(ndef_msg_t* pNdef)
{
  return &pNdef->bufferBldr;
}

//void* ndef_tag_impl(ndef_tag_t* pNdefTag);

#ifdef __cplusplus
}
#endif

#endif /* NDEF_H_ */

/**
 * @}
 * @}
 * */

