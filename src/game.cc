#include "game.hh"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <unistd.h>
#include "ai/shell/shell.hh"

const int Game::EMPTY= -1;

Game Game::load(const std::string& path)
{
  std::vector<int> data;
  std::ifstream is(path);
  assert(is.is_open());

  for (unsigned i = 0; i < 9; ++i)
    {
      for (unsigned j = 0; j < 9; ++j)
        {
          char c = is.get();
          data.push_back(c == '.' ? EMPTY : c - '0');
          if (j == 2 || j == 5)
            is.get();
        }
      is.get();
      if (i == 2 || i == 5)
        for (int k = 0; k < 12; ++k)
          is.get();
    }

  return Game(data);
}

Game::Game()
  : Game(std::vector<int>(81, EMPTY))
{}

Game::Game(const std::vector<int>& data)
  : data_(data)
  , base_(81)
  , writes_(0)
  , clears_(0)
  , cvs_(nullptr)
{
  assert(data.size() == 81);
  for (std::size_t i = 0; i < 81; ++i)
    {
      int n = data[i];
      assert(n == EMPTY || (n >= 1 && n <= 9));
      base_[i] = n != EMPTY;
    }
}

int Game::cell_get(unsigned i, unsigned j) const
{
  assert(i < 9);
  assert(j < 9);
  return data_[i * 9 + j];
}
void Game::cell_set(unsigned i, unsigned j, int val)
{
  assert(i < 9);
  assert(j < 9);
  assert(val >= 1 && val <= 9);
  assert(data_[i * 9 + j] == EMPTY);
  data_[i * 9 + j] = val;
  ++writes_;
  if (cvs_)
    display_();
}
void Game::cell_clear(unsigned i, unsigned j)
{
  assert(i < 9);
  assert(j < 9);
  assert(data_[i * 9 + j] != EMPTY);
  data_[i * 9 + j] = EMPTY;
  ++clears_;
  if (cvs_)
    display_();
}

ShellSprite Game::to_sprite() const
{
  std::size_t width = 19;
  std::size_t height = 19;
  ShellSprite::data_t data;

  for (unsigned i = 0; i <= 9; ++i)
    {
      const char* bg = i == 3 || i == 6 ? Shell::FG_RED
        : Shell::FG_BLUE;
        data.emplace_back('+', bg);
      for (std::size_t j = 0; j < 9; ++j)
        {
          const char* bg = i == 3 || i == 6 || j == 2 || j == 5
            ? Shell::FG_RED : Shell::FG_BLUE;
          data.emplace_back('-', bg);
          data.emplace_back('+', bg);
        }

      if (i == 9)
        break;

      data.emplace_back('|', Shell::FG_BLUE);
      for (std::size_t j = 0; j < 9; ++j)
        {
          int val = cell_get(i, j);
          const char* fg = base_[i * 9 + j]
            ? Shell::FG_DEFAULT : Shell::FG_YELLOW;
          char c = val == EMPTY ? ' ' : val + '0';
          data.emplace_back(c, fg);
          const char* bg = j == 2 || j == 5 ? Shell::FG_RED
            : Shell::FG_BLUE;
          data.emplace_back('|', bg);
        }
    }

  return ShellSprite{width, height, data};
}

std::vector<int> Game::line_at(unsigned i, unsigned j) const
{
  assert(i < 9);
  assert(j < 9);
  std::vector<int> res;

  for (unsigned k = 0; k < 9; ++k)
    if (k != j)
      res.push_back(cell_get(i, k));
  return res;
}

std::vector<int> Game::column_at(unsigned i, unsigned j) const
{
  assert(i < 9);
  assert(j < 9);
  std::vector<int> res;

  for (unsigned k = 0; k < 9; ++k)
    if (k != i)
      res.push_back(cell_get(k, j));
  return res;
}

std::vector<int> Game::region_at(unsigned i, unsigned j) const
{
  assert(i < 9);
  assert(j < 9);
  std::vector<int> res;

  unsigned i0 = i - i % 3;
  unsigned j0 = j - j % 3;

  for (unsigned r = i0; r < i0 + 3; ++r)
    for (unsigned c = j0; c < j0 + 3; ++c)
      if (r != i || c != j)
        res.push_back(cell_get(r, c));
  return res;
}


bool Game::is_complete() const
{
  for (unsigned i = 0; i < 9; ++i)
    for (unsigned j = 0; j < 9; ++j)
      {
        int val = cell_get(i, j);
        if(val == EMPTY)
          return false;
        auto line = line_at(i, j);
        auto col = column_at(i, j);
        auto region = region_at(i, j);
        if (std::find(line.begin(), line.end(), val) != line.end())
          return false;
        if (std::find(col.begin(), col.end(), val) != col.end())
          return false;
        if (std::find(region.begin(), region.end(), val) != region.end())
          return false;
      }

  return true;
}

std::size_t Game::writes_get() const
{
  return writes_;
}

std::size_t Game::clears_get() const
{
  return clears_;
}

void Game::canvas_set(ShellCanvas* cvs)
{
  cvs_ = cvs;
}



void Game::display_()
{
  cvs_->clear();
  cvs_->draw_sprite(0, 0, to_sprite());
  cvs_->render();
  usleep(50 * 1e3);
}
