#ifndef LQR_H
#define LQR_H

#include "esp_dsp.h"

/// computed K and F gains for an LQR controller
class lqr {
public:
  lqr(dspm::Mat K, dspm::Mat F): K(K), F(F) {}
  dspm::Mat calculate(const dspm::Mat &x_ref, const dspm::Mat &x) const {
    return K * (-1.) * x + F * x_ref;
  }
private:
  dspm::Mat K;
  dspm::Mat F;
};

lqr solve_lqr(dspm::Mat &A, dspm::Mat &b, const dspm::Mat &Q, const dspm::Mat &R);

/// discrete algebraic riccati equation solver to compute lqr gains
class dare_solver {
public:
  dare_solver(
    dspm::Mat &A,
    dspm::Mat &B,
    const dspm::Mat &Q,
    const dspm::Mat &R,
    int max_iters=20,
    float tolerance=1e-4
  ): A(A), B(B), Q(Q), R(R), P(Q), remaining_iters(max_iters), tolerance(tolerance) {
    prev_norm = P.norm();
  }
  /// returns: true if the solver is finished
  bool iterate();
  /// returns: current DARE solution
  dspm::Mat solution() const;
private:
  dspm::Mat &A;
  dspm::Mat &B;
  const dspm::Mat &Q;
  const dspm::Mat &R;
  dspm::Mat P;
  float prev_norm;
  int remaining_iters;
  float tolerance;
};

#endif
