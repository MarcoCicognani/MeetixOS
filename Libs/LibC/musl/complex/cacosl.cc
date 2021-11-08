#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex cacosl(long double _Complex z) {
    return cacos(z);
}
#else
// FIXME
#    define PI_2 1.57079632679489661923132169163975144L
long double _Complex cacosl(long double _Complex z) {
    z = casinl(z);
    return CMPLXL(PI_2 - creall(z), -cimagl(z));
}
#endif
