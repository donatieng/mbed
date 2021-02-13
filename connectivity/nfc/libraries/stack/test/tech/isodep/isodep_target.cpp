// Google Test
#include "gtest/gtest.h"

// Module to test
#include "tech/isodep/isodep_target.h"

// Fakes & mocks
#include "fake_nfc_scheduler_timer.h"
#include "mock_transceiver.h"

class IsoDepTargetTest : public ::testing::Test {
public: 
    IsoDepTargetTest() : ::testing::Test() {

    }

protected:
    void SetUp() override {
        _istream_bytes = nullptr;
        _ostream_bytes = nullptr;
        ac_buffer_init(&historical_bytes, nullptr, 0);
        nfc_tech_isodep_target_init(&isodep_target, &transceiver, &historical_bytes, &IsoDepTargetTest::s_disconnected_cb, this);
    }

    void TearDown() override {

    }

    void set_historical_bytes(const std::vector<uint8_t>& data) {
        historical_bytes_vector = data;
        ac_buffer_init(&historical_bytes, historical_bytes_vector.data(), historical_bytes_vector.size());
    }

    nfc_err_t connect() {
        return nfc_tech_isodep_target_connect(&isodep_target);
    }

    void disconnect() {
        nfc_tech_isodep_target_disconnect(&isodep_target);
    }

    nfc_err_t transmit(const std::vector<uint8_t>& data) {
        _istream_bytes = &data;
        _istream_pos = 0;
        ac_istream_init(&_istream, &IsoDepTargetTest::s_ac_istream_fn, this);
        return nfc_tech_isodep_target_transmit(&isodep_target, &_istream, &IsoDepTargetTest::s_done_cb, this);
    }

    nfc_err_t receive(std::vector<uint8_t>& data) {
        _ostream_bytes = &data;
        ac_ostream_init(&_ostream, &IsoDepTargetTest::s_ac_ostream_fn, this);
        return nfc_tech_isodep_target_receive(&isodep_target, &_ostream, &IsoDepTargetTest::s_done_cb, this);
    }

    MOCK_METHOD(void, done, (nfc_err_t ret), ());
    MOCK_METHOD(void, disconnected, (bool deselected), ());

    ac_buffer_t historical_bytes;
    std::vector<uint8_t> historical_bytes_vector;
    testing::NiceMock<MockTransceiver> transceiver;
    nfc_tech_isodep_target_t isodep_target;
private:
    ac_istream_t _istream;
    const std::vector<uint8_t>* _istream_bytes;
    size_t _istream_pos;

    ac_ostream_t _ostream;
    std::vector<uint8_t>* _ostream_bytes;

    void ac_istream_fn(ac_buffer_t *pDataIn, bool *pClose, size_t maxLength) {
        ASSERT_NE(nullptr, _istream_bytes);
        size_t length = std::min(maxLength, _istream_bytes->size() - _istream_pos);
        ac_buffer_init(pDataIn, _istream_bytes->data() + _istream_pos, length); 
        _istream_pos += length;
        if(_istream_pos == _istream_bytes->size()) {
            *pClose = true;
        }
    }

    void ac_ostream_fn(ac_buffer_t *pDataOut, bool closed) {
        (void) closed;
        ASSERT_NE(nullptr, _ostream_bytes);

        while(ac_buffer_reader_readable(pDataOut) > 0) {
            _ostream_bytes->push_back(ac_buffer_read_nu8(pDataOut));
        }
    }

    static void s_ac_istream_fn(ac_buffer_t *pDataIn, bool *pClose, size_t maxLength, void *pUserParam);
    static void s_ac_ostream_fn(ac_buffer_t *pDataOut, bool closed, void *pUserParam);
    static void s_done_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData);
    static void s_disconnected_cb(nfc_tech_isodep_t *pIsodep, bool deselected, void *pUserData);
};

void IsoDepTargetTest::s_ac_istream_fn(ac_buffer_t *pDataIn, bool *pClose, size_t maxLength, void *pUserParam) {
    static_cast<IsoDepTargetTest*>(pUserParam)->ac_istream_fn(pDataIn, pClose, maxLength);
}

void IsoDepTargetTest::s_ac_ostream_fn(ac_buffer_t *pDataOut, bool closed, void *pUserParam) {
    static_cast<IsoDepTargetTest*>(pUserParam)->ac_ostream_fn(pDataOut, closed);
}

void IsoDepTargetTest::s_done_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData) {
    static_cast<IsoDepTargetTest*>(pUserData)->done(ret);
}

void IsoDepTargetTest::s_disconnected_cb(nfc_tech_isodep_t *pIsodep, bool deselected, void *pUserData) {
    static_cast<IsoDepTargetTest*>(pUserData)->disconnected(deselected);
}

TEST_F(IsoDepTargetTest, RATS) {
    // Receive RATS
}