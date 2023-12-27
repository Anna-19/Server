#include <UnitTest++/UnitTest++.h>

#include <iostream>
#include <string>
#include <map>

#include "interface.h"
#include "logger.h"
#include "listener.h"
#include "worker.h"
#include "server_errors.h"

struct Calculator {
    Worker w{0, map<string,string>{}};
};


SUITE(LoggerTests) {
    TEST(forbidden_to_write) {
        Logger logger("/var/log/vcalc.log");
        CHECK_EQUAL(false, logger.log("Test message"));
    }
    TEST(enable_to_write) {
        Logger logger("test.log");
        CHECK(logger.log("Test message"));
    }
}

SUITE(MathTests) {
    TEST_FIXTURE(Calculator, vector_zero) {
        std::vector<int32_t> test_vec{0, 0, 0, 0, 0, 0};
        CHECK_EQUAL(0, w.calculate(test_vec));
    }
    TEST_FIXTURE(Calculator, vector_negative_nums) {
        std::vector<int32_t> test_vec{-5, -6, -4684254, -0, -9564};
        CHECK_EQUAL(-938765, w.calculate(test_vec));
    }
    TEST_FIXTURE(Calculator, vector_positive_nums) {
        std::vector<int32_t> test_vec{5, 6, 4684254, 0, 9564};
        CHECK_EQUAL(938765, w.calculate(test_vec));
    }
    TEST_FIXTURE(Calculator, vector_positive_negative) {
        std::vector<int32_t> test_vec{-65, 435, -5, 7873};
        CHECK_EQUAL(2059, w.calculate(test_vec));
    }
    TEST_FIXTURE(Calculator, vector_empty) {
        std::vector<int32_t> test_vec{};
        CHECK_EQUAL(0, w.calculate(test_vec));
    }
    TEST_FIXTURE(Calculator, math_overflow) {
        std::vector<int32_t> test_vec{2147483645, 53, -422};
        CHECK_EQUAL(2147483647, w.calculate(test_vec));
    }
    TEST_FIXTURE(Calculator, math_underflow) {
        std::vector<int32_t> test_vec{-2147483640, -34, 54};
        CHECK_EQUAL(-2147483648, w.calculate(test_vec));
    }
}

int main() {
    UnitTest::RunAllTests();
    return 0;
}