#include "libm.h"

float _Complex conjf(float _Complex z) {
    return CMPLXF(crealf(z), -cimagf(z));
}
