#pragma once

#include <set>
#include <mutex>
#include <thread>
#include "Shell.h"

#define DEFAULT_FREQ    100

namespace RhIO
{
    class NodePool;
    class StreamManager
    {
        public:
            StreamManager(Shell*);
            ~StreamManager();
            
            /**
             * Handler to update the stream
             */
            typedef std::function<void (std::string name, std::string)> StreamUpdateHandler;
            void setStreamCallback(StreamUpdateHandler handler);
            void unsetStreamCallback();

            /**
             * Handlers
             */
            void boolHandler(const std::string &name, long timestamp, bool val);
            void intHandler(const std::string &name, long timestamp, int val);
            void floatHandler(const std::string &name, long timestamp, float val);
            void stringHandler(const std::string &name, long timestamp, const std::string &val);
            void streamHandler(const std::string &name, long timestamp, const std::string &str);
            
            /**
             * Add a node pool to monitor
             */
            void addPool(Shell *shell, NodePool *pool);
            void removePool(Shell *shell, NodePool *pool);

            /**
             * Sets the frequency limit for the flush
             */
            void setFrequency(int frequency=DEFAULT_FREQ);

            void update();            

        protected:
            int frequency;
            bool alive;
            std::thread worker;
            std::mutex mutex;
            std::set<NodePool*> pools;
            StreamUpdateHandler handler;
    };
}
