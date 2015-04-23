#include <iostream>
#include "Terminal.h"
#include "NodePool.h"

namespace RhIO
{
    NodePool::NodePool()
        : dirty(false), handler(PoolUpdateHandler()), timestamp(0)
    {
    }
            
    void NodePool::setCallback(PoolUpdateHandler handler_)
    {
        handler = handler_;
    }

    void NodePool::update()
    {
        if (handler) {
            handler(this);
        }
        dirty = false;
    }

    void NodePool::draw()
    {
        for (auto nodeVal : *this) {
            auto val = nodeVal.value;

            printf("%-15s", val->name.c_str());
            std::cout << " ";

            Terminal::setColor("grey", false);
            printf("%6s ", Node::getType(val).c_str());
            Terminal::clear();

            Terminal::setColor("grey", false);
            std::cout << " val: ";
            Terminal::clear();
            printf("%-8s", Node::toString(val).c_str());

            if (val->comment != "") {
                Terminal::setColor("grey", false);
                std::cout << " desc: ";
                Terminal::clear();
                std::cout << val->comment;
            }
            std::cout << std::endl;
        }
    }
}
