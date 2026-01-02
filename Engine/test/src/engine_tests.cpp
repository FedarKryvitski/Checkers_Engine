#include "gtest/gtest.h"

class DividerTest : public ::testing::Test {

protected:
	virtual void SetUp() {};

	virtual void TearDown() {};

	virtual void verify(int index) {
		EXPECT_EQ(1, 1);
		EXPECT_EQ(2, index);
	}
};

TEST_F(DividerTest, TEST_1) { verify(2); }