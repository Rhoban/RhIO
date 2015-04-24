#include "GnuPlot.h"

namespace RhIO
{
    GnuPlotSignal::GnuPlotSignal(std::string name_)
        : name(name_)
    {
    }

    GnuPlot::GnuPlot()
        : plotFd(-1), replot(false), timeWindow(15000)
    {
    }

    GnuPlot::~GnuPlot()
    {
        for (auto signal : signals) {
            delete signal;
        }
    }

    void GnuPlot::setX(int x)
    {
        timeRef.push_back(x);
    }

    void GnuPlot::push(std::string name, double value)
    {
        if (!signalsByName.count(name)) {
            auto signal = new GnuPlotSignal(name);
            signalsByName[name] = signal;
            signals.push_back(signal);
        }
        auto signal = signalsByName[name];
        signal->values.push_back(value);
    }

    void GnuPlot::render()
    {
        auto first = timeRef.front();
        auto last = timeRef.back();
//#error "TO FIX!"
        if (last-first > timeWindow) {
            timeRef.pop_front();
            for (auto signal : signals) {
                signal->values.pop_front();
            }
        }

        std::string commands = generatePlotting();

        //Send to gnuplot instance
        if (plotFd <= 0) {
            createGnuplotInstance();
        }
        write(plotFd, commands.c_str(), commands.length());
        replot = true;
        //    waitCloseGnuplotInstance();
    }

    std::string GnuPlot::generatePlotting()
    {
        std::string commands;
        std::string data;

        if (replot) {
            commands = "replot ";
        } else {
            commands = "plot ";
        }

        bool isFirst = true;
        for (auto signal : signals) {
            if (!replot) {
                if (!isFirst) {
                    commands += ", ";
                }
                isFirst = false;
                commands += "'-' u 1:2 w l ";
                commands += " title '" + signal->name + "' ";
            }

            std::ostringstream oss;
            for (int k=0; k<timeRef.size(); k++) {
                oss << timeRef[k] << " " << signal->values[k] << std::endl;
            }
            data += oss.str();
            data += "e\n";
        }
        commands += ";\n";

        return commands + data;
    }

    void GnuPlot::waitCloseGnuplotInstance()
    {
        waitpid(-1, NULL, 0);
        if (plotFd != -1) {
            close(plotFd);
            plotFd = -1;
        }
    }

    void GnuPlot::createGnuplotInstance()
    {
        //Creating communication pipe
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            throw std::runtime_error("Plot failed to create pipe");
        }

        //Forkink current process
        pid_t pid = fork();
        if (pid > 0) {
            //Closing reading pipe end
            close(pipefd[0]);
            //Saving pipe fd
            plotFd = pipefd[1];
        } else if (pid == 0) {
            //Closing writting pipe end
            close(pipefd[1]);
            //Redirecting reading pipe end to standart input
            if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                throw std::runtime_error("Plot failed to dup2");
            }
            //Closing output and err
            int null = open("/dev/null", O_WRONLY);
            if (dup2(null, STDOUT_FILENO) == -1) {
                throw std::runtime_error("Plot failed to dup2");
            }
            if (dup2(null, STDERR_FILENO) == -1) {
                throw std::runtime_error("Plot failed to dup2");
            }
            //Calling Gnuplot
            execlp("gnuplot", "gnuplot", "-", NULL);
        } else {
            throw std::runtime_error("Plot failed to fork");
        }
    }

    void GnuPlot::closeWindow()
    {
        write(plotFd, "quit\n", 5);
        write(plotFd, "quit\n", 5);
        close(plotFd);
    }
}
