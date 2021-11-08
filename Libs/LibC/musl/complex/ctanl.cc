#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex ctanl(long double _Complex z) {
    return ctan(z);
}
#else
long double _Complex ctanl(long double _Complex z) {
    z = ctanhl(CMPLXL(-cimagl(z), creall(z)));
    return CMPLXL(cimagl(z), -creall(z));
}
#endif
