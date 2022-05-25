#include <iostream>
#include <fstream>
#include <random>
#include <array>
#include <map>
#include <cstdint>
#include <icecream.hpp>
#include "ContinuousStrategy.hpp"
#include "ContinuousGame.hpp"

void Simulate(const ContinuousStrategy& strategy, std::ostream* snapout = nullptr) {
  const size_t N = 90;
  const size_t t_init = N * 1'000;
  const double q = 0.9;
  const double epsilon = 0.05;
  ContinuousStrategy all_c = ContinuousStrategy::AllC, all_d = ContinuousStrategy::AllD;
  // ContinuousGame::population_t population = {{strategy, N/3}, {all_c, N/3}, {all_d, N/3} };  // map of StrategyID & its size
  ContinuousGame::population_t population = {{strategy, N}};  // map of StrategyID & its size
  ContinuousGame g(population, 1234ul);
  g.Update(t_init, q, epsilon);
  g.ResetCoopCount();

  ContinuousGame::good_count_t g_count;
  const size_t N_measure = 1000, delta_t = 100;
  for (size_t i = 0; i < N_measure; i++) {
    g.Update(delta_t, q, epsilon);
    g.CountUpGoodRepSpeciesWise(g_count);
  }

  auto coop_count = g.GetCoopCount();
  IC(coop_count, g_count);

  if (snapout) {
    g.PrintM(*snapout);
  }
}

void SimulateRelaxation(const ContinuousStrategy& strategy, uint64_t seed) {
  const size_t N = 100;
  const double q = 1.0;
  const double m_low = 0.9;
  ContinuousGame::population_t population = {{strategy, N}};  // map of StrategyID & its size

  const size_t N_sample = 1, t = 100;
  for (size_t i = 0; i < N_sample; i++) {
    ContinuousGame g(population, seed + i);
    g.RandomizeM(std::uniform_real_distribution<double>(m_low, 1.0));
    auto ans = g.UpdateWithoutError(t, q);

    for (size_t t = 0; t < ans.size(); t++) {
      std::cout << ans[t] << std::endl;
    }
  }
}

void RandomInvaders(const ContinuousStrategy& resident, long n_trials, uint64_t seed) {
  const size_t N = 100;
  const size_t N_res = N * 0.9, N_mut = N * 0.1;
  const size_t t_init = 1'000, t_measure = 1'000;
  const double q = 0.4;
  const double epsilon = 0.1;
  const double b = 2.0, c = 1.0;

  std::mt19937_64 rnd(seed);
  std::uniform_real_distribution<double> uni(0.0, 1.0);
  for (size_t n = 0; n < n_trials; n++) {
    std::array<double,4> betas;
    std::array<double,8> alphas;
    for (int i = 0; i < 4; i++) { betas[i] = uni(rnd); }
    for (int i = 0; i < 8; i++) { alphas[i] = uni(rnd); }
    betas[0b00] = 0;
    betas[0b01] = 1;
    betas[0b10] = 0;
    betas[0b11] = 1;
    alphas[0b111] = 1;
    ContinuousStrategy mutant = ContinuousStrategy::ConstructFromEdgeValues(betas, alphas);

    ContinuousGame::population_t population = {{resident, N_res}, {mutant, N_mut}};  // map of StrategyID & its size
    ContinuousGame g(population, rnd());
    g.Update(t_init * N, q, epsilon);
    g.ResetCoopCount();

    g.Update(t_measure * N, q, epsilon);

    auto coop_count = g.GetCoopCount();
    // IC(coop_count);
    double payoff_res = (
       b * (coop_count.at({0,0})[0] + coop_count.at({1,0})[0])
      -c * (coop_count.at({0,0})[0] + coop_count.at({0,1})[0])
    ) / (N_res * t_measure);
    double payoff_mut = (
       b * (coop_count.at({0,1})[0] + coop_count.at({1,1})[0])
      -c * (coop_count.at({1,0})[0] + coop_count.at({1,1})[0])
    ) / (N_mut * t_measure);
    std::cout << alphas[0] << ' ' << alphas[1] << ' ' << alphas[2] << ' ' << alphas[3] << ' ' << alphas[4] << ' ' << alphas[5] << ' ' << alphas[6] << ' ' << alphas[7] << ' ';
    std::cout << betas[0] << ' ' << betas[1] << ' ' << betas[2] << ' ' << betas[3] << ' ';
    std::cout << payoff_res << ' ' << payoff_mut << ' ' << std::endl;
    // if (payoff_mut > payoff_res) { IC(payoff_res, payoff_mut, betas, alphas); }
  }
}

int main(int argc, char* argv[]) {
  // assessment_rule_t assess_rule = [](double act, double donor, double recip) {
  //   return 0.5 * donor * (1.0-recip) + std::min(act, recip);
  // };
  // action_rule_t act_rule = [](double donor, double recip) { return recip; };
  // ContinuousStrategy str(assess_rule, act_rule);
  // Simulate(str, &std::cout);

  BinaryStrategy l3(BinaryStrategy::L3_id);
  ContinuousStrategy l3_cont = ContinuousStrategy::ConstructFromBinaryStrategy(l3);
  SimulateRelaxation(l3_cont, 123456789ull);
  // Simulate(l3_cont);
  // RandomInvaders(l3_cont, 100, 1234ull);

  return 0;
}
