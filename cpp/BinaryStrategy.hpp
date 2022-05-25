#ifndef BINARY_STRATEGY_HPP
#define BINARY_STRATEGY_HPP

#include <iostream>
#include <cstdint>

// Cooperate : 1, Defect : 0
// Good : 1, Bad : 0

class AssessmentRule {
  public:
  AssessmentRule(uint64_t _id = 0) : id(_id) { assert(id >= 0 && id < 256); };
  uint64_t Assess(uint64_t action, uint64_t rep_d, uint64_t rep_r) const {
    assert(rep_d == 0 || rep_d == 1);
    assert(rep_r == 0 || rep_r == 1);
    assert(action == 0 || action == 1);
    uint64_t i = action * 4 + rep_d * 2 + rep_r;
    return (id >> i) & 1ul;
  }
  uint64_t id;
};


class ActionRule {
  public:
  ActionRule(uint64_t _id = 0) : id(_id) { assert(id >= 0 && id < 16); }
  uint64_t Act(uint64_t rep_d, uint64_t rep_r) const {
    assert(rep_d == 0 || rep_d == 1);
    assert(rep_r == 0 || rep_r == 1);
    uint64_t i = rep_d * 2 + rep_r;
    return (id >> i) & 1ul;
  }
  uint64_t id;
};


class BinaryStrategy {
  public:
  BinaryStrategy(uint64_t _id = 0) : id(_id), assess(id>>4), action(id&15) { assert(id >= 0 && id < 4096); }
  uint64_t id;
  AssessmentRule assess;
  ActionRule action;
  uint64_t Act(uint64_t donor_rep, uint64_t recip_rep) const { return action.Act(donor_rep, recip_rep); }
  uint64_t Assess(uint64_t action, uint64_t donor_rep, uint64_t recip_rep) const { return assess.Assess(action, donor_rep, recip_rep); }
  constexpr static uint64_t IS = 0b11110000'1010;
  constexpr static uint64_t L1_id = 0b11110100'1011;
  constexpr static uint64_t L2_id = 0b10110100'1011;
  constexpr static uint64_t L3_id = 0b11110101'1010;
  constexpr static uint64_t L4_id = 0b11100101'1010;
  constexpr static uint64_t L5_id = 0b10110101'1010;
  constexpr static uint64_t L6_id = 0b10100101'1010;
  constexpr static uint64_t L7_id = 0b11100100'1010;
  constexpr static uint64_t L8_id = 0b10100100'1010;
  constexpr static uint64_t alld_id = 0b00000000'0000;
  constexpr static uint64_t allc_id = 0b11111111'1111;
};

#endif // STRATEGY_HPP