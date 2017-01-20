#pragma once

#include <string>
#include <vector>
#include "ai/shell/grid.hh"
#include "ai/shell/shell-sprite.hh"
#include "ai/shell/shell-canvas.hh"

class Game
{
public:
  static const int EMPTY;

  Game();
  Game(const std::vector<int>& data);

  static Game load(const std::string& path);

  int cell_get(unsigned i, unsigned j) const;
  void cell_set(unsigned i, unsigned j, int val);
  void cell_clear(unsigned i, unsigned j);

  std::vector<int> line_at(unsigned i, unsigned j) const;
  std::vector<int> column_at(unsigned i, unsigned j) const;
  std::vector<int> region_at(unsigned i, unsigned j) const;

  bool is_complete() const;
  std::size_t writes_get() const;
  std::size_t clears_get() const;
  ShellSprite to_sprite() const;

  void canvas_set(ShellCanvas* cvs);
  
private:
  std::vector<int> data_;
  std::vector<int> base_;
  std::size_t writes_;
  std::size_t clears_;
  ShellCanvas* cvs_;

  void display_();
};
