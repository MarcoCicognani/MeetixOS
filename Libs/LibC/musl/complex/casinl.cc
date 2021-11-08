#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex casinl(long double _Complex z) {
    return casin(z);
}
#else
// FIXME
long double _Complex casinl(long double _Complex z) {
    long double _Complex w;
    long double x, y;

    x = creall(z);
    y = cimagl(z);
    w = CMPLXL(1.0 - (x - y) * (x + y), -2.0 * x * y);
    return clogl(CMPLXL(-y, x) + csqrtl(w));
}
#endif
