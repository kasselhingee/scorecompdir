# include "../scorecompdir_types.h"
# include "approx.cpp"
# include "sm_possphere.cpp"
# include "manifold_simplex.cpp"
# include "manifold_alr.cpp"
# include "manifold_Snative.cpp"
# include "hfuns.cpp"
# include "dirichlet.cpp"
# include "vMF.cpp"
# include "PrintFor.cpp"

// define a function that tapes a log likelihood
CppAD::ADFun<double> tapell(veca1 z, //data measurement tranformed to M manifold
                            veca1 theta, //theta parameter
                               a1type (*llf)(const veca1 &, const veca1 &), //the log likelihood function
                               manifold<a1type> *pman, //it seems points must be passed for abstract classes (note error when compiling without the *, and Stefan's demo)
                               Eigen::Matrix<int, Eigen::Dynamic, 1> fixedtheta, //TRUE (1) values indicate that the corresponding value of theta is not a variable (dynamic or independent)
                               bool verbose
                               ){
  if (theta.size() != fixedtheta.size()){
    stop("theta and fixedtheta must have the same length");
  }

  //separate fixed and variable theta
  veca1 thetavar(theta.size() - fixedtheta.sum());
  veca1 thetafxd(fixedtheta.sum());
  size_t idx_var(0);
  size_t idx_fxd(0);
  for (long int i=0;i<theta.size();i++){
    if (fixedtheta[i]){
      thetafxd[idx_fxd] = theta[i];
      idx_fxd += 1;
    } else {
      thetavar[idx_var] = theta[i];
      idx_var += 1;
    }
  }

  if (verbose){
    Rcout << "Fixing according to pattern: " << std::endl;
    for (long int i=0;i<fixedtheta.size();i++){
      Rcout << " " << fixedtheta[i];
    }
    Rcout << std::endl;

    Rcout << "Fixed theta is:";
    if (thetafxd.size() == 0){
      Rcout << " none" << std::endl;
    } else {
      for (long int i=0;i<thetafxd.size();i++){
        Rcout << " " << thetafxd[i];
      }
      Rcout << std::endl;
    }
  }

  //tape relationship between x and log-likelihood
  CppAD::Independent(z, thetavar);  //for this tape, theta must be altered using new_dynamic
  if (verbose){
    Rcout << "thetavar is: " << thetavar.transpose() << std::endl;
    PrintForVec("\n thetavar is: ", thetavar);
  }

  //combine fixed and variable theta
  veca1 thetarecom(theta.size());
  idx_var = 0;
  for (long int i=0;i<theta.size();i++){
    if (fixedtheta[i]){
      thetarecom[i] = theta[i];
    } else {
      thetarecom[i] = thetavar[idx_var];
      idx_var += 1;
    }
  }
  if (verbose){
    Rcout << "thetarecom is: " << thetarecom.transpose() << std::endl;
    PrintForVec("\n thetarecom is: ", thetarecom);
  }

  // range space vector
  veca1 y(1); // vector of ranges space variables
  veca1 u(0); //0 here because size dictated by fromM
  u = pman->fromM(z);
  y.setZero();
  y[0] += llf(u, thetarecom);

  //get log determinant of fromM
  y[0] += pman->logdetJfromM(z);
  CppAD::ADFun<double> tape;  //copying the change_parameter example, a1type is used in constructing f, even though the input and outputs to f are both a2type.
  tape.Dependent(z, y);
  if (verbose){
    Rcout << "tape has " << tape.size_dyn_ind() << " independent dynamic parameters" << std::endl;
    Rcout << "tape requires vectors of length " << tape.Domain() << std::endl;
    Rcout << "tape returns vectors of length " << tape.Range() << std::endl;
  }
  return(tape);
}

CppAD::ADFun<double> tapeh2(veca1 z,
                            a1type (*h2fun)(const veca1 &, const double &),
                            const double & acut){
  //tape relationship between z and h2
  CppAD::Independent(z);
  // range space vector
  size_t m = 1;               // number of ranges space variables
  veca1 y(m); // vector of ranges space variables
  y[0] = h2fun(z, acut);
  CppAD::ADFun<double> tape;  //copying the change_parameter example, a1type is used in constructing f, even though the input and outputs to f are both a2type.
  tape.Dependent(z, y);
  return(tape);
}

