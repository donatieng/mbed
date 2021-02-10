// Google Test
#include "gtest/gtest.h"

// Module to test
#include "tech/isodep/isodep_target.h"

// Fake
#include "fake_nfc_scheduler_timer.h"

class IsoDepTargetTest : public ::testing::Test {
 protected:
  void SetUp() override {
    
  }

  void TearDown() override {

  }
};

TEST_F(IsoDepTargetTest, Init) {
    ASSERT_EQ(1, 0);
}