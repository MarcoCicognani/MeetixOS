#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex cacoshl(long double _Complex z) {
    return cacosh(z);
}
#else
long double _Complex cacoshl(long double _Complex z) {
    z = cacosl(z);
    return CMPLXL(-cimagl(z), creall(z));
}
#endif
