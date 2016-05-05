#include <iostream>
#include "Shell.h"
#include "ViewCommand.h"
#include "CSV.h"
#include "NodePool.h"
#include "StreamManager.h"

using namespace std::placeholders;

namespace RhIO
{
    std::string ViewCommand::getName()
    {
        return "view";
    }

    std::string ViewCommand::getDesc()
    {
        return "View a frame stream";
    }

    std::string ViewCommand::getUsage()
    {
        return "view frame ...";
    }

    void ViewCommand::process(std::vector<std::string> args)
    {
        if (!args.size()) {
            errorUsage();
        } else {
            auto client = shell->getClient();
            auto stream = shell->getStream();
            
            _viewers.clear();
            for (size_t i=0;i<args.size();i++) {
                auto nodeFrame = shell->getNodeFrame(args[i]);
                _viewers.push_back({
                    nodeFrame.getName(),
                    FrameStreamViewer(nodeFrame.getName(), 
                        nodeFrame.format,
                        nodeFrame.width, nodeFrame.height)
                });
                _viewers[i].second.start();
                client->enableStreamingFrame(nodeFrame.getName());
            }
            
            stream->setFrameCallback(std::bind(&ViewCommand::update, this, _1, _2, _3));
            shell->wait();
            stream->unsetFrameCallback();
            clearStream();
            
            for (size_t i=0;i<args.size();i++) {
                auto nodeFrame = shell->getNodeFrame(args[i]);
                client->disableStreamingFrame(nodeFrame.getName());
                _viewers[i].second.stop();
            }
        }
    }

    void ViewCommand::update(std::string name, unsigned char* data, size_t size)
    {
        for (size_t i=0;i<_viewers.size();i++) {
            if (name == _viewers[i].first) {
                _viewers[i].second.pushFrame(data, size);
            }
        }
    }
}
