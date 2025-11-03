#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string>

class TestBase
{
protected:
    using TestCase = std::function<void()>;
    std::vector<TestCase> test_cases_;
    std::string test_class_name_;
    size_t passed_count_ = 0;
    size_t failed_count_ = 0;

    void ASSERT_TRUE(bool expr, const std::string &msg = "")
    {
        if (!expr)
        {
            throw std::runtime_error("ASSERT_TRUE failed: " + msg);
        }
    }

    template <typename T>
    void ASSERT_EQ(const T &actual, const T &expected, const std::string &msg = "")
    {
        if (actual != expected)
        {
            throw std::runtime_error(
                "ASSERT_EQ failed:actual=" + std::to_string(actual) +
                ", expected=" + std::to_string(expected) +
                (msg.empty() ? "" : ("(" + msg + ")")));
        }
    }
};

#endif