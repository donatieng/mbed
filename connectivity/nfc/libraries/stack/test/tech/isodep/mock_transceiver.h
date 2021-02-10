#include "transceiver/transceiver.h"

// Bridge C-API to CPP
class Transceiver : public nfc_transceiver_t {
    Transceiver(nfc_transport_t *pTransport, nfc_scheduler_timer_t *pTimer);
private:
    virtual void set_protocols(nfc_tech_t initiators, nfc_tech_t targets, polling_options_t options) = 0;
    virtual void poll() = 0;
    virtual void set_crc(bool crcOut, bool crcIn) = 0;
    virtual void set_timeout(int timeout) = 0;
    virtual void set_transceive_options(bool transmit, bool receive, bool repoll) = 0;
    virtual void set_transceive_framing(nfc_framing_t framing) = 0;
    virtual void set_write(ac_buffer_t *pWriteBuf) = 0;
    virtual ac_buffer_t *get_read() = 0;
    virtual size_t get_last_byte_length() = 0;
    virtual void set_last_byte_length(size_t lastByteLength) = 0;
    virtual size_t get_first_byte_align() = 0;
    virtual void set_first_byte_align(size_t firstByteAlign) = 0;
    virtual void transceive() = 0;
    virtual void abort() = 0;
    virtual void close() = 0;
    virtual void sleep(bool sleep) = 0;

    const static transceiver_impl_t s_fn_table;
    static void s_set_protocols(nfc_transceiver_t *pTransceiver, nfc_tech_t initiators, nfc_tech_t targets, polling_options_t options);
    static void s_poll(nfc_transceiver_t *pTransceiver);
    static void s_set_crc(nfc_transceiver_t *pTransceiver, bool crcOut, bool crcIn);
    static void s_set_timeout(nfc_transceiver_t *pTransceiver, int timeout);
    static void s_set_transceive_options(nfc_transceiver_t *pTransceiver, bool transmit, bool receive, bool repoll);
    static void s_set_transceive_framing(nfc_transceiver_t *pTransceiver, nfc_framing_t framing);
    static void s_set_write(nfc_transceiver_t *pTransceiver, ac_buffer_t *pWriteBuf); //Set write buffer
    static ac_buffer_t *s_get_read(nfc_transceiver_t *pTransceiver); //Get read buffer
    static size_t s_get_last_byte_length(nfc_transceiver_t *pTransceiver);
    static void s_set_last_byte_length(nfc_transceiver_t *pTransceiver, size_t lastByteLength);
    static size_t s_get_first_byte_align(nfc_transceiver_t *pTransceiver);
    static void s_set_first_byte_align(nfc_transceiver_t *pTransceiver, size_t firstByteAlign);
    static void s_transceive(nfc_transceiver_t *pTransceiver);
    static void s_abort(nfc_transceiver_t *pTransceiver);
    static void s_close(nfc_transceiver_t *pTransceiver);
    static void s_sleep(nfc_transceiver_t *pTransceiver, bool sleep);
};