#include <iostream>
#include "StreamManager.h"
#include "NodePool.h"
#include "Node.h"

using namespace std::placeholders;

namespace RhIO
{
    StreamManager::StreamManager(Shell *shell)
        : alive(true), worker(&StreamManager::update, this), frequency(DEFAULT_FREQ)
    {
        auto sub = shell->getClientSub();

        sub->setHandlerBool(std::bind(&StreamManager::boolHandler, this, _1, _2, _3));
        sub->setHandlerInt(std::bind(&StreamManager::intHandler, this, _1, _2, _3));
        sub->setHandlerFloat(std::bind(&StreamManager::floatHandler, this, _1, _2, _3));
        sub->setHandlerStr(std::bind(&StreamManager::stringHandler, this, _1, _2, _3));
        sub->setHandlerStream(std::bind(&StreamManager::streamHandler, this, _1, _2, _3));
    }
    
    void StreamManager::setStreamCallback(StreamUpdateHandler handler_)
    {
        handler = handler_;
    }

    void StreamManager::unsetStreamCallback()
    {
        handler = StreamUpdateHandler();
    }

    StreamManager::~StreamManager()
    {
        alive = false;
        worker.join();
    }

    void StreamManager::boolHandler(const std::string &name, long timestamp, bool val)
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

    void StreamManager::intHandler(const std::string &name, long timestamp, int val)
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

    void StreamManager::floatHandler(const std::string &name, long timestamp, float val)
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

    void StreamManager::stringHandler(const std::string &name, long timestamp, const std::string &val)
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
            
    void StreamManager::streamHandler(const std::string &name, long timestamp, const std::string &str)
    {
        (void) timestamp;
        if (handler) {
            handler(name, str);
        }
    }
    
    void StreamManager::addPool(Shell *shell, NodePool *pool)
    {
        mutex.lock();
        auto client = shell->getClient();
        for (auto entry : *pool) {
            try {
                client->enableStreamingValue(entry.getName());
            } catch (...)
            {
            }
        }
        pools.insert(pool);
        mutex.unlock();
    }

    void StreamManager::removePool(Shell *shell, NodePool *pool)
    {
        mutex.lock();
        auto client = shell->getClient();
        for (auto entry : *pool) {
            try {
                client->disableStreamingValue(entry.getName());
            } catch (...)
            {
            }
        }
        pools.erase(pool);
        mutex.unlock();
    }

    void StreamManager::update()
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
                std::chrono::milliseconds(1000/frequency));
        }
    }

    void StreamManager::setFrequency(int frequency_)
    {
        frequency = frequency_;
    }
}
