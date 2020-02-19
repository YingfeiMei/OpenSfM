#pragma once

#include <Eigen/Dense>

#define SQUARE(x) ((x)*(x))

template< class T>
T Sign(const T& a){
  if(a < T(0)){
    return T(-1);
  }
  else{
    return T(1);
  }
}

template<class MAT, class VEC>
bool SolveAX0(const MAT& A, VEC* solution){
  // Don't solve under-constrained systems
  if(A.rows() < A.cols()){
    return false;
  }

  Eigen::JacobiSVD<MAT> svd(A, Eigen::ComputeFullV);
  const int data_size = A.cols();
  *solution = svd.matrixV().col(data_size-1);

  // Check ratio of smallest eigenvalues for single nullspace
  const double minimum_ratio = 4;
  const auto values = svd.singularValues();
  const double ratio = values(data_size-2)/values(data_size-1);

  // Some nullspace will make a solution
  const bool some_nullspace = ratio > minimum_ratio;
  if (some_nullspace)
    return true;
  else
    return false;
}

Eigen::Matrix3d SkewMatrix(const Eigen::Vector3d& v);

void SolveQuartic(const double coefficients[5], double roots[4]);
void RefineQuartic(const double coefficients[5], double roots[4], const int iterations = 2);