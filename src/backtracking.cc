#include "backtracking.hh"

void Backtracking::solve(Game& g)
{
  Backtracking solver(g);
  solver.btc_search_();
}

Backtracking::Backtracking(Game& game)
  : game_(game)
{}

bool Backtracking::btc_search_()
{
  if (game_.is_complete())
    return true;

  pos_t var = select_var_();
  unsigned i = var.first;
  unsigned j = var.second;

  std::set<int> values = find_values_(i, j);
  for (auto value : values)
    {
      game_.cell_set(i, j, value);
      bool res = btc_search_();
      if (res)
        return true;
      game_.cell_clear(i, j);
    }

  return false;
}

Backtracking::pos_t Backtracking::select_var_()
{
  for (unsigned i = 0; i < 9; ++i)
    for (unsigned j = 0; j < 9; ++j)
      if (game_.cell_get(i, j) == Game::EMPTY)
        return pos_t{i, j};

  return pos_t{9, 9};
}

std::set<int> Backtracking::find_values_(unsigned i, unsigned j)
{
  std::set<int> res {1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (auto x : game_.line_at(i, j))
    res.erase(x);
  for (auto x : game_.column_at(i, j))
    res.erase(x);
  for (auto x : game_.region_at(i, j))
    res.erase(x);

  return res;
}
