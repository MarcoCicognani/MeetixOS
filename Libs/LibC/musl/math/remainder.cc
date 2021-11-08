#include "libc.h"

#include <cmath>

double remainder(double x, double y) {
    int q;
    return remquo(x, y, &q);
}

extern "C" {
weak_alias(remainder, drem);
}
