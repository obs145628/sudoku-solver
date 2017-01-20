#include <algorithm>
#include <iostream>
#include "ai/io/dir.hh"
#include "ai/math/random.hh"
#include "ai/shell/shell.hh"
#include "ai/shell/shell-canvas.hh"
#include "ai/shell/timer.hh"
#include "backtracking.hh"
#include "dump.hh"
#include "forward-checking.hh"
#include "game.hh"
#include "hill-climbing.hh"
#include "simulated-annealing.hh"

Game load_game(const std::string& type)
{
  static Random rand;
  auto files = Dir::list_files("../grids/" + type);
  auto name = files[rand.int32_get() % files.size()];
  auto path = "../grids/" + type + "/" + name;
  return Game::load(path);
}

template <class F>
static void run(const std::string& name,
                const std::vector<Game>& games, F f)
{
  std::size_t n = games.size();
  double dn = n;
  std::size_t succ = 0;
  std::size_t writes = 0;
  std::size_t clears = 0;
  Timer clock;
  for (Game g : games)
    {
      f(g);
      succ += g.is_complete();
      writes += g.writes_get();
      clears += g.clears_get();
    }
  auto time = clock.get();

  double av_writes = writes / dn;
  double av_clears = clears / dn;
  
  std::cout << "----------------------------------------\n";
  std::cout << "Algorithm: " << name << "\n";
  std::cout << "  Success: " << succ << "/" << n << "\n";
  std::cout << "   Writes: " << av_writes << "\n";
  std::cout << "   Clears: " << av_clears << "\n";
  std::cout << "     Time: " << time << "ms\n";
  std::cout << "----------------------------------------\n\n";
}

int bench(const std::string& type, int argc, char** argv)
{
  int n = atoi(argv[argc - 1]);
  std::vector<Game> games;
  for (int i = 0; i < n; ++i)
    games.push_back(load_game(type));

  std::vector<std::string> algos;
  for (int i = 3; i < argc - 1; ++i)
    algos.push_back(argv[i]);

               
  if (std::find(algos.begin(), algos.end(), "dump") != algos.end())
    run("dump", games, [](Game& g2)
        {
          Dump::solve(g2);
        });

  if (std::find(algos.begin(), algos.end(), "btk") != algos.end())
    run("Backtracking", games, [](Game& g2)
        {
          Backtracking::solve(g2);
        });


  if (std::find(algos.begin(), algos.end(), "fc") != algos.end())
    run("Forward Checking", games, [](Game& g2)
        {
          ForwardChecking::solve(g2, false, false, false);
        });

  if (std::find(algos.begin(), algos.end(), "arc") != algos.end())
    run("Arc consistency", games, [](Game& g2)
        {
          ForwardChecking::solve(g2, true, false, false);
        });


  if (std::find(algos.begin(), algos.end(), "fc+mrv") != algos.end())
    run("Forward Checking + Minimum Remaining Values", games, [](Game& g2)
        {
          ForwardChecking::solve(g2, false, true, false);
        });


  if (std::find(algos.begin(), algos.end(), "arc+mrv") != algos.end())
    run("Arc consistency + Minimum Remaining Values", games, [](Game& g2)
        {
          ForwardChecking::solve(g2, true, true, false);
        });

  if (std::find(algos.begin(), algos.end(), "fc++") != algos.end())
    run("Forward Checking + Minimum Remaining Values"
        " + Least Constraining Values", games, [](Game& g2)
        {
          ForwardChecking::solve(g2, false, true, true);
        });

  if (std::find(algos.begin(), algos.end(), "arc++") != algos.end())
    run("Arc consistency + Minimum Remaining Values"
        " + Least Constraining Values", games, [](Game& g2)
        {
          ForwardChecking::solve(g2, true, true, true);
        });


  if (std::find(algos.begin(), algos.end(), "climb") != algos.end())
    run("Hill Climbing", games, [](Game& g2)
        {
          HillClimbing::solve(g2);
        });

  if (std::find(algos.begin(), algos.end(), "sa") != algos.end())
    run("Simulated Annealing", games, [](Game& g2)
        {
          SimulatedAnnealing::solve(g2);
        });

  return 0;
}

int main(int argc, char** argv)
{
  if (argc < 3)
    return 1;

  std::string type = argv[1];
  std::string algo = argv[2];

  if (algo == "bench")
    return bench(type, argc, argv);

  Game g = load_game(type);
  Game g2 = g;

  {
    ShellCanvas cvs;
    cvs.draw_sprite(0, 0, g.to_sprite());
    cvs.render();
    g2.canvas_set(&cvs);

    if (algo == "dump")
      Dump::solve(g2);
    else if (algo == "btk")
      Backtracking::solve(g2);
    else if (algo == "fc")
      ForwardChecking::solve(g2, false, false, false);
    else if (algo == "arc")
      ForwardChecking::solve(g2, true, false, false);
    else if (algo == "fc+mrv")
      ForwardChecking::solve(g2, false, true, false);
    else if (algo == "arc+mrv")
      ForwardChecking::solve(g2, true, true, false);
    else if (algo == "fc++")
      ForwardChecking::solve(g2, false, true, true);
    else if (algo == "arc++")
      ForwardChecking::solve(g2, true, true, true);
    else if (algo == "climb")
      HillClimbing::solve(g2);
    else if (algo == "sa")
      SimulatedAnnealing::solve(g2);
    else
      return 1;
  }

  if(g2.is_complete())
    std::cout << Shell::FG_GREEN << "[SUCCESS]"
              << Shell::FG_DEFAULT << std::endl;
  else
    std::cout << Shell::FG_RED << "[FAILLURE]"
              << Shell::FG_DEFAULT << std::endl;
  
  return 0;
}
