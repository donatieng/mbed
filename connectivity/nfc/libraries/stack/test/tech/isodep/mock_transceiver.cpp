#include "mock_transceiver.h"

const transceiver_impl_t Transceiver::s_fn_table = {
    .set_protocols = Transceiver::s_set_protocols,
    .poll = Transceiver::s_poll,
    .set_crc = Transceiver::s_set_crc,
    .set_timeout = Transceiver::s_set_timeout,
    .set_transceive_options = Transceiver::s_set_transceive_options,
    .set_transceive_framing = Transceiver::s_set_transceive_framing,
    .set_write = Transceiver::s_set_write,
    .get_read = Transceiver::s_get_read,
    .set_last_byte_length = Transceiver::s_set_last_byte_length,
    .get_last_byte_length = Transceiver::s_get_last_byte_length,
    .set_first_byte_align = Transceiver::s_set_first_byte_align,
    .transceive = Transceiver::s_transceive,
    .abort = Transceiver::s_abort,
    .close = Transceiver::s_close,
    .sleep = Transceiver::s_sleep
};

Transceiver::Transceiver(nfc_transport_t *pTransport, nfc_scheduler_timer_t *pTimer) {
    transceiver_init(this, pTransport, pTimer);
    this->fn = &Transceiver::s_fn_table;
}

void Transceiver::s_set_protocols(nfc_transceiver_t *pTransceiver, nfc_tech_t initiators, nfc_tech_t targets, polling_options_t options)
{
    static_cast<Transceiver*>(pTransceiver)->set_protocols(initiators, targets, options);
}

void Transceiver::s_poll(nfc_transceiver_t *pTransceiver) {
    static_cast<Transceiver*>(pTransceiver)->poll();
}

void Transceiver::s_set_crc(nfc_transceiver_t *pTransceiver, bool crcOut, bool crcIn) {
    static_cast<Transceiver*>(pTransceiver)->set_crc(crcOut, crcIn);
}

void Transceiver::s_set_timeout(nfc_transceiver_t *pTransceiver, int timeout) {
    static_cast<Transceiver*>(pTransceiver)->set_timeout(timeout);
}

void Transceiver::s_set_transceive_options(nfc_transceiver_t *pTransceiver, bool transmit, bool receive, bool repoll) {
    static_cast<Transceiver*>(pTransceiver)->set_transceive_options(transmit, receive, repoll);
}

void Transceiver::s_set_transceive_framing(nfc_transceiver_t *pTransceiver, nfc_framing_t framing) {
    static_cast<Transceiver*>(pTransceiver)->set_transceive_framing(framing);
}

void Transceiver::s_set_write(nfc_transceiver_t *pTransceiver, ac_buffer_t *pWriteBuf) {
    static_cast<Transceiver*>(pTransceiver)->set_write(pWriteBuf);
}

ac_buffer_t *Transceiver::s_get_read(nfc_transceiver_t *pTransceiver) {
    return static_cast<Transceiver*>(pTransceiver)->get_read();
}

size_t Transceiver::s_get_last_byte_length(nfc_transceiver_t *pTransceiver) {
    return static_cast<Transceiver*>(pTransceiver)->get_last_byte_length();
}

void Transceiver::s_set_last_byte_length(nfc_transceiver_t *pTransceiver, size_t lastByteLength) {
    static_cast<Transceiver*>(pTransceiver)->set_last_byte_length(lastByteLength);
}

size_t Transceiver::s_get_first_byte_align(nfc_transceiver_t *pTransceiver) {
    return static_cast<Transceiver*>(pTransceiver)->get_first_byte_align();
}

void Transceiver::s_set_first_byte_align(nfc_transceiver_t *pTransceiver, size_t firstByteAlign) {
    static_cast<Transceiver*>(pTransceiver)->set_first_byte_align(firstByteAlign);
}

void Transceiver::s_transceive(nfc_transceiver_t *pTransceiver) {
    static_cast<Transceiver*>(pTransceiver)->transceive();
}

void Transceiver::s_abort(nfc_transceiver_t *pTransceiver) {
    static_cast<Transceiver*>(pTransceiver)->abort();
}

void Transceiver::s_close(nfc_transceiver_t *pTransceiver) {
    static_cast<Transceiver*>(pTransceiver)->close();
}

void Transceiver::s_sleep(nfc_transceiver_t *pTransceiver, bool sleep) {
    static_cast<Transceiver*>(pTransceiver)->sleep(sleep);
}