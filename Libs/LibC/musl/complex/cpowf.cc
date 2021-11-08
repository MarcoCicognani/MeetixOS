#include "libm.h"

float _Complex cpowf(float _Complex z, float _Complex c) {
    return cexpf(c * clogf(z));
}
