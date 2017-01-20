#pragma once

#include "local-search.hh"

class HillClimbing : public LocalSearch
{
public:
  static void solve(Game& g);

private:
  HillClimbing(Game& g);
  void exec_();
};
