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

#include <CCLang/Lang/StringView.hh>

/**
 * @brief Define a Case for testing an aspect of something
 */
#define TEST_CASE(name)                                                                                                                    \
    class TestCaseImpl##name : public UnitTest::Case {                                                                                     \
    public:                                                                                                                                \
        [[nodiscard]] static auto construct_and_register() -> TestCaseImpl##name { return TestCaseImpl##name{}; }                 \
                                                                                                                                           \
        ~TestCaseImpl##name() override = default;                                                                                          \
                                                                                                                                           \
        void run_test() const override;                                                                                                    \
                                                                                                                                           \
    private:                                                                                                                               \
        explicit TestCaseImpl##name()                                                                                             \
            : UnitTest::Case{ #name##sv, false } {                                                                                         \
        }                                                                                                                                  \
    };                                                                                                                                     \
    [[gnu::used]]                                                                                                                          \
    static auto const s_test_case_inst_##name = TestCaseImpl##name::construct_and_register();                                              \
                                                                                                                                           \
    void TestCaseImpl##name::run_test() const

/**
 * @brief Define a Case for testing the performances of something
 */
#define BENCHMARK_CASE(name)                                                                                                               \
    class BenchmarkCaseImpl##name : public UnitTest::Case {                                                                                \
    public:                                                                                                                                \
        [[nodiscard]] static auto construct_and_register() -> BenchmarkCaseImpl##name { return BenchmarkCaseImpl##name{}; }       \
                                                                                                                                           \
        ~BenchmarkCaseImpl##name() override = default;                                                                                     \
                                                                                                                                           \
        void run_test() const override;                                                                                                    \
                                                                                                                                           \
    private:                                                                                                                               \
        explicit BenchmarkCaseImpl##name()                                                                                        \
            : UnitTest::Case{ #name##sv, true } {                                                                                          \
        }                                                                                                                                  \
    };                                                                                                                                     \
    [[gnu::used]]                                                                                                                          \
    static auto const s_benchmark_case_inst_##name = BenchmarkCaseImpl##name::construct_and_register();                                    \
                                                                                                                                           \
    void BenchmarkCaseImpl##name::run_test() const

namespace UnitTest {

class Case {
public:
    virtual ~Case() = default;

    /**
     * @brief Test main
     */
    virtual void run_test() const = 0;

    /**
     * @brief Getters
     */
    [[nodiscard]] auto name() const -> StringView;
    [[nodiscard]] auto is_benchmark() const -> bool;

protected:
    explicit Case(StringView name, bool is_benchmark) noexcept;

private:
    StringView m_name;
    bool       m_is_benchmark{ false };
};

} /* namespace UnitTest */
