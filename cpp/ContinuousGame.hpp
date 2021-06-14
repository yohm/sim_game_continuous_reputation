#ifndef CONTINUOUS_GAME_HPP
#define CONTINUOUS_GAME_HPP

#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <random>
#include <iomanip>
#include "ContinuousStrategy.hpp"

class ContinuousGame {
  public:
  using population_t = std::vector<std::pair<ContinuousStrategy, size_t>>;  // vector of StrategyID & its size

  ContinuousGame(const population_t& pop, uint64_t _seed) : population(pop), rnd(_seed), uni(0.0, 1.0) {
    for (size_t i = 0; i < population.size(); i++) {
      auto kv = population[i];
      for (size_t ii = 0; ii < kv.second; ii++) {
        strategies.emplace_back(kv.first);
        strategy_index.emplace_back(i);
      }
    }
    N = strategies.size();
    M.resize(N);
    for (size_t i = 0; i < N; i++) { M[i].resize(N, 1.0); }
    ResetCoopCount();
  }

  void Update(size_t t_max, double q, double epsilon, double impl_error_prob = 0.1) {
    for (size_t t = 0; t < t_max; t++) {
      // randomly choose donor & recipient
      size_t donor = static_cast<size_t>(R01() * N);
      size_t recip = static_cast<size_t>(R01() * N);
      while (recip == donor) { recip = static_cast<size_t>(R01() * N); }

      double action = strategies[donor].Act( M[donor][donor], M[donor][recip] );
      // implementation error
      if(R01() < impl_error_prob) action = R01();

      auto key = std::make_pair(strategy_index[donor], strategy_index[recip]);
      coop_count[key][0] += action;
      coop_count[key][1] += 1.0;

      // updating the donor's reputation
      for (size_t obs = 0; obs < N; obs++) {
        if (obs == donor || obs == recip || R01() < q) {  // observe with probability q
          double a_obs = (R01() < 1.0 - epsilon) ? action : R01();
          double new_rep = strategies[obs].Assess(a_obs, M[obs][donor], M[obs][recip]);
          M[obs][donor] = new_rep;
        }
      }
    }
  }

  using coop_count_t = std::map< std::pair<size_t,size_t>, std::array<double,2> >;
  using good_count_t = std::map< std::pair<size_t,size_t>, double>;

  void CountUpGoodRepSpeciesWise(good_count_t& ans) const {
    for (size_t i = 0; i < N; i++) {
      size_t str_i = strategy_index[i];
      for (size_t j = 0; j < N; j++) {
        size_t str_j = strategy_index[j];
        ans[{str_i, str_j}] += M[i][j];
      }
    }
  }

  // key : pair of [donor_idx, recipient_idx]
  // value: [number of cooperation] / [number of actions (cooperation + defection)
  coop_count_t GetCoopCount() const { return coop_count; }

  void ResetCoopCount() {
    coop_count.clear();
    size_t N_species = population.size();
    for (size_t i = 0; i < N_species; i++) {
      for (size_t j = 0; j < N_species; j++) {
        coop_count[{i,j}] = {0.0, 0.0};
      }
    }
  }

  void PrintM(std::ostream& out = std::cerr) const {
    out << std::setprecision(3);
    for (size_t i = 0; i < M.size(); i++) {
      for (size_t j = 0; j < M[i].size(); j++) {
        out << M[i][j] << ' ';
      }
      out << "\n";
    }
  }

  private:
  const population_t population;
  size_t N;
  std::mt19937_64 rnd;
  std::uniform_real_distribution<double> uni;
  std::vector<ContinuousStrategy> strategies;
  std::vector<size_t> strategy_index;  // [0,0,...,0, 1,1,....,1, 2,2,......,2]
  std::vector<std::vector<double> > M;
  coop_count_t coop_count;
  double R01() { return uni(rnd); }
};

#endif  // CONTINUOUS_GAME_HPP
