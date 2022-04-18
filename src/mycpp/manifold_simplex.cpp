// code for various tools for the simplex in full Euclidean space
namespace simplex { // begin the empty namespace

  template <class Type>
  Eigen::Matrix<Type, Eigen::Dynamic, 1> toM(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &x){
     Eigen::Matrix<Type, Eigen::Dynamic, 1> out(x.size());
     out = x;
     return(out);
  }

  template <class Type>
  Eigen::Matrix<Type, Eigen::Dynamic, 1> fromM(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &x){
     Eigen::Matrix<Type, Eigen::Dynamic, 1> out(x.size());
     out = x;
     return(out);
  }

  template <class Type>
  Type logdetJ_fromM(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &z){
     Type out;
     out = 0.;
     return(out);
  }


  // manifold tangent-plane projection matrix P (for isometric(?) embeddings this is closely related to the manifold metric
  template <class Type>
  Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> Pmat_M(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &x){
    int n = x.size();
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> Pmat(n, n);
    Eigen::Matrix<Type, Eigen::Dynamic, 1> ones(n);
    ones.setOnes();
    double nd = n;
    Pmat = Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>::Identity(n,n) - (ones*ones.transpose()/nd);
    return(Pmat);
  }

  //partial derivative of the tangent-plane projection matrix
  template <class Type>
  Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> dPmat_M(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &x, const int &d){
    int n = x.size();
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> bvx(n, n);
    bvx.setZero();
    return(bvx);
  }


////////////////////APPROX HELPERS/////////////////////
  //automatically choose approximation centre
  template <class Type>
  Eigen::Matrix<Type, Eigen::Dynamic, 1> taylorapprox_bdry(
		  CppAD::ADFun<Type> &f,
		  const size_t order,
		  const Eigen::Matrix<Type, Eigen::Dynamic, 1> xbeta,
		  double shiftsize=1E-5){
     Eigen::Matrix<Type, Eigen::Dynamic, 1> x(xbeta.size() / 2);
     x << xbeta.block(0,0,x.size(), 1);
     Eigen::Matrix<Type, Eigen::Dynamic, 1> shiftdir(x.size());
     shiftdir.setOnes();
     shiftdir *= -1 * shiftsize;
     shiftdir = Pmat_M(x) * shiftdir;

     Eigen::Matrix<Type, Eigen::Dynamic, 1> centre(xbeta.size());
     centre = xbeta;
     centre.block(0,0,x.size(), 1) << x + shiftdir;
     std::cout << "Approximation centre is:" << shiftdir.transpose() << std::endl;
     Eigen::Matrix<Type, Eigen::Dynamic, 1> out(0);
     out = taylorapprox(f, centre, order, xbeta);
     return(out);
  }

}