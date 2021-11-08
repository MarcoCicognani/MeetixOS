#include "libm.h"

float cabsf(float _Complex z) {
    return hypotf(crealf(z), cimagf(z));
}
