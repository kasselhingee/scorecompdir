// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "cdabyppi_types.h"
#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// corels
bool corels();
RcppExport SEXP _cdabyppi_corels() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(corels());
    return rcpp_result_gen;
END_RCPP
}
// rcpp_hello_world
List rcpp_hello_world();
RcppExport SEXP _cdabyppi_rcpp_hello_world() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(rcpp_hello_world());
    return rcpp_result_gen;
END_RCPP
}
// ptapesmo
XPtr< CppAD::ADFun<double> > ptapesmo(svecd xbetain, size_t n);
RcppExport SEXP _cdabyppi_ptapesmo(SEXP xbetainSEXP, SEXP nSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< svecd >::type xbetain(xbetainSEXP);
    Rcpp::traits::input_parameter< size_t >::type n(nSEXP);
    rcpp_result_gen = Rcpp::wrap(ptapesmo(xbetain, n));
    return rcpp_result_gen;
END_RCPP
}
// psmo
double psmo(XPtr< CppAD::ADFun<double> > pfun, svecd xin, svecd betain);
RcppExport SEXP _cdabyppi_psmo(SEXP pfunSEXP, SEXP xinSEXP, SEXP betainSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr< CppAD::ADFun<double> > >::type pfun(pfunSEXP);
    Rcpp::traits::input_parameter< svecd >::type xin(xinSEXP);
    Rcpp::traits::input_parameter< svecd >::type betain(betainSEXP);
    rcpp_result_gen = Rcpp::wrap(psmo(pfun, xin, betain));
    return rcpp_result_gen;
END_RCPP
}
// psmo
// double psmo(XPtr< CppAD::ADFun<double> > pfun, Rcpp::NumericMatrix xin, svecd betain);
RcppExport SEXP _cdabyppi_psmo(SEXP pfunSEXP, SEXP xinSEXP, SEXP betainSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr< CppAD::ADFun<double> > >::type pfun(pfunSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericMatrix >::type xin(xinSEXP);
    Rcpp::traits::input_parameter< svecd >::type betain(betainSEXP);
    rcpp_result_gen = Rcpp::wrap(psmo(pfun, xin, betain));
    return rcpp_result_gen;
END_RCPP
}
// smo_n_grad
double smo_n_grad(svecd xin, svecd betain);
RcppExport SEXP _cdabyppi_smo_n_grad(SEXP xinSEXP, SEXP betainSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< svecd >::type xin(xinSEXP);
    Rcpp::traits::input_parameter< svecd >::type betain(betainSEXP);
    rcpp_result_gen = Rcpp::wrap(smo_n_grad(xin, betain));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_cdabyppi_corels", (DL_FUNC) &_cdabyppi_corels, 0},
    {"_cdabyppi_rcpp_hello_world", (DL_FUNC) &_cdabyppi_rcpp_hello_world, 0},
    {"_cdabyppi_ptapesmo", (DL_FUNC) &_cdabyppi_ptapesmo, 2},
    {"_cdabyppi_psmo", (DL_FUNC) &_cdabyppi_psmo, 3},
    {"_cdabyppi_psmo", (DL_FUNC) &_cdabyppi_psmo, 3},
    {"_cdabyppi_smo_n_grad", (DL_FUNC) &_cdabyppi_smo_n_grad, 2},
    {NULL, NULL, 0}
};

RcppExport void R_init_cdabyppi(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
