#include <iostream>
#include "Stream.h"
#include "NodePool.h"
#include "Node.h"

using namespace std::placeholders;

namespace RhIO
{
    Stream::Stream(Shell *shell)
        : alive(true), worker(&Stream::update, this)
    {
        auto sub = shell->getClientSub();

        sub->setHandlerBool(std::bind(&Stream::boolHandler, this, _1, _2, _3));
        sub->setHandlerInt(std::bind(&Stream::intHandler, this, _1, _2, _3));
        sub->setHandlerFloat(std::bind(&Stream::floatHandler, this, _1, _2, _3));
        sub->setHandlerStr(std::bind(&Stream::stringHandler, this, _1, _2, _3));
    }

    Stream::~Stream()
    {
        alive = false;
        worker.join();
    }

    void Stream::boolHandler(const std::string &name, long timestamp, bool val)
    {
        mutex.lock();
        for (auto pool : pools) {
            for (auto node : *pool) {
                if (auto var = Node::asBool(node.value)) {
                    if (node.getName() == name) {
                        var->value = val;
                        pool->dirty = true;
                        pool->timestamp = timestamp;
                    }
                }
            }
        }
        mutex.unlock();
    }

    void Stream::intHandler(const std::string &name, long timestamp, int val)
    {
        mutex.lock();
        for (auto pool : pools) {
            for (auto node : *pool) {
                if (auto var = Node::asInt(node.value)) {
                    if (node.getName() == name) {
                        var->value = val;
                        pool->dirty = true;
                        pool->timestamp = timestamp;
                    }
                }
            }
        }
        mutex.unlock();
    }

    void Stream::floatHandler(const std::string &name, long timestamp, float val)
    {
        mutex.lock();
        for (auto pool : pools) {
            for (auto node : *pool) {
                if (auto var = Node::asFloat(node.value)) {
                    if (node.getName() == name) {
                        var->value = val;
                        pool->dirty = true;
                        pool->timestamp = timestamp;
                    }
                }
            }
        }
        mutex.unlock();
    }

    void Stream::stringHandler(const std::string &name, long timestamp, const std::string &val)
    {
        mutex.lock();
        for (auto pool : pools) {
            for (auto node : *pool) {
                if (auto var = Node::asString(node.value)) {
                    if (node.getName() == name) {
                        var->value = val;
                        pool->dirty = true;
                        pool->timestamp = timestamp;
                    }
                }
            }
        }
        mutex.unlock();
    }
    
    void Stream::addPool(NodePool *pool)
    {
        mutex.lock();
        pools.insert(pool);
        mutex.unlock();
    }

    void Stream::removePool(NodePool *pool)
    {
        mutex.lock();
        pools.erase(pool);
        mutex.unlock();
    }

    void Stream::update()
    {
        while (alive) {
            mutex.lock();
            for (auto pool : pools) {
                if (pool->dirty) {
                    pool->update();
                }
            }
            mutex.unlock();

            std::this_thread::sleep_for(
                std::chrono::milliseconds(10));
        }
    }
}
