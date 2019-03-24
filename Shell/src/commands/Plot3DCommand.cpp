#include <iostream>
#include "Shell.h"
#include "Plot3DCommand.h"
#include "Node.h"
#include "NodePool.h"
#include "StreamManager.h"
#include <GnuPlot.h>

using namespace std::placeholders;

namespace RhIO
{
std::string Plot3DCommand::getName()
{
  return "plot3d";
}

std::string Plot3DCommand::getDesc()
{
  return "Plot 3D parameters";
}

std::string Plot3DCommand::getUsage()
{
  return "x-var y1 y2 [y3 [y4...]]";
}

void Plot3DCommand::process(std::vector<std::string> args)
{
  if (args.size() < 3)
  {
    errorUsage();
  }

  NodePool pool;
  pool = shell->getPool(args);

  Terminal::setColor("white", true);
  std::cout << "Plotting." << std::endl;
  std::cout << "  q: quit" << std::endl;
  std::cout << "  h: change history" << std::endl;
  std::cout << "  p: pause/unpause" << std::endl;
  Terminal::clear();

  paused = false;
  GnuPlot plot(3);
  pool.setCallback(std::bind(&Plot3DCommand::update, this, &plot, _1));

  auto stream = shell->getStream();
  stream->addPool(shell, &pool);
  while (true)
  {
    char c;
    if ((c = getchar()) > 0)
    {
      if (c == 'q')
      {
        break;
      }
      if (c == 'h')
      {
        plot.changeHistory();
      }
      if (c == 'p')
      {
        paused = !paused;
        if (paused)
        {
          printf("Paused\n");
        }
        else
        {
          printf("Unpaused\n");
        }
      }
    }
  }
  stream->removePool(shell, &pool);

  plot.closeWindow();
}

void Plot3DCommand::update(GnuPlot* plot, NodePool* pool)
{
  if (!paused)
  {
    plot->setX(pool->timestamp);
    for (auto node : *pool)
    {
      plot->push(node.getName(), Node::toNumber(node.value));
    }
    plot->render();
  }
}
}  // namespace RhIO
