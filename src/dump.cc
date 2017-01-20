#include "dump.hh"

#include <set>
#include <utility>
#include <vector>
#include "ai/math/random.hh"

#include <iostream>

struct Cell
{
  unsigned i;
  unsigned j;
  std::set<int> vals;
};

class Solver
{

public:
  Solver(Game& g)
    : g_(g)
  {
    compute_cells_();
  }

  void solve()
  {
    while (!cells_.empty())
      {
        auto pos = get_min_();
        Cell c = cells_[pos];
        std::vector<int> vals (c.vals.begin(), c.vals.end());
        if (vals.empty())
          break;

        int x = vals.size() == 1 ? vals[0]
          : vals[rand_.int32_get() % vals.size()];
        g_.cell_set(c.i, c.j, x);

        cells_.erase(cells_.begin() + pos);
        apply_res_(c.i, c.j, x);
      }
  }


private:
  Game& g_;
  Random rand_;
  std::vector<Cell> cells_;

  Cell compute_cell_(unsigned i, unsigned j)
  {
    Cell res;
    res.i = i;
    res.j = j;
    res.vals = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto line = g_.line_at(i, j);
    for (auto x : line)
      res.vals.erase(x);

    auto col = g_.column_at(i, j);
    for (auto x : col)
      res.vals.erase(x);

    auto region = g_.region_at(i, j);
    for (auto x : region)
      res.vals.erase(x);

    return res;
  }

  void compute_cells_()
  {
    for (unsigned i = 0; i < 9; ++i)
      for (unsigned j = 0; j < 9; ++j)
        if (g_.cell_get(i, j) == Game::EMPTY)
          cells_.push_back(compute_cell_(i, j));
  }

  std::size_t get_min_() const
  {
    unsigned res = 0;
    for (unsigned i = 1; i < cells_.size(); ++i)
      if (cells_[i].vals.size() < cells_[res].vals.size())
        res = i;
    return res;
  }

  void apply_res_(unsigned i, unsigned j, int val)
  {
    for (auto& c : cells_)
      {
        if(c.i == i || c.j == j
           || ((c.i - c.i % 3 == i - i % 3)
               && (c.j - c.j % 3 == j - j % 3)))
          c.vals.erase(val);
      }
  }
};

void Dump::solve(Game& g)
{
  Solver solver(g);
  solver.solve();
}
