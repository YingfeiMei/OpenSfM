#include <foundation/numeric.h>
#include <iostream>

Eigen::Matrix3d SkewMatrix(const Eigen::Vector3d& v){
  Eigen::Matrix3d m;
  m <<    0, -v(2),  v(1),
       v(2),     0, -v(0),
      -v(1),  v(0),     0;
  return m;
}

// taken from https://github.com/sidneycadot/quartic
static std::complex<double> ComplexSqrt(const std::complex<double> & z){
    return pow(z, 1. / 2.);
}

static std::complex<double> ComplexCbrt(const std::complex<double> & z){
    return pow(z, 1. / 3.);
}

void SolveQuartic(const double coefficients[5], double roots[4]){
    const double a = coefficients[4];
    const double b = coefficients[3] / a;
    const double c = coefficients[2] / a;
    const double d = coefficients[1] / a;
    const double e = coefficients[0] / a;

    const double Q1 = c * c - 3. * b * d + 12. * e;
    const double Q2 = 2. * c * c * c - 9. * b * c * d + 27. * d * d + 27. * b * b * e - 72. * c * e;
    const double Q3 = 8. * b * c - 16. * d - 2. * b * b * b;
    const double Q4 = 3. * b * b - 8. * c;

    const std::complex<double> Q5 = ComplexCbrt(Q2 / 2. + ComplexSqrt(Q2 * Q2 / 4. - Q1 * Q1 * Q1));
    const std::complex<double> Q6 = (Q1 / Q5 + Q5) / 3.;
    const std::complex<double> Q7 = 2. * ComplexSqrt(Q4 / 12. + Q6);

    roots[0] = (-b - Q7 - ComplexSqrt(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7)).real() / 4.;
    roots[1] = (-b - Q7 + ComplexSqrt(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7)).real() / 4.;
    roots[2] = (-b + Q7 - ComplexSqrt(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7)).real() / 4.;
    roots[3] = (-b + Q7 + ComplexSqrt(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7)).real() / 4.;
}

void RefineQuartic
(
  const double coeffs[5],
  double roots[4],
  const int iterations)
{
  for (int i = 0; i < iterations; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
        auto& root = roots[j];
      const double error =
        coeffs[4] + root * (coeffs[3] +
                            root * (coeffs[2] +
                                    root * (coeffs[1] +
                                            root * coeffs[0])));
      
      const double derivative =
        coeffs[3] + root * (2 * coeffs[2] +
                            root * ((4 * coeffs[0] * root + 3 * coeffs[1])));
      std::cout << error << "\t" << derivative << std::endl;
      root -= error / derivative;
    }
  }
}