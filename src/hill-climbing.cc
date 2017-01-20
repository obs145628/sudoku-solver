#include "hill-climbing.hh"
#include <cassert>

void HillClimbing::solve(Game& g)
{
  HillClimbing solver(g);
  solver.exec_();
}


HillClimbing::HillClimbing(Game& g)
  : LocalSearch(g)
{}

void HillClimbing::exec_()
{
  init_();
  if (vals_.empty())
    return;
  unsigned err = count_errors_();

  while (true)
    {
      unsigned min_a = 0;
      unsigned min_b = 0;
      unsigned min_err = -1;

      for (unsigned a = 0; a < vals_.size(); ++a)
        for (unsigned b = a + 1; b < vals_.size(); ++b)
          {
            unsigned err = count_errors_(a, b);
            if (err < min_err)
              {
                min_a = a;
                min_b = b;
                min_err = err;
              }
          }
      
      if (min_err >= err)
        break;

      apply_swap_(min_a, min_b);
      err = min_err;
      if (err == 0)
        break;
    }
}
