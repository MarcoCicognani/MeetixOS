/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#define TEST_CASE(name)                                                                                                \
    class TestCaseImpl##name : public UnitTest::Case {                                                                 \
    public:                                                                                                            \
        TestCaseImpl##name()                                                                                           \
            : UnitTest::Case{ #name, false } {}                                                                        \
                                                                                                                       \
        ~TestCaseImpl##name() override = default;                                                                      \
                                                                                                                       \
        void run_test() override;                                                                                      \
    };                                                                                                                 \
                                                                                                                       \
    static TestCaseImpl##name s_test_case_inst_##name{};                                                               \
                                                                                                                       \
    void TestCaseImpl##name::run_test()

#define BENCHMARK_CASE(name)                                                                                           \
    class BenchmarkCaseImpl##name : public UnitTest::Case {                                                            \
    public:                                                                                                            \
        BenchmarkCaseImpl##name()                                                                                      \
            : UnitTest::Case{ #name, true } {}                                                                         \
                                                                                                                       \
        ~BenchmarkCaseImpl##name() override = default;                                                                 \
                                                                                                                       \
        void run_test() override;                                                                                      \
    };                                                                                                                 \
                                                                                                                       \
    static BenchmarkCaseImpl##name s_benchmark_case_inst_##name{};                                                     \
                                                                                                                       \
    void BenchmarkCaseImpl##name::run_test()

namespace UnitTest {

class Case {
public:
    Case(char const* name, bool is_benchmark);
    virtual ~Case() = default;

    virtual void run_test() = 0;

    [[nodiscard]] char const* name() const;
    [[nodiscard]] bool        is_benchmark() const;

private:
    char const* m_name{ nullptr }; /* REM use TC::Collection::String when implemented */
    bool        m_is_benchmark{ false };
};

} /* namespace UnitTest */
