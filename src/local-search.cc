#include "local-search.hh"
#include <cassert>
#include <utility>

Random LocalSearch::rand_ {};

LocalSearch::LocalSearch(Game& game)
  : game_(game)
{}

void LocalSearch::init_()
{
  build_pos_();
  build_vals_();
  apply_vals_();
}

void LocalSearch::build_pos_()
{
  for (unsigned i = 0; i < 9; ++i)
    for (unsigned j = 0; j < 9; ++j)
      if (game_.cell_get(i, j) == Game::EMPTY)
        pos_.push_back(i * 9 + j);
}

void LocalSearch::build_vals_()
{
  std::vector<int> freqs(10);
  for (unsigned i = 0; i < 9; ++i)
    for (unsigned j = 0; j < 9; ++j)
      if (game_.cell_get(i, j) != Game::EMPTY)
        ++freqs[game_.cell_get(i, j)];

  for (int i = 1; i < 10; ++i)
    {
      int freq = 9 - freqs[i];
      for (int j = 0; j < freq; ++j)
        vals_.push_back(i);
    }

  auto n = vals_.size();
  for (auto i = n; i > 0; --i)
    {
      auto j = rand_.int32_get() % i;
      std::swap(vals_[i - 1], vals_[j]);
    }
}

void LocalSearch::apply_vals_()
{
  for (unsigned k = 0; k < vals_.size(); ++k)
    {
      unsigned pos = pos_[k];
      unsigned i = pos / 9;
      unsigned j = pos % 9;
      game_.cell_set(i, j, vals_[k]);
    }
}

void LocalSearch::apply_swap_(unsigned a, unsigned b)
{
  int vala = vals_[a];
  int valb = vals_[b];
  unsigned posa = pos_[a];
  unsigned posb = pos_[b];

  vals_[a] = valb;
  vals_[b] = vala;

  unsigned ai = posa / 9;
  unsigned aj = posa % 9;
  unsigned bi = posb / 9;
  unsigned bj = posb % 9;
  game_.cell_clear(ai, aj);
  game_.cell_clear(bi, bj);
  game_.cell_set(ai, aj, valb);
  game_.cell_set(bi, bj, vala);
}

std::vector<int> LocalSearch::build_grid_(unsigned a, unsigned b)
{
  std::vector<int> res;
  unsigned posa = pos_.size() ? pos_[a] : -1;
  unsigned posb = pos_.size() ? pos_[b] : -1;

  for (unsigned k = 0; k < 81; ++k)
    {
      unsigned i = k / 9;
      unsigned j = k % 9;

      if (k == posa)
        res.push_back(vals_[b]);
      else if (k == posb)
        res.push_back(vals_[a]);
      else
        res.push_back(game_.cell_get(i, j));
    }

  return res;
}

unsigned LocalSearch::count_errors_(unsigned a, unsigned b)
{
  auto grid = build_grid_(a, b);
  unsigned res = 0;

  for (unsigned i = 0; i < 9; ++i)
    for (unsigned j = 0; j < 9; ++j)
      {
        int val = grid[i * 9 + j];

        for (unsigned k = 0; k < 9; ++k)
          {
            if (k == i)
              continue;
            int val2 = grid[k * 9 + j];
            if (val == val2)
              ++res;
          }

        for (unsigned k = 0; k < 9; ++k)
          {
            if (k == j)
              continue;
            int val2 = grid[i * 9 + k];
            if (val == val2)
              ++res;
          }

        unsigned i0 = i - i % 3;
        unsigned j0 = j - j % 3;
        for (unsigned r = i0; r < i0 + 3; ++r)
          for (unsigned c = j0; c < j0 + 3; ++c)
            {
              if (r == i && c == j)
                continue;
              int val2 = grid[r * 9 + c];
              if (val == val2)
                ++res;
            }
      }

  return res;
}
