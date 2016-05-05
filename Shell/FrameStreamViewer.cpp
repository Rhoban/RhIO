#include "FrameStreamViewer.hpp"

namespace RhIO {

FrameStreamViewer::FrameStreamViewer(const std::string& name,
    FrameFormat format,
    size_t width, size_t height) :
    _name(name),
    _format(format),
    _width(width),
    _height(height),
    _pipeFd(-1),
    _playerPID(-1)
{
}

void FrameStreamViewer::start()
{
    if (_pipeFd != -1) {
        throw std::logic_error(
            "StreamViewer already started");
    }
    createPlayerInstance();
}
void FrameStreamViewer::stop()
{
    if (_pipeFd != -1) {
        close(_pipeFd);
        _pipeFd = -1;
        kill(_playerPID, SIGTERM);
    }
}

void FrameStreamViewer::pushFrame(unsigned char* data, size_t size)
{
    if (_pipeFd == -1) {
        throw std::logic_error(
            "StreamViewer not started");
    }
    write(_pipeFd, data, size);
}

void FrameStreamViewer::createPlayerInstance()
{
    //Creating communication pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw std::runtime_error(
            "StreamViewer failed to create pipe");
    }

    //Ignore the broken pipe signal
    signal(SIGPIPE, SIG_IGN);
    
    //Forking current process
    pid_t pid = fork();
    if (pid > 0) {
        //Closing reading pipe end
        close(pipefd[0]);
        //Saving pipe fd
        _pipeFd = pipefd[1];
        _playerPID = pid;
    } else if (pid == 0) {
        //Closing writting pipe end
        close(pipefd[1]);
        //Redirecting reading pipe end to standart input
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            throw std::runtime_error(
                "StreamViewer failed to dup2");
        }
        //Closing output and err
        int null = open("/dev/null", O_WRONLY);
        if (dup2(null, STDOUT_FILENO) == -1) {
            throw std::runtime_error(
                "StreamViewer failed to dup2");
        }
        if (dup2(null, STDERR_FILENO) == -1) {
            throw std::runtime_error(
                "StreamViewer failed to dup2");
        }
        //Calling the Player
        std::string videoSize = std::to_string(_width) 
            + "x" + std::to_string(_height);
        std::string format = "";
        if (_format == FrameFormat::RGB) {
            format = "rgb24";
        }
        if (_format == FrameFormat::BGR) {
            format = "bgr24";
        }
        if (_format == FrameFormat::YUV) {
            format = "yuv444p";
        }
        execlp("ffplay", "ffplay", 
            "-v", "quiet", 
            "-f", "rawvideo", 
            "-infbuf",
            "-framedrop",
            "-window_title", _name.c_str(),
            "-pixel_format", format.c_str(), 
            "-video_size", videoSize.c_str(), 
            "-framerate", "100", 
            "-",
            NULL);
        throw std::logic_error(
            "StreamViewer exec fails");
    } else {
        throw std::runtime_error(
            "StreamViewer failed to fork");
    }
}

}

