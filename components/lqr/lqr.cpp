#include "lqr.hpp"
#include "mat.h"
#include <algorithm>

bool dare_solver::iterate() {
  if (remaining_iters == 0) return false;
  dspm::Mat bt_p = B.t() * P;
  dspm::Mat s = R + bt_p * B;
  dspm::Mat rhs = bt_p * A;
  dspm::Mat k = dspm::Mat::solve(s, rhs);

  dspm::Mat ap = A.t() * P;
  dspm::Mat p_next = Q + ap * A - ap * B * k;
  float diff_norm = (p_next - P).norm();
  float denom = std::max(prev_norm, 1.0f);

  if (diff_norm / denom < tolerance) return true;

  P = p_next;
  prev_norm = P.norm();

  return true;
}

dspm::Mat dare_solver::solution() const {
  return P;
}

lqr solve_lqr(dspm::Mat &A, dspm::Mat &B, const dspm::Mat &Q, const dspm::Mat &R) {
  dare_solver solver(A, B, Q, R);
  while (!solver.iterate());
  dspm::Mat P = solver.solution();

  dspm::Mat S = R + B.t() * P * B;
  dspm::Mat K = dspm::Mat::solve(S, B.t() * P * A);
  dspm::Mat F = dspm::Mat::solve(S, B.t() * P);

  return lqr(K, F);
}

