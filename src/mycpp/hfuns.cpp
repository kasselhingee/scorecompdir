  //////////////////////////////////////////
  // weight function and grad(h^2) functions

  //prodsq
  template <class Type>
  Type prodsq(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &x, const double & acut){
    Type prd;
    prd = x.array().square().prod();
    //constraint
    Type acutb(acut * acut);
    Type out = CppAD::CondExpLe(prd, acutb, prd, acutb);
    return(out);
  }


  //minsq
  template <class Type>
  Type minsq(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &x, const double & acut){
    Eigen::Matrix<Type, Eigen::Dynamic, 1> xsq(x.size());
    xsq = x.array().square();
    Type minval(acut * acut);
    for(size_t i=0;i<x.size();i++){
      minval = CppAD::CondExpLe(xsq[i], minval, xsq[i], minval);
    }
    return(minval);
  }

  //no weights
  template <class Type>
  Type oneweights(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &x, const double & acut){
    Type out;
    out = 1.;
    return(out);
  }


  //hprod  - doesn't work mathematically - could be removed
  template <class Type>
  Type hprod(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &x, const double & acut){
    Type prd;
    prd = x.array().prod();
    //constraint
    Type acutb(acut);
    Type out = CppAD::CondExpLe(prd, acutb, prd, acutb);
    return(out);
  }

