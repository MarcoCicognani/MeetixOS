#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex cpowl(long double _Complex z, long double _Complex c) {
    return cpow(z, c);
}
#else
long double _Complex cpowl(long double _Complex z, long double _Complex c) {
    return cexpl(c * clogl(z));
}
#endif
