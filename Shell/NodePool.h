#pragma once

#include <functional>
#include "Node.h"

namespace RhIO
{
    class NodePool : public std::vector<NodeValue>
    {
        public:
            typedef std::function<void (NodePool *pool)> PoolUpdateHandler;

            NodePool();
            void setCallback(PoolUpdateHandler handler);
            void update();
            bool dirty;
            long timestamp;

            void draw();

        protected:
            PoolUpdateHandler handler;
    };
}
