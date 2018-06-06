#include <iostream>
#include "StreamManager.h"
#include "NodePool.h"
#include "Node.h"

using namespace std::placeholders;

namespace RhIO
{
    StreamManager::StreamManager(Shell *shell)
        : alive(true), frequency(DEFAULT_FREQ)
    {
        clientRep = shell->getClient();
        clientSub = shell->getClientSub();

        clientSub->setHandlerBool(std::bind(&StreamManager::boolHandler, this, _1, _2, _3));
        clientSub->setHandlerInt(std::bind(&StreamManager::intHandler, this, _1, _2, _3));
        clientSub->setHandlerFloat(std::bind(&StreamManager::floatHandler, this, _1, _2, _3));
        clientSub->setHandlerStr(std::bind(&StreamManager::stringHandler, this, _1, _2, _3));
        clientSub->setHandlerStream(std::bind(&StreamManager::streamHandler, this, _1, _2, _3));
        clientSub->setHandlerFrame(std::bind(&StreamManager::frameHandler, this, _1, _2, _3, _4, _5, _6));

        worker = new std::thread(&StreamManager::update, this);
    }
    
    void StreamManager::setStreamCallback(StreamUpdateHandler handler_)
    {
        handlerStream = handler_;
    }

    void StreamManager::unsetStreamCallback()
    {
        handlerStream = StreamUpdateHandler();
    }
    
    void StreamManager::setFrameCallback(FrameUpdateHandler handler_)
    {
        handlerFrame = handler_;
    }

    void StreamManager::unsetFrameCallback()
    {
        handlerFrame = FrameUpdateHandler();
    }

    StreamManager::~StreamManager()
    {
        alive = false;
        worker->join();
        delete worker;
    }

    void StreamManager::boolHandler(const std::string &name, long timestamp, bool val)
    {
        mutex.lock();
        for (auto& pool : pools) {
            for (auto& node : *pool) {
                if (const auto& var = Node::asBool(node.value)) {
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
        for (auto& pool : pools) {
            for (auto& node : *pool) {
                if (const auto& var = Node::asInt(node.value)) {
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
        for (auto& pool : pools) {
            for (auto& node : *pool) {
                if (const auto& var = Node::asFloat(node.value)) {
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
        for (auto& pool : pools) {
            for (auto& node : *pool) {
                if (const auto& var = Node::asString(node.value)) {
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
        if (handlerStream) {
            handlerStream(name, str);
        }
    }
    
    void StreamManager::frameHandler(const std::string &name, long timestamp, 
        size_t width, size_t height,
        unsigned char* data, size_t size)
    {
        (void) timestamp;
        if (handlerFrame) {
            handlerFrame(name, width, height, data, size);
        }
    }
    
    void StreamManager::addPool(Shell *shell, NodePool *pool)
    {
        mutex.lock();
        auto client = shell->getClient();
        for (auto& entry : *pool) {
            try {
                client->enableStreamingValue(entry.getName());
            } catch (...) {
            }
        }
        pools.insert(pool);
        mutex.unlock();
    }

    void StreamManager::removePool(Shell *shell, NodePool *pool)
    {
        mutex.lock();
        auto client = shell->getClient();
        for (auto& entry : *pool) {
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
        lastStreamingCheck = std::chrono::system_clock::now();
        while (alive) {
            mutex.lock();
            for (auto& pool : pools) {
                if (pool->dirty) {
                    pool->update();
                }
            }
            //Check regularly than streaming is
            //enabled for registered values
            auto timeNow = std::chrono::system_clock::now();
            std::chrono::duration<double, std::milli> dur = 
                timeNow - lastStreamingCheck;
            if (dur.count() > 2000.0) {
                for (auto& pool : pools) {
                    for (auto& entry : *pool) {
                        try {
                            clientRep->checkStreamingValue(entry.getName());
                        } catch (...)
                        {
                        }
                    }
                }
                lastStreamingCheck = std::chrono::system_clock::now();
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
