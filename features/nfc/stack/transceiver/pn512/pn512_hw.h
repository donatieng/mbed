/**
 * \file pn512_hw.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

#ifndef PN512_HW_H_
#define PN512_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"

#include "pn512.h"

//Utility for transport: SPI address read/write
#define PN512_SPI_ADDR_R(x) ((1<<7) | ((x) << 1))
#define PN512_SPI_ADDR_W(x) ((0<<7) | ((x) << 1))

void pn512_hw_init(pn512_t* pPN512);

/** \internal Write bytes at the specified address on the underlying transport link
 * \param pPN512 pointer to pn512_t structure
 * \param addr address at which to write
 * \param buf buffer to write
 * \param len length of buffer
 */
static inline void pn512_hw_write(pn512_t* pPN512, uint8_t addr, uint8_t* buf, size_t len)
{
  nfc_transport_write(((nfc_transceiver_t*)pPN512)->pTransport, addr, buf, len);
}

/** \internal Read bytes from the specified address on the underlying transport link
 * \param pPN512 pointer to pn512_t structure
 * \param addr address from which to read
 * \param buf buffer to read
 * \param len length of buffer
 */
static inline void pn512_hw_read(pn512_t* pPN512, uint8_t addr, uint8_t* buf, size_t len)
{
  nfc_transport_read(((nfc_transceiver_t*)pPN512)->pTransport, addr, buf, len);
}

static inline void pn512_hw_write_buffer(pn512_t* pPN512, uint8_t addr, buffer_t* pData, size_t len)
{
  while( len > 0 )
  {
    if( buffer_reader_readable(pData) == 0 )
    {
      return;
    }
    size_t cpyLen = MIN(len, buffer_reader_current_buffer_length(pData));
    nfc_transport_write(((nfc_transceiver_t*)pPN512)->pTransport, addr, buffer_reader_current_buffer_pointer(pData), cpyLen);
    buffer_read_n_skip(pData, cpyLen);
    len -= cpyLen;
  }
}

static inline void pn512_hw_read_buffer(pn512_t* pPN512, uint8_t addr, buffer_builder_t* pData, size_t len)
{
  while( len > 0 )
  {
    if( buffer_builder_writeable(pData) == 0 )
    {
      return;
    }
    //Read payload
    size_t cpyLen = MIN(len, buffer_builder_space(pData));
    nfc_transport_read(((nfc_transceiver_t*)pPN512)->pTransport, addr, buffer_builder_write_position(pData), cpyLen);
    buffer_builder_write_n_skip(pData, cpyLen);
    len -= cpyLen;
  }
}

#ifdef __cplusplus
}
#endif

#endif /* PN512_HW_H_ */