// function that tapes the score-matching objective
CppAD::ADFun<double> tapesmo(veca1 u, //a vector. The composition measurement for taping
                             veca1 theta, //a vector of parameters for taping
                             CppAD::ADFun<double> & lltape,
                             manifold<a1type> &M,
                             a1type (*h2fun)(const veca1 &, const double &), // the weight function h^2
                             const double & acut, //the acut constraint for the weight functions
                             bool verbose
                             ){
    size_t d(lltape.Domain()); //manifold is embedded in Euclidean space of dimension d (may be different to simplex)

    //Projection matrix
    mata1 Pmat(d, d);

    //get h2 tape
    CppAD::ADFun<double> dh2tape;
    veca1 z(d); //size of z changed by toM result below
    z = M.toM(u); //transform u to the manifold
    CppAD::ADFun<a1type, double> h2tape; //The second type here 'double' is for the 'RecBase' in ad_fun.hpp. It doesn't seem to change the treatment of the object.
    h2tape = tapeh2(z, h2fun, acut).base2ad(); //convert to a function of a1type rather than double

    //check inputs and ll tape match
    if (lltape.Domain() != (unsigned)z.size()){stop("Dimension of z (the input measurement on the manifold) is %i, which does not match domain size of log likelihood function of %i.", z.size(), lltape.Domain());}
    if (lltape.size_dyn_ind() != theta.size()){stop("Size of parameter vector theta (=%i) does not match parameter size of log likelihood function (=%i).", theta.size(), lltape.size_dyn_ind());}

    // make ll tape higher order (i.e. as if it was taped using a2type instead of a1type)
    CppAD::ADFun<a1type, double> lltapehigher;
    lltapehigher = lltape.base2ad();

    //START TAPING
    CppAD::Independent(theta, u); //differentiate wrt beta, dynamic parameter is u
    if (verbose){
      PrintForVec("\n\n(START) theta is: ", theta);
      PrintForVec("\n\n(START) u is: ", u);
    }

    // veca1 u(n);
    z = M.toM(u); //transform u to the manifold

    Pmat = M.Pmatfun(z);
    if (verbose){PrintForMatrix("\nThe value of Pmat is: \n", Pmat);}
    veca1 h2(1);
    veca1 gradh2(d); //will be set by h2tape.Jacobian below anyway
    h2 = h2tape.Forward(0, z);
    gradh2 = h2tape.Jacobian(z);
    if (verbose){
      PrintForVec("\n h2 is: ", h2);
      PrintForVec("\n gradh2 is: ", gradh2);
    }

    //update parameters ('dynamic' values) of lltape
    lltapehigher.new_dynamic(theta);

    //grad(ll)
    veca1 jac(d); // Jacobian of ll
    jac  = lltapehigher.Jacobian(z);      // Jacobian for operation sequence
    if (verbose){PrintForVec("\nThe value of ll gradient is: ", jac);}

    //hgPg
    veca1 hgPg(1);
    hgPg[0] = 0.5 * h2[0] * (Pmat * jac).dot(jac);
    if (verbose){PrintForVec("\n(1)The value of hgPg is:", hgPg);}

    //hlap
    veca1 lapl(1);
    lapl[0] = 0.;
    for(size_t i=0; i < d; i++){
       lapl[0] += Pmat.row(i) * M.dPmatfun(z, i) * jac;
       if (verbose){PrintForMatrix("\nThe value of dPmat is: \n", M.dPmatfun(z, i));}
    }
    mata1 hess(d * d, 1);
    hess = lltapehigher.Hessian(z, 0); //the zero here is something about selecting the range-space component of f, 0 selects the first and only component, I presume.
    hess.resize(d, d);
    if (verbose){PrintForMatrix("\nThe value of ll Hessian is: \n", hess);}
    lapl[0] += (Pmat*hess).trace();
    lapl[0] *= h2[0]; //weight by h2
    if (verbose){PrintForVec("\n(2)The value of hlapl is:", lapl);}


    //ghPg
    veca1 ghPg(1);
    ghPg = gradh2.transpose() * Pmat * jac;//jac; //gradprodsq(x).transpose().eval() *
    if (verbose){PrintForVec("\n(3)The value of ghPg is:", ghPg);}

    //combine components
    veca1 smo(1);
    smo = lapl + hgPg + ghPg;
    if (verbose){PrintForVec("\n(END)The value of smo is:", smo);}

    //finish taping
    CppAD::ADFun<double> smofun;
    smofun.Dependent(theta, smo);
    smofun.optimize(); //remove some of the extra variables that were used for recording the ADFun f above, but aren't needed anymore.
    smofun.check_for_nan(false); //no error if some of the results of the Jacobian are nan.
    return(smofun);
}
