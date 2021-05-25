# indirect reciprocity with continuous reputation model

The source code for the article *[Sanghun Lee, Yohsuke Murase, Seung Ki Baek "Local stability of cooperation in a continuous model of indirect reciprocity"](https://arxiv.org/abs/2104.02881)*.

## How to build

- Clone the repository with its submdules
    - `git clone --recursive git@github.com:yohm/sim_game_continuous_reputation.git`
- build the executables using cmake.
    - `mkdir build; cd build; cmake ..; make`
    - Or you can compile directly `g++ --std=c++14 -Iicecream main_continuous_coop_prob.cpp -o main_continuous_coop_prob.out`

## How to run

```shell
./main_continuous_coop_prob.out
```
