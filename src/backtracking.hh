#pragma once

#include "game.hh"
#include <set>
#include <utility>

class Backtracking
{
public:
  using pos_t = std::pair<unsigned, unsigned>;
  static void solve(Game& g);

private:
  Game& game_;

  Backtracking(Game& game);
  bool btc_search_();
  pos_t select_var_();

  std::set<int> find_values_(unsigned i, unsigned j);


};
