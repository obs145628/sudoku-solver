#pragma once

#include "ai/misc/random.hh"
#include "game.hh"

class LocalSearch
{
public:
  LocalSearch(Game& game);

protected:
  static Random rand_;

  Game& game_;
  std::vector<unsigned> pos_;
  std::vector<int> vals_;

  void init_();
  void build_pos_();
  void build_vals_();
  void apply_vals_();
  void apply_swap_(unsigned a, unsigned b);

  std::vector<int> build_grid_(unsigned a = 0, unsigned b = 0);
  unsigned count_errors_(unsigned a = 0, unsigned b = 0);
};
