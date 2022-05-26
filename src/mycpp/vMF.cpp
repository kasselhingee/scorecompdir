namespace { // begin the empty namespace

  template <class T>
  T ll_vMF(const Eigen::Matrix<T, Eigen::Dynamic, 1> &u,
                 const Eigen::Matrix<T, Eigen::Dynamic, 1> &theta)
    //lklhood is log(k*mu.x), where mu is a unit vector
    //for this software theta is the vector k*mu, which is unrestricted (except mu shouldn't be zero)
  {
    T y(0.);  // initialize summation
    for(size_t i = 0; i < u.size(); i++)
    {   y   += theta[i] * u[i];
    }
    return y;
  }

  template <class T>
  Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>
  BinghamMatrix(const Eigen::Matrix<T, Eigen::Dynamic, 1> &theta){
    // A is symmetric, and the sum of the diagonals is zero
    //assume the parameter vector theta is encoded as:
    //c(diag(A)[1:(p-1)], A[upper.tri(ALs)]
    size_t d = (-3 + std::sqrt(17 + 8 * theta.size()))/2;
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Amat(d, d);
    Amat.setZero();
    //populate the diagonal
    size_t vecidx = 0;
    for (size_t row=0; row < d-1; row++){
      Amat(row,row) = theta[vecidx];
      vecidx +=1;
    }
    Amat(d-1, d-1) = -theta.block(0, 0, d - 1, 1).sum(); //so trace of A is zero

    //populate the upper and lower triangles
    //the upper triangle has d-1 rows, the rows have 1 to (d-1) elements. Arithmetic series:
    //(d-1)/2 [2 + (d-1−1)] = (d-1) (2 + d - 2)/2 = (d - 1) d/2
    Eigen::Matrix<T, Eigen::Dynamic, 1> upptriblock((d - 1) * d/2);
    upptriblock = theta.block(d-1, 0, upptriblock.size(), 1);
    vecidx = 0;
    for (size_t col=1; col < d; col++){
      for (size_t row=0; row < col; row++){
        Amat(row, col) = upptriblock[vecidx]; //bug fix - column index is 0!!
        Amat(col, row) = upptriblock[vecidx];
        vecidx +=1;
      }
    }
    return(Amat);
  }

  template <class T>
  T ll_Bingham(const Eigen::Matrix<T, Eigen::Dynamic, 1> &u,
           const Eigen::Matrix<T, Eigen::Dynamic, 1> &theta)
    //lklhood is log(u*A*u) - from Mardia et al 2016
    // A is symmetric, and the sum of the diagonals is zero
  {
    size_t d  = u.size();
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Amat(d, d);
    Amat = BinghamMatrix(theta);

    Eigen::Matrix<T, 1, 1> out_e;
    out_e = u.transpose() * Amat * u;
    T out(out_e[0]);
    return out;
  }

  template <class T>
  T ll_FB(const Eigen::Matrix<T, Eigen::Dynamic, 1> &u,
               const Eigen::Matrix<T, Eigen::Dynamic, 1> &theta)
    //lklhood is log(u*A*u + km*u) - from Mardia et al 2016
    // A is symmetric, and the sum of the diagonals is zero
    //m*k is any vector
  {
    size_t d  = u.size();
    size_t Binghamthetasize = d - 1 + (d-1)*d/2;
    Eigen::Matrix<T, Eigen::Dynamic, 1> Btheta;
    Btheta = theta.block(0,0, Binghamthetasize, 1);
    Eigen::Matrix<T, Eigen::Dynamic, 1> Ftheta;
    Ftheta = theta.block(Binghamthetasize,0, d, 1);
    T out;
    out = ll_Bingham(u, Btheta);
    out += ll_vMF(u, Ftheta);
    return(out);
  }

  template <class T>
  T ll_Rivest(const Eigen::Matrix<T, Eigen::Dynamic, 1> &u,
          const Eigen::Matrix<T, Eigen::Dynamic, 1> &theta){
    //assume the first part of theta defines the matrix A
    // then the next element is concentration parameter, and
    // the final element is the eigenvector/value to use
    size_t d  = u.size();
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Amat(d, d);
    Eigen::Matrix<T, Eigen::Dynamic, 1> Btheta;
    Btheta = theta.block(0,0, d - 1 + (d-1)*d/2, 1);
    Amat = BinghamMatrix(Btheta);
    T out;
    out = u.transpose() * Amat * u;

    std::cout << "Amat is:" << std::endl << Amat << std::endl;

    // Eigen::SelfAdjointEigenSolver<Eigen::Matrix2f> eigensolver(Amat);
    // if (eigensolver.info() != Eigen::Success) return(out);
    // std::cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << std::endl;
    // std::cout << "Here's a matrix whose columns are eigenvectors of A \n"
    //           << "corresponding to these eigenvalues:\n"
    //           << eigensolver.eigenvectors() << std::endl;

   return(out);
  }

}
