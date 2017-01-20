#include "simulated-annealing.hh"
#include <cmath>

void SimulatedAnnealing::solve(Game& g)
{
  SimulatedAnnealing solver(g);
  solver.exec_();
}


SimulatedAnnealing::SimulatedAnnealing(Game& g)
  : LocalSearch(g)
{}

void SimulatedAnnealing::exec_()
{
  init_();
  if (vals_.empty())
    return;

  unsigned err = count_errors_();

  unsigned i = 0;
  while (true)
    {
      double t = temperature_(i++);
      if (t == 0)
        break;

      auto next = next_();
      unsigned next_err = count_errors_(next.first, next.second);

      if (next_err >= err)
        {
          double delta = next_err - err;
          double proba = std::exp( - delta / t);
          if (rand_.double_get() > proba)
            continue;
        }

      err = next_err;
      apply_swap_(next.first, next.second);
      if (err == 0)
        break;
    }
}


double SimulatedAnnealing::temperature_(unsigned i)
{
  double t = 10 - std::sqrt(std::sqrt(i)) * 0.5;
  if (t < 0)
    t = 0;
  return t;
}

SimulatedAnnealing::successor_t
SimulatedAnnealing::next_()
{
  unsigned a = rand_.int32_get() % pos_.size();
  unsigned b = rand_.int32_get() % pos_.size();
  while (a == b)
    b = rand_.int32_get() % pos_.size();
  return {a, b};
}
