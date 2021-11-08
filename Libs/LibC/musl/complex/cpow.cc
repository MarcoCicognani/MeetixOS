#include "libm.h"

/* pow(z, c) = exp(c log(z)), See C99 G.6.4.1 */

double _Complex cpow(double _Complex z, double _Complex c) {
    return cexp(c * clog(z));
}
