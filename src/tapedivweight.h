#ifndef tapedivweight
#define tapedivweight

# include "scorecompdir_types.h"

CppAD::ADFun<double> tapeh2(veca1 z,
                            a1type (*h2fun)(const veca1 &, const double &),
                            const double & acut);

#endif