#pragma once

#include <set>
#include <mutex>
#include <thread>
#include "Shell.h"

#define DEFAULT_FREQ    100

namespace RhIO
{
    class NodePool;
    class Stream
    {
        public:
            Stream(Shell*);
            ~Stream();

            void boolHandler(const std::string &name, long timestamp, bool val);
            void intHandler(const std::string &name, long timestamp, int val);
            void floatHandler(const std::string &name, long timestamp, float val);
            void stringHandler(const std::string &name, long timestamp, const std::string &val);
            
            void addPool(NodePool *pool);
            void removePool(NodePool *pool);
            void setFrequency(int frequency=DEFAULT_FREQ);

            void update();

        protected:
            int frequency;
            bool alive;
            std::thread worker;
            std::mutex mutex;
            std::set<NodePool*> pools;
    };
}
