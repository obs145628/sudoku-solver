#pragma once

#include "game.hh"
#include <set>
#include <vector>
#include <utility>

class ForwardChecking
{
public:
  static void solve(Game& g, bool arc, bool mrv, bool lcv);


private:

  struct Cell
  {
    unsigned i;
    unsigned j;
    std::set<int> vals;
  };

  ForwardChecking(Game& game, bool arc, bool mrv, bool lcv);
  
  Game& game_;
  bool arc_;
  bool mrv_;
  bool lcv_;
  std::vector<Cell> cells_;

  Cell compute_cell_(unsigned i, unsigned j);
  void compute_cells_();

  bool btc_search_();
  std::size_t select_var_();

  bool check_(unsigned i, unsigned j, int val);

  std::vector<int> get_values_(std::size_t pos);
  int check_lcv_(unsigned i, unsigned j, int val);
};
