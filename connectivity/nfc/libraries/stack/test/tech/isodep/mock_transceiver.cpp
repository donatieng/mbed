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

MockTransceiver::MockTransceiver() : Transceiver(nullptr, nullptr),
    _crcOut(false), _crcIn(false),
    _timeout(0), _transceive_transmit(true), _transceive_receive(true), _transceive_repoll(false),
    _transceive_framing(nfc_framing_unknown),
    _last_byte_length(8), _first_byte_align(0) {
        ac_buffer_init(&_read_buffer, nullptr, 0);
    }

std::pair<bool, bool> MockTransceiver::get_crc() {
    return std::make_pair(_crcOut, _crcIn);
}

int MockTransceiver::get_timeout() const {
    return _timeout;
}

bool MockTransceiver::get_transceive_transmit() const {
    return _transceive_transmit;
}

bool MockTransceiver::get_transceive_receive() const {
    return _transceive_receive;
}

bool MockTransceiver::get_transceive_repoll() const {
    return _transceive_repoll;
}

nfc_framing_t MockTransceiver::get_transceive_framing() const {
    return _transceive_framing;
}  

std::vector<uint8_t> MockTransceiver::get_write_bytes() const {
    return _write_bytes;
}

void MockTransceiver::set_read_bytes(const std::vector<uint8_t>& read_bytes) {
    _read_bytes = read_bytes;
    ac_buffer_init(&_read_buffer, _read_bytes.data(), _read_bytes.size());
}

void MockTransceiver::transceive_done(nfc_err_t ret) {
    // Call done callback
    this->cb(this, ret, this->pUserData);
}

//

void MockTransceiver::set_crc(bool crcOut, bool crcIn) {
    _crcOut = crcOut;
    _crcIn = crcIn;
}

void MockTransceiver::set_timeout(int timeout) {
    _timeout = timeout;
}

void MockTransceiver::set_transceive_options(bool transmit, bool receive, bool repoll) {
    _transceive_transmit = transmit;
    _transceive_receive = receive;
    _transceive_repoll = repoll;
}

void MockTransceiver::set_transceive_framing(nfc_framing_t framing) {
    _transceive_framing = framing;
}

void MockTransceiver::set_write(ac_buffer_t *pWriteBuf) {
    _write_bytes.clear();

    if(pWriteBuf == nullptr) {
        return;
    }

    ac_buffer_t cpy;
    ac_buffer_dup(&cpy, pWriteBuf);
    while(ac_buffer_reader_readable(&cpy)) {
        _write_bytes.push_back(ac_buffer_read_nu8(&cpy));
    }
}

ac_buffer_t *MockTransceiver::get_read() {
    return &_read_buffer;
}

size_t MockTransceiver::get_last_byte_length() {
    return _last_byte_length;
}

void MockTransceiver::set_last_byte_length(size_t lastByteLength) {
    _last_byte_length = lastByteLength;
}

size_t MockTransceiver::get_first_byte_align() {
    return _first_byte_align;
}

void MockTransceiver::set_first_byte_align(size_t firstByteAlign) {
    _first_byte_align = firstByteAlign;
}

void MockTransceiver::transceive() {
    _read_bytes.clear();
    ac_buffer_init(&_read_buffer, nullptr, 0);
    transceive_wrapper();
    _timeout = 0;
    _transceive_transmit = _transceive_receive = true;
    _transceive_repoll = false;
    _write_bytes.clear();
    _last_byte_length = 8;
    _first_byte_align = 0;
}