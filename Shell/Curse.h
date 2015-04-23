#include <string>
#include <vector>
#include <stdio.h>
#include <ncurses.h>
#include <Value.hpp>
#include "NodePool.h"
#include "Node.h"

#define SLIDER_WIDTH    12

namespace RhIO
{
    class Shell;
    class ValueBase;
    class Curse
    {
        public:
            void run(); 
            void init();
            void loop();
            void end();
            void draw(int x, int y, char *s);

            int selected;
            int row, col;
    
            void getMinMax(ValueBase *value, float *min, float *max);
            void increment(ValueBase *value, int delta);
            void bound(ValueBase *value);

            Shell *shell;
            NodePool values;
    };
}
