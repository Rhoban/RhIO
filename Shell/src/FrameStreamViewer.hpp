#pragma once

#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <rhio_client/RhIOClient.hpp>

namespace RhIO {

/**
 * FrameStreamViewer
 *
 * Use ffplay to display real time
 * frame in raw RGB24 format
 */
class FrameStreamViewer
{
    public:

        /**
         * Initialization with image size
         */
        FrameStreamViewer(const std::string& name, 
            FrameFormat format);

        /**
         * Start and stop player instance.
         * (The proccess is forked 
         * and a pipe is created).
         */
        void start();
        void stop();

        /**
         * Display the given image with the Player.
         * Raw image data and its size are given.
         * Image width and height size are given.
         */
        void pushFrame(
            size_t width, size_t height, 
            unsigned char* data, size_t size);

    private:

        /**
         * Windows name
         */
        std::string _name;

        /**
         * Image format
         */
        FrameFormat _format;

        /**
         * Image stream format
         */
        size_t _width;
        size_t _height;

        /**
         * Player pipe file descriptor
         * and child PID
         */
        int _pipeFd;
        int _playerPID;

        /**
         * Fork current process to create 
         * a new Player window
         */
        void createPlayerInstance();
};

}

