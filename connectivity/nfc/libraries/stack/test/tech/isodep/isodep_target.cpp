// Std lib
#include <algorithm>

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
        return nfc_tech_isodep_target_transmit(&isodep_target, &_istream, &IsoDepTargetTest::s_transmit_done_cb, this);
    }

    nfc_err_t receive(std::vector<uint8_t>& data) {
        _ostream_bytes = &data;
        ac_ostream_init(&_ostream, &IsoDepTargetTest::s_ac_ostream_fn, this);
        return nfc_tech_isodep_target_receive(&isodep_target, &_ostream, &IsoDepTargetTest::s_receive_done_cb, this);
    }

    MOCK_METHOD(void, receive_done, (nfc_err_t ret), ());
    MOCK_METHOD(void, transmit_done, (nfc_err_t ret), ());
    MOCK_METHOD(void, disconnected, (bool deselected), ());

    ac_buffer_t historical_bytes;
    std::vector<uint8_t> historical_bytes_vector;
    testing::StrictMock<MockTransceiver> transceiver;
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
        *pClose = (_istream_pos == _istream_bytes->size());
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
    static void s_receive_done_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData);
    static void s_transmit_done_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData);
    static void s_disconnected_cb(nfc_tech_isodep_t *pIsodep, bool deselected, void *pUserData);
};

void IsoDepTargetTest::s_ac_istream_fn(ac_buffer_t *pDataIn, bool *pClose, size_t maxLength, void *pUserParam) {
    static_cast<IsoDepTargetTest*>(pUserParam)->ac_istream_fn(pDataIn, pClose, maxLength);
}

void IsoDepTargetTest::s_ac_ostream_fn(ac_buffer_t *pDataOut, bool closed, void *pUserParam) {
    static_cast<IsoDepTargetTest*>(pUserParam)->ac_ostream_fn(pDataOut, closed);
}

void IsoDepTargetTest::s_receive_done_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData) {
    static_cast<IsoDepTargetTest*>(pUserData)->receive_done(ret);
}

void IsoDepTargetTest::s_transmit_done_cb(nfc_tech_isodep_t *pIsodep, nfc_err_t ret, void *pUserData) {
    static_cast<IsoDepTargetTest*>(pUserData)->transmit_done(ret);
}

void IsoDepTargetTest::s_disconnected_cb(nfc_tech_isodep_t *pIsodep, bool deselected, void *pUserData) {
    static_cast<IsoDepTargetTest*>(pUserData)->disconnected(deselected);
}

TEST_F(IsoDepTargetTest, ATS_Good) {
    // Set historical bytes
    set_historical_bytes({0xAB, 0xCD, 0xEF});

    // Populate valid RATS
    // FSD Min is 256 -> FSDI is 8
    // Set DID to 0
    transceiver.set_read_bytes({0xE0 /* RATS command */, (8 << 4) | 0});

    // Send ATS back
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        EXPECT_EQ(true, transceiver.get_transceive_receive());
        EXPECT_EQ(true, transceiver.get_transceive_transmit());
        EXPECT_EQ(false, transceiver.get_transceive_repoll());
        EXPECT_EQ(std::make_pair(true, true), transceiver.get_crc());

        // Check ATS (we have set 3 historical bytes)
        // Format: TL T0 TA(1) TB(1) TC(1)
        // Our implementation supports FSC = 256; FSCI should be 8
        // TA(1): Only same bit rate divisor supported, only D=1 supported
        // TB(1): We support WFI = 8, SFGI = 2
        // TC(1): We don't support advanced protocol features, DOD or NAD
        std::vector<uint8_t> ats = {8 /* Length*/, (7 << 4) | 8, 0x80,  (8 << 4) | 2, 0x00, 0xAB, 0xCD, 0xEF};
        EXPECT_EQ(ats, transceiver.get_write_bytes());
    });
    EXPECT_EQ(NFC_OK, connect());
}

TEST_F(IsoDepTargetTest, ATS_Bad_Recover) {
    // Set historical bytes
    set_historical_bytes({0xAB, 0xCD, 0xEF});

    // Populate incorrect RATS
    transceiver.set_read_bytes({0xF0 /* Bad RATS command */, (8 << 4) | 0});

    // Transceiver should go back to receive mode
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        EXPECT_EQ(true, transceiver.get_transceive_receive());
        EXPECT_EQ(false, transceiver.get_transceive_transmit()); // Note this is false!
        EXPECT_EQ(false, transceiver.get_transceive_repoll());
        EXPECT_EQ(std::make_pair(true, true), transceiver.get_crc());

        // Now if we receive a good RATS we should recover
        transceiver.set_read_bytes({0xE0, (8 << 4) | 0});
    });

    EXPECT_EQ(NFC_OK, connect());
    
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        EXPECT_EQ(true, transceiver.get_transceive_receive());
        EXPECT_EQ(true, transceiver.get_transceive_transmit());
        EXPECT_EQ(false, transceiver.get_transceive_repoll());
        EXPECT_EQ(std::make_pair(true, true), transceiver.get_crc());
        std::vector<uint8_t> ats = {8 /* Length*/, (7 << 4) | 8, 0x80,  (8 << 4) | 2, 0x00, 0xAB, 0xCD, 0xEF};
        EXPECT_EQ(ats, transceiver.get_write_bytes());
    });

    transceiver.transceive_done(NFC_OK);
}

TEST_F(IsoDepTargetTest, ATS_Bad_Drop_Field) {
    // Populate incorrect RATS
    transceiver.set_read_bytes({0xF0 /* Bad RATS command */, (8 << 4) | 0});

    // Transceiver should go back to receive mode
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        EXPECT_EQ(true, transceiver.get_transceive_receive());
        EXPECT_EQ(false, transceiver.get_transceive_transmit()); // Note this is false!
        EXPECT_EQ(false, transceiver.get_transceive_repoll());
        EXPECT_EQ(std::make_pair(true, true), transceiver.get_crc());

        // Now let's say the field drops :(
    });

    EXPECT_EQ(NFC_OK, connect());

    EXPECT_CALL(*this, disconnected(/*deselected: */false));
    transceiver.transceive_done(NFC_ERR_FIELD);
}

TEST_F(IsoDepTargetTest, DEP_I_REQ_S_WTX_REQ_WTX_RESP) {
    // RATS
    transceiver.set_read_bytes({0xE0, (8 << 4) | 0});

    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // Get the ATS answer (not checked here) and send the first DEP command
        // Send an I-block with a simple message
        transceiver.set_read_bytes({(0 << 6) | 2 | 0 /* I Block, Block number 0 */, 0xAB, 0xCD, 0xEF});
    });

    std::vector<uint8_t> rx;

    // Make sure we are setup to receive the first piece of data
    EXPECT_EQ(NFC_OK, receive(rx));

    EXPECT_CALL(*this, receive_done(NFC_OK)).WillOnce([&](){
        std::vector<uint8_t> data = {0xAB, 0xCD, 0xEF};
        EXPECT_EQ(rx, data);
    });

    EXPECT_EQ(NFC_OK, connect());

    // As we haven't specified anything to send back, 
    // we should respond with an S-Block containing a WTX request
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // Our stack uses a WTXM of 10 --> FWT Temp = FWT * WTXM 
        // = (256*16/(13.56E6))*(2^8) * 10 = 193 ms
        std::vector<uint8_t> wtx = {(3 << 6) | (3 << 4) | 2 /* S Block, WTX */, 10 /* WTXM */};
        EXPECT_EQ(wtx, transceiver.get_write_bytes());

        // Send WTX response (which is the same as the request)
        transceiver.set_read_bytes({(3 << 6) | (3 << 4) | 2 /* S Block, WTX */, 10 /* WTXM */});
    });

    transceiver.transceive_done(NFC_OK);

    // WTX response received - this should NOT trigger another transceive
    // as we're waiting for the client to call transmit()
    transceiver.transceive_done(NFC_OK);
}


TEST_F(IsoDepTargetTest, DEP_I_REQ_I_RESP_I_REQ) {
    // RATS
    transceiver.set_read_bytes({0xE0, (8 << 4) | 0});

    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // Get the ATS answer (not checked here) and send the first DEP command
        // Send an I-block with a simple message
        transceiver.set_read_bytes({(0 << 6) | 2 | 0 /* I Block, Block number 0 */, 0xAB, 0xCD, 0xEF});
    });

    std::vector<uint8_t> rx;
    std::vector<uint8_t> tx = {0x12, 0x34, 0x56};

    // Make sure we are setup to receive the first piece of data
    EXPECT_EQ(NFC_OK, receive(rx));

    EXPECT_CALL(*this, receive_done(NFC_OK)).WillOnce([&](){
        // We have received a message (not checked here), now respond
        EXPECT_EQ(NFC_OK, transmit(tx));
        EXPECT_EQ(NFC_OK, receive(rx)); // need to setup receiving again
    });

    EXPECT_EQ(NFC_OK, connect());

    // Check that we've responded with the correct I-block
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // The target's block number is initialized to 1 and toggled on reception of a valid I-Block
        // Therefore we should respond with a 0 block number
        std::vector<uint8_t> iblock = {(0 << 6) | 2 | 0 /* I Block, Block number 0 */, 0x12, 0x34, 0x56};
        EXPECT_EQ(iblock, transceiver.get_write_bytes());

        // The reader then sends an I-Block to acknowledge reception (block number is toggled)
        transceiver.set_read_bytes({(0 << 6) | 2 | 1 /* I Block, Block number 1 */});
    });

    // Will trigger the above
    transceiver.transceive_done(NFC_OK);

    EXPECT_CALL(*this, transmit_done(NFC_OK));
    EXPECT_CALL(*this, receive_done(NFC_OK));

    // Receive the I-Block response, this should trigger a WTX request but we don't care!
    EXPECT_CALL(transceiver, transceive_wrapper());

    transceiver.transceive_done(NFC_OK);
}

TEST_F(IsoDepTargetTest, DEP_I_REQ_S_WTX_REQ_S_WTX_RESP_I_RESP) {
    // RATS
    transceiver.set_read_bytes({0xE0, (8 << 4) | 0});

    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // Get the ATS answer (not checked here) and send the first DEP command
        // Send an I-block with a simple message
        transceiver.set_read_bytes({(0 << 6) | 2 | 0 /* I Block, Block number 0 */, 0xAB, 0xCD, 0xEF});
    });

    std::vector<uint8_t> rx;

    // Make sure we are setup to receive the first piece of data
    EXPECT_EQ(NFC_OK, receive(rx));

    EXPECT_CALL(*this, receive_done(NFC_OK));

    EXPECT_EQ(NFC_OK, connect());

    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // WTX request not checked here
        // Send WTX response (which is the same as the request)
        transceiver.set_read_bytes({(3 << 6) | (3 << 4) | 2 /* S Block, WTX */, 10 /* WTXM */});
    });

    transceiver.transceive_done(NFC_OK);

    // WTX response received - this should NOT trigger another transceive
    // as we're waiting for the client to call transmit()
    transceiver.transceive_done(NFC_OK);
    
    // Now send something
    std::vector<uint8_t> tx = {0x12, 0x34, 0x56};

    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // The target's block number is initialized to 1 and toggled on reception of a valid I-Block
        // Therefore we should respond with a 0 block number
        std::vector<uint8_t> iblock = {(0 << 6) | 2 | 0 /* I Block, Block number 0 */, 0x12, 0x34, 0x56};
        EXPECT_EQ(iblock, transceiver.get_write_bytes());
    });

    EXPECT_EQ(NFC_OK, transmit(tx)); // This should trigger a transceive
}

// Chaining
TEST_F(IsoDepTargetTest, DEP_CHAINING_I_REQ_R_ACK_I_REQ_I_RESP) {
    // RATS
    transceiver.set_read_bytes({0xE0, (8 << 4) | 0});

    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // Get the ATS answer (not checked here) and send the first DEP command
        // Send an I-block with chaining
        transceiver.set_read_bytes({(0 << 6) | (1 << 4) | 2 | 0 /* I Block, Chaining, Block number 0 */, 0xAA});
    });

    std::vector<uint8_t> rx;
    std::vector<uint8_t> tx = {0x12, 0x34, 0x56};

    // Make sure we are setup to receive the first piece of data
    EXPECT_EQ(NFC_OK, receive(rx));

    EXPECT_EQ(NFC_OK, connect());

    // Check that we've responded with the correct R-ACK
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // The target's block number is initialized to 1 and toggled on reception of a valid I-Block
        // Therefore we should respond with a 0 block number
        std::vector<uint8_t> rack = {(2 << 6) | (1 << 5) | (0 << 4) | 2 | 0 /* R Block, ACK, Block number 0 */};
        EXPECT_EQ(rack, transceiver.get_write_bytes());

        // The reader then sends an I-Block to continue with the next/last fragment (block number is toggled)
        transceiver.set_read_bytes({(0 << 6) | (0 << 4) | 2 | 1 /* I Block, No chaining, Block number 1 */, 0xBB});
    });

    // Will trigger the above
    transceiver.transceive_done(NFC_OK);

    EXPECT_CALL(*this, receive_done(NFC_OK)).WillOnce([&](){
        // The message should have been reconstructed
        std::vector<uint8_t> data = {0xAA, 0xBB};
        EXPECT_EQ(rx, data);

        // Now respond
        EXPECT_EQ(NFC_OK, transmit(tx)); // Will trigger the transceive() call below
    });

    // Check that we've responded with the correct I-block
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // The target's block number is currently 0 and toggled on reception of a valid I-Block
        // Therefore we should respond with a 0 block number
        std::vector<uint8_t> iblock = {(0 << 6) | 2 | 1 /* I Block, Block number 1 */, 0x12, 0x34, 0x56};
        EXPECT_EQ(iblock, transceiver.get_write_bytes());
    });

    // Will trigger the receive_done() call above
    transceiver.transceive_done(NFC_OK);
}

TEST_F(IsoDepTargetTest, DEP_I_REQ_CHAINING_I_RESP_R_ACK_I_RESP_I_REQ) {
    // RATS
    const uint8_t fsdi = 0;
    const size_t fsdi_to_fsd[] = {16, 24, 32, 40, 48, 64, 96, 128, 256};
    const size_t fsd = (fsdi >= 9)?fsdi_to_fsd[8]:fsdi_to_fsd[fsdi];
    transceiver.set_read_bytes({0xE0, (fsdi << 4) | 0});

    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // Get the ATS answer (not checked here) and send the first DEP command
        // Send an I-block with a simple message
        transceiver.set_read_bytes({(0 << 6) | 2 | 0 /* I Block, Block number 0 */, 0xAB, 0xCD, 0xEF});
    });

    std::vector<uint8_t> rx;
    std::vector<uint8_t> tx;

    // Populate tx so that it just doesn't fit in a single frame
    // FSD is the limit for the full frame (including PCB, and CRC)
    // We are not using CID and NAD so the maximum payload
    // that fits in a single I-Block is (FSD - 3)
    // So we're sending FSD - 3 + 1 bytes
    for(size_t i = 0; i < (fsd - 3) + 1; i++) {
        tx.push_back(i);
    }

    // Make sure we are setup to receive the first piece of data
    EXPECT_EQ(NFC_OK, receive(rx));

    EXPECT_CALL(*this, receive_done(NFC_OK)).WillOnce([&](){
        // We have received a message (not checked here), now respond
        EXPECT_EQ(NFC_OK, transmit(tx));
        EXPECT_EQ(NFC_OK, receive(rx)); // need to setup receiving again
    });

    EXPECT_EQ(NFC_OK, connect());

    // Check that we've responded with the correct I-block
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // The target's block number is initialized to 1 and toggled on reception of a valid I-Block
        // Therefore we should respond with a 0 block number
        std::vector<uint8_t> iblock = {(0 << 6) | (1 << 4) | 2 | 0 /* I Block, Chaining, Block number 0 */};
        std::copy(tx.cbegin(), tx.cend() - 1, std::back_inserter(iblock));
        EXPECT_EQ(iblock, transceiver.get_write_bytes());

        // The reader then sends an R-ACK Block to acknowledge reception (block number is toggled)
        transceiver.set_read_bytes({(2 << 6) | (1 << 5) | (0 << 4) | 2 | 1 /* R Block, ACK, Block number 1 */});
    });

    // Will trigger the above
    transceiver.transceive_done(NFC_OK);

    // Check that we've responded with the correct I-block
    EXPECT_CALL(transceiver, transceive_wrapper()).WillOnce([&](){
        // The target's block number is currently set to 0 and toggled on reception of a valid R-Block
        // Therefore we should respond with a 1 block number
        std::vector<uint8_t> iblock = {(0 << 6) | (0 << 4) | 2 | 1 /* I Block, No chaining, Block number 1 */, *(tx.cend() - 1) /* last byte */};
        EXPECT_EQ(iblock, transceiver.get_write_bytes());

        // The reader then sends an I Block to acknowledge reception (block number is toggled)
        transceiver.set_read_bytes({(0 << 6) | 2 | 0 /* I Block, Block number 0 */, 0xAA, 0xBB});
    });

    // Will trigger the above
    transceiver.transceive_done(NFC_OK);

    EXPECT_CALL(*this, transmit_done(NFC_OK));
    EXPECT_CALL(*this, receive_done(NFC_OK));

    // Receive the I-Block response, this should trigger a WTX request but we don't care!
    EXPECT_CALL(transceiver, transceive_wrapper());

    transceiver.transceive_done(NFC_OK);
}

// Deselection
