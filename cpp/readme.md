## How to build

- Clone the repository with its submdules
    - `git clone --recursive git@github.com:yohm/sim_game_continuous_reputation.git`
- build the executables using cmake.
    - `cd cpp; mkdir build; cd build; cmake ..; make`
    - Or you can compile directly `g++ --std=c++14 -Iicecream main_continuous_coop_prob.cpp -o main_continuous_coop_prob.out`

## How to run

```shell
./main_continuous_coop_prob.out
```

