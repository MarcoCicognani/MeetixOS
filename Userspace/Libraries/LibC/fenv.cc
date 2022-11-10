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

/* Implementation taken from https://github.com/SerenityOS/serenity/blob/master/Userland/Libraries/LibC/fenv.cpp */

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "ConstantFunctionResult"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <LibC/fenv.h>

static constexpr uint32_t c_default_mxcsr_value = 0x1f80;

namespace Details {

static uint16_t read_status_register() {
    uint16_t status_register;
    asm volatile("fstsw %0" : "=m"(status_register));
    return status_register;
}

static uint16_t read_control_word() {
    uint16_t control_word;
    asm volatile("fstcw %0" : "=m"(control_word));
    return control_word;
}

static void set_control_word(uint16_t new_control_word) {
    asm volatile("fldcw %0" ::"m"(new_control_word));
}

static auto read_mxcsr() -> uint32_t {
    uint32_t mxcsr;
    asm volatile("stmxcsr %0" : "=m"(mxcsr));
    return mxcsr;
}

static void set_mxcsr(uint32_t new_mxcsr) {
    asm volatile("ldmxcsr %0" ::"m"(new_mxcsr));
}

} /* namespace Details */

extern "C" {

int fegetenv(fenv_t* env) {
    if ( !env )
        return 1;

    asm volatile("fstenv %0" : "=m"(env->__x87_fpu_env)::"memory");
    env->__mxcsr = Details::read_mxcsr();
    return 0;
}

int fesetenv(fenv_t const* env) {
    if ( !env )
        return 1;

    if ( env == FE_DFL_ENV ) {
        asm volatile("finit");
        Details::set_mxcsr(c_default_mxcsr_value);
        return 0;
    }

    asm volatile("fldenv %0" ::"m"(env) : "memory");
    Details::set_mxcsr(env->__mxcsr);
    return 0;
}

int feholdexcept(fenv_t* env) {
    fegetenv(env);

    fenv_t current_env;
    fegetenv(&current_env);

    current_env.__x87_fpu_env.__status_word &= ~FE_ALL_EXCEPT;
    current_env.__x87_fpu_env.__status_word &= ~(1 << 7);      /* clear the "Exception Status Summary" bit */
    current_env.__x87_fpu_env.__control_word &= FE_ALL_EXCEPT; /* mask the corresponding exception */

    fesetenv(&current_env);
    return 0;
}

int feupdateenv(fenv_t const* env) {
    auto raised_exceptions = fetestexcept(FE_ALL_EXCEPT);

    fesetenv(env);
    feraiseexcept(raised_exceptions);
    return 0;
}

int fegetexceptflag(fexcept_t* except, int exceptions) {
    if ( !except )
        return 1;

    *except = static_cast<uint16_t>(fetestexcept(exceptions));
    return 0;
}

int fesetexceptflag(fexcept_t const* except, int exceptions) {
    if ( !except )
        return 1;

    fenv_t current_env;
    fegetenv(&current_env);

    exceptions &= FE_ALL_EXCEPT;
    current_env.__x87_fpu_env.__status_word &= exceptions;
    current_env.__x87_fpu_env.__status_word &= ~(1 << 7); /* make sure exceptions don't get raised */

    fesetenv(&current_env);
    return 0;
}

int fegetround() {
    return (Details::read_status_register() >> 10) & 3;
}

int fesetround(int rounding_mode) {
    if ( rounding_mode < FE_TONEAREST || rounding_mode > FE_TOWARDZERO )
        return 1;

    auto control_word = Details::read_control_word();
    control_word &= ~(3 << 10);
    control_word |= rounding_mode << 10;

    Details::set_control_word(control_word);

    auto mxcsr = Details::read_mxcsr();
    mxcsr &= ~(3 << 13);
    mxcsr |= rounding_mode << 13;

    Details::set_mxcsr(mxcsr);
    return 0;
}

int feclearexcept(int exceptions) {
    exceptions &= FE_ALL_EXCEPT;

    fenv_t current_env;
    fegetenv(&current_env);

    current_env.__x87_fpu_env.__status_word &= ~exceptions;
    current_env.__x87_fpu_env.__status_word &= ~(1 << 7); /* clear the "Exception Status Summary" bit */

    fesetenv(&current_env);
    return 0;
}

int fetestexcept(int exceptions) {
    uint16_t status_register = Details::read_status_register() & FE_ALL_EXCEPT;
    exceptions &= FE_ALL_EXCEPT;

    return status_register & exceptions;
}

int feraiseexcept(int exceptions) {
    fenv_t env;
    fegetenv(&env);

    exceptions &= FE_ALL_EXCEPT;

    /* While the order in which the exceptions are raised is unspecified, FE_OVERFLOW and FE_UNDERFLOW must be raised
     * before FE_INEXACT, so handle that case in this branch
     */
    if ( exceptions & FE_INEXACT ) {
        env.__x87_fpu_env.__status_word &= (static_cast<uint16_t>(exceptions) & ~FE_INEXACT);
        fesetenv(&env);
        asm volatile("fwait"); /* "raise" the exception by performing a floating point operation */

        fegetenv(&env);
        env.__x87_fpu_env.__status_word &= FE_INEXACT;
        fesetenv(&env);
        asm volatile("fwait");
        return 0;
    }

    env.__x87_fpu_env.__status_word &= exceptions;
    fesetenv(&env);
    asm volatile("fwait");
    return 0;
}

} /* extern "C" */

#pragma clang diagnostic pop
