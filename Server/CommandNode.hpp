#ifndef RHIO_COMMANDNODE_HPP
#define RHIO_COMMANDNODE_HPP

#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <functional>

#include "BaseNode.hpp"

namespace RhIO {

/**
 * CommandNode
 *
 * Implement Commands feature for IONode
 */
class CommandNode: public BaseNode<CommandNode>
{
    public:

        /**
         * Typedef for Command function
         */
        typedef std::function<
            std::string(std::vector<std::string>)> CommandFunc;
        
        /**
         * Inherit BaseNode constructor
         */
        using BaseNode<CommandNode>::BaseNode;

        /**
         * Custom assignement operator
         * (because std::mutex is non copyable)
         */
        CommandNode& operator=(const CommandNode& node);

        /**
         * Return true if given command name is registered
         */
        bool commandExist(const std::string& name) const;

        /**
         * Call given command name with given set of
         * arguments and return command result.
         * Throw std::logic_error if given name does not
         * exist
         */
        std::string call(const std::string& name, 
            const std::vector<std::string>& arguments);

        /**
         * Return the textual description of given relative
         * command name.
         * Throw std::logic_error if given name does not
         * exist
         */
        std::string commandDescription(
            const std::string& name) const;

        /**
         * Register a new command with given name, textual
         * description and callback function.
         * WARNING: command must not call another command
         * neither request method on this Command node 
         * (mutex deadlock).
         */
        void newCommand(const std::string& name, 
            const std::string& comment,
            CommandFunc func);

        /**
         * Return the relative name list of 
         * all registered commands
         */
        std::vector<std::string> listCommands() const;

    private:

        /**
         * Container map for commands functions
         * and commands descriptions
         */
        std::map<std::string, CommandFunc> _commands;
        std::map<std::string, std::string> _descriptions;
        
        /**
         * Mutex protecting concurent commands creation
         */
        mutable std::mutex _mutex;
};

}

#endif

