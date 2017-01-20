#pragma once

#include "local-search.hh"
#include <utility>

class SimulatedAnnealing : public LocalSearch
{
public:
  using successor_t = std::pair<unsigned, unsigned>;
  static void solve(Game& g);

private:
  SimulatedAnnealing(Game& g);
  void exec_();

  double temperature_(unsigned i);

  successor_t next_();
};
