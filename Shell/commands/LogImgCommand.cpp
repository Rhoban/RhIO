#include <gd.h>
#include "Shell.h"
#include "LogImgCommand.h"
#include "CSV.h"
#include "NodePool.h"
#include "StreamManager.h"

using namespace std::placeholders;

namespace RhIO
{
    std::string LogImgCommand::getName()
    {
        return "logimg";
    }

    std::string LogImgCommand::getDesc()
    {
        return "Log images to files";
    }

    std::string LogImgCommand::getUsage()
    {
        return "logimg frame ...";
    }

    void LogImgCommand::process(std::vector<std::string> args)
    {
        if (!args.size()) {
            errorUsage();
        } else {
            auto client = shell->getClient();
            auto stream = shell->getStream();
            
            for (size_t i=0;i<args.size();i++) {
                auto nodeFrame = shell->getNodeFrame(args[i]);
                ids[nodeFrame.getName()] = 0;
                names[nodeFrame.getName()] = nodeFrame.name;
                client->enableStreamingFrame(nodeFrame.getName());
            }
            
            stream->setFrameCallback(std::bind(&LogImgCommand::update, this, _1, _2, _3, _4, _5));
            shell->wait();
            stream->unsetFrameCallback();
            clearStream();
            
            for (size_t i=0;i<args.size();i++) {
                auto nodeFrame = shell->getNodeFrame(args[i]);
                client->disableStreamingFrame(nodeFrame.getName());
            }
        }
    }

    void LogImgCommand::update(std::string name, size_t width, size_t height, 
        unsigned char* data, size_t size)
    {
        if (ids.count(name)) {
            char buffer[name.size()+10];
            sprintf(buffer, "%s-%05d.png", names[name].c_str(), ids[name]);
            ids[name]++;

            (void)size;
            int k = 0;
            auto img = gdImageCreateTrueColor(width, height);
            for (unsigned int y=0; y<height; y++) {
                for (unsigned int x=0; x<width; x++) {
                    int c = 0;
                    c |= ((data[k++]&0xff)<<16);
                    c |= ((data[k++]&0xff)<<8);
                    c |= ((data[k++]&0xff)<<0);
                    gdImageSetPixel(img, x, y, c);
                }
            }

            if (FILE *output = fopen(buffer, "w")) {
                gdImagePng(img, output);
                fclose(output);
            }
            gdImageDestroy(img);
        }
    }
}
