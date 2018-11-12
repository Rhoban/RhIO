#include <string>
#include <vector>
#include <stdio.h>
#include <ncurses.h>
#include <rhio_client/RhIOClient.hpp>
#include "NodePool.h"
#include "Node.h"

namespace RhIO
{
    class Command;
    class Shell;
    struct ValueBase;
    class Curse
    {
        public:
            Curse(Command *command);

            void run(); 
            void init();
            void loop();
            void end();
            void draw(int x, int y, std::string s);

            int selected;
            int row, col;
            int granularity;
            int maxSliders, offset;
    
            void getMinMax(ValueBase *value, float *min, float *max);
            void increment(ValueBase *value, int delta);
            void bound(ValueBase *value);

            void update(NodePool *);

            Shell *shell;
            Command *command;
            NodePool values;

            bool streamUpdated;
    };
}
