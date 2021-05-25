#ifndef CONTINUOUS_STRATEGY_HPP
#define CONTINUOUS_STRATEGY_HPP

#include <iostream>
#include <cstdint>
#include <functional>
#include "BinaryStrategy.hpp"

// Cooperate : 1, Defect : 0
// Good : 1, Bad : 0

using assessment_rule_t = std::function<double(double,double,double)>;
using action_rule_t = std::function<double(double,double)>;

class ContinuousStrategy {
  public:
  ContinuousStrategy(const assessment_rule_t &assess, const action_rule_t &action) : assess(assess), action(action) {};
  assessment_rule_t assess;
  action_rule_t action;
  double Act(double donor_rep, double recip_rep) const { return action(donor_rep, recip_rep); }
  double Assess(double action, double donor_rep, double recip_rep) const { return assess(action, donor_rep, recip_rep); }
  const static ContinuousStrategy AllC;
  const static ContinuousStrategy AllD;
  static ContinuousStrategy ConstructFromEdgeValues(const std::array<double,4>& betas, const std::array<double,8>& alphas) {
    // betas[0] = beta_00, betas[1] = beta_01, ... , beta[3] = beta_11
    action_rule_t beta = [betas](double x, double y) {
      return (1.0-x)*( betas[0b01]*y + betas[0b00]*(1.0-y) ) + x*( betas[0b11]*y + betas[0b10]*(1.0-y) );
    };
    // alphas[0] = alpha_000, alphas[1] = alpha_001, ... , alphas[7] = alpha_111
    assessment_rule_t alpha = [alphas](double x, double y, double z) {
      double z0 = (1.0-x)*( alphas[0b010]*y + alphas[0b000]*(1.0-y) ) + x*( alphas[0b110]*y + alphas[0b100]*(1.0-y) );
      double z1 = (1.0-x)*( alphas[0b011]*y + alphas[0b001]*(1.0-y) ) + x*( alphas[0b111]*y + alphas[0b101]*(1.0-y) );
      return (1.0-z)*z0 + z*z1;
    };
    return ContinuousStrategy(alpha, beta);
  }
  static ContinuousStrategy ConstructFromBinaryStrategy(const BinaryStrategy& str) {
    // construct action rule
    // beta(x,y) = (1-x)[ b01 y + b00 (1-y) ] + x[ b11 y + b10 (1-y) ]
    //           = (b11-b10-b01+b00) xy + (b10-b00)x + (b01-b00)y + b00
    std::array<double,4> betas;
    betas[0b00] = str.Act(0,0);
    betas[0b01] = str.Act(0,1);
    betas[0b10] = str.Act(1,0);
    betas[0b11] = str.Act(1,1);
    // construct assessment rule
    std::array<double,8> alphas;
    alphas[0b000] = str.Assess(0,0,0);
    alphas[0b001] = str.Assess(0,0,1);
    alphas[0b010] = str.Assess(0,1,0);
    alphas[0b011] = str.Assess(0,1,1);
    alphas[0b100] = str.Assess(1,0,0);
    alphas[0b101] = str.Assess(1,0,1);
    alphas[0b110] = str.Assess(1,1,0);
    alphas[0b111] = str.Assess(1,1,1);
    return ConstructFromEdgeValues(betas, alphas);
  }

};

const ContinuousStrategy ContinuousStrategy::AllC = ContinuousStrategy([](double a, double d, double r) { return 1.0; }, [](double d, double r) {return 1.0; });
const ContinuousStrategy ContinuousStrategy::AllD = ContinuousStrategy([](double a, double d, double r) { return 0.0; }, [](double d, double r) {return 0.0; });

#endif // CONTINUOUS_STRATEGY_HPP