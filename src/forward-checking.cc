#include "forward-checking.hh"
#include <algorithm>
#include <utility>

void ForwardChecking::solve(Game& g, bool arc, bool mrv, bool lcv)
{
  ForwardChecking solver(g, arc, mrv, lcv);
  solver.btc_search_();
}

ForwardChecking::ForwardChecking(Game& game, bool arc, bool mrv, bool lcv)
  : game_(game)
  , arc_(arc)
  , mrv_(mrv)
  , lcv_(lcv)
{
  compute_cells_();
}

ForwardChecking::Cell ForwardChecking::compute_cell_(unsigned i, unsigned j)
{
  Cell res;
  res.i = i;
  res.j = j;
  res.vals = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (auto x : game_.line_at(i, j))
    res.vals.erase(x);
  for (auto x : game_.column_at(i, j))
    res.vals.erase(x);
  for (auto x : game_.region_at(i, j))
    res.vals.erase(x);

  return res;
}

void ForwardChecking::compute_cells_()
{
  for (unsigned i = 0; i < 9; ++i)
    for (unsigned j = 0; j < 9; ++j)
      if (game_.cell_get(i, j) == Game::EMPTY)
        cells_.push_back(compute_cell_(i, j));
}

bool ForwardChecking::btc_search_()
{
  if (cells_.empty())
    return true;

  std::size_t pos = select_var_();
  Cell var = cells_[pos];
  std::vector<int> values = get_values_(pos);


  for (auto value : values)
    {
      auto copy = cells_;
      game_.cell_set(var.i, var.j, value);

      if (check_(var.i, var.j, value)
          && btc_search_())
        return true;
      
      game_.cell_clear(var.i, var.j);
      cells_ = copy;
    }

  return false;
}

std::size_t ForwardChecking::select_var_()
{
  if (!mrv_)
    return 0;

  std::size_t res = 0;
  for (std::size_t i = 1; i < cells_.size(); ++i)
    if (cells_[i].vals.size() < cells_[res].vals.size())
      res = i;

  return res;
}

bool ForwardChecking::check_(unsigned i, unsigned j, int val)
{
  for (auto& c : cells_)
    {
      if (c.i == i && c.j == j)
        continue;
      
      if(c.i == i || c.j == j
         || ((c.i - c.i % 3 == i - i % 3)
             && (c.j - c.j % 3 == j - j % 3)))
        {
          auto it = c.vals.find(val);
          if (it != c.vals.end())
            {
              c.vals.erase(it);
              if (c.vals.empty())
                return false;

              if (c.vals.size() == 1 && arc_
                  && !check_(c.i, c.j, *c.vals.begin()))
                return false;
            }
        }
    }

  return true;
}

std::vector<int> ForwardChecking::get_values_(std::size_t pos)
{
  Cell cell = cells_[pos];
  cells_.erase(cells_.begin() + pos);

  if (!lcv_)
    return std::vector<int>(cell.vals.begin(), cell.vals.end());

  using elem_t = std::pair<int, int>;

  std::vector<elem_t> elems;

  for (auto value : cell.vals)
    {
      auto copy = cells_;
      int nb = check_lcv_(cell.i, cell.j, value);
      cells_ = copy;

      if (nb != -1)
        elems.emplace_back(nb, value);
    }

  std::sort(elems.begin(), elems.end(), [](const elem_t& a, const elem_t& b)
            {
              return a.first < b.first;
            });
  

  std::vector<int> res;
  for (const auto& elem : elems)
    res.push_back(elem.second);
  return res;
}

int ForwardChecking::check_lcv_(unsigned i, unsigned j, int val)
{
  int res = 0;
  
  for (auto& c : cells_)
    {
      if (c.i == i && c.j == j)
        continue;
      
      if(c.i == i || c.j == j
         || ((c.i - c.i % 3 == i - i % 3)
             && (c.j - c.j % 3 == j - j % 3)))
        {
          auto it = c.vals.find(val);
          if (it != c.vals.end())
            {
              c.vals.erase(it);
              ++res;
              if (c.vals.empty())
                return -1;

              if (c.vals.size() == 1 && arc_)
                {
                  int sub = check_(c.i, c.j, *c.vals.begin());
                  if (sub == -1)
                    return -1;
                  res += sub; 
                }
            }
        }
    }

  return res;
}
