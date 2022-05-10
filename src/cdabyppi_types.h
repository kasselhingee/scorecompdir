# include <iostream>        // standard input/output
# include <vector>          // standard vector
# include <cppad/example/cppad_eigen.hpp>  //load eigen
# include <cppad/cppad.hpp> // the CppAD package
# include <Eigen/LU> //for matrix determinant
# include <Rcpp.h>
using namespace Rcpp;

typedef std::vector<double> svecd;
typedef Eigen::Matrix<double, Eigen::Dynamic, 1> vecd; //a vector of double values
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matd;//a matrix of double
typedef CppAD::AD<double> a1type;   // for first (outer) level of taping
typedef Eigen::Matrix<a1type, Eigen::Dynamic, 1> veca1; //a vector of a1type values
typedef Eigen::Matrix<a1type, Eigen::Dynamic, Eigen::Dynamic> mata1;//a matrix of a1types
typedef CppAD::AD<a1type> a2type;  // for second (inner) level of taping
typedef Eigen::Matrix<a2type, Eigen::Dynamic, 1> veca2;

// template <typename T>
// struct manifold {
//   Eigen::Matrix<T, Eigen::Dynamic, 1> (*toM)(const Eigen::Matrix<T, Eigen::Dynamic, 1> &); //map from simplex to manifold
//   Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> (*Pmatfun)(const Eigen::Matrix<T, Eigen::Dynamic, 1> &); //projection matrix for manifold
//   Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> (*dPmatfun)(const Eigen::Matrix<T, Eigen::Dynamic, 1> &, const int &); //elementwise derivative of projection matrix for manifold
//   Eigen::Matrix<T, Eigen::Dynamic, 1> (*fromM)(const Eigen::Matrix<T, Eigen::Dynamic, 1> &); //transformation from manifold to simplex
//   T (*logdetJfromM)(const Eigen::Matrix<T, Eigen::Dynamic, 1> &); //determinant of Jacobian of the tranformation - for correcting the likelihood function as it is a density
// };

template <typename T>
struct manifold { //exactly like a class, but with default public members https://stackoverflow.com/questions/24196885/can-c-struct-have-member-functions
  //make these members 'pure virtual' means this is an 'abstract class'
  virtual Eigen::Matrix<T, Eigen::Dynamic, 1> toM(const Eigen::Matrix<T, Eigen::Dynamic, 1> &) = 0; //map from simplex to manifold
  virtual Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Pmatfun(const Eigen::Matrix<T, Eigen::Dynamic, 1> &) = 0; //projection matrix for manifold
  virtual Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> dPmatfun(const Eigen::Matrix<T, Eigen::Dynamic, 1> &, const int &) = 0; //elementwise derivative of projection matrix for manifold
  virtual Eigen::Matrix<T, Eigen::Dynamic, 1> fromM(const Eigen::Matrix<T, Eigen::Dynamic, 1> &) = 0; //transformation from manifold to simplex
  virtual T logdetJfromM(const Eigen::Matrix<T, Eigen::Dynamic, 1> &) = 0; //determinant of Jacobian of the tranformation - for correcting the likelihood function as it is a density
  //for taping will need to pass copies - so that the coefficients of the tape are not updated by other calls part way through
  virtual ~manifold(){}; //destructor
  manifold(){};
};

template <typename T>
struct simplexman : public manifold<T> {
  Eigen::Matrix<T, Eigen::Dynamic, 1> toM(const Eigen::Matrix<T, Eigen::Dynamic, 1> &x){
    Eigen::Matrix<T, Eigen::Dynamic, 1> out(x.size());
    out = x;
    return(out);
  }

  Eigen::Matrix<T, Eigen::Dynamic, 1> fromM(const Eigen::Matrix<T, Eigen::Dynamic, 1> &x){
    Eigen::Matrix<T, Eigen::Dynamic, 1> out(x.size());
    out = x;
    return(out);
  }

  T logdetJfromM(const Eigen::Matrix<T, Eigen::Dynamic, 1> &z){
    T out;
    out = 0.;
    return(out);
  }


  // manifold tangent-plane projection matrix P (for isometric(?) embeddings this is closely related to the manifold metric
  Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Pmatfun(const Eigen::Matrix<T, Eigen::Dynamic, 1> &x){
    int n = x.size();
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Pmat(n, n);
    Eigen::Matrix<T, Eigen::Dynamic, 1> ones(n);
    ones.setOnes();
    double nd = n;
    Pmat = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>::Identity(n,n) - (ones*ones.transpose()/nd);
    return(Pmat);
  }

  //partial derivative of the tangent-plane projection matrix
  Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> dPmatfun(const Eigen::Matrix<T, Eigen::Dynamic, 1> &x, const int &d){
    int n = x.size();
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> bvx(n, n);
    bvx.setZero();
    return(bvx);
  }

  ~simplexman(){};
  simplexman(){};
};
