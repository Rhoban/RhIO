// #define PLOT_ACCESSIBILITY_MODE
#include "GnuPlot.h"

float histories[] = {15.0, 30.0, 60.0, 3.0};
#define HISTORIES (sizeof(histories)/sizeof(float))

namespace RhIO
{
    GnuPlotSignal::GnuPlotSignal(std::string name_)
        : name(name_)
    {
    }

    GnuPlot::GnuPlot(bool mode2D_)
        : plotFd(-1), replot(false), history(0), timeRefOffset(-1), mode2D(mode2D_)
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
        if (timeRefOffset < 0) {
            timeRefOffset = x;
        }
        timeRef.push_back(x-timeRefOffset);
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
        int timeWindow = histories[history]*1000;
        bool removed;
        do {
            removed = false;
            auto first = timeRef.front();
            auto last = timeRef.back();
            if (last-first > timeWindow) {
                removed = true;
                timeRef.pop_front();
                for (auto signal : signals) {
                    signal->values.pop_front();
                }
            }
        } while (removed);

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
            commands = "";
#ifdef PLOT_ACCESSIBILITY_MODE
            commands += "set xtics font \",30\";";
            commands += "set ytics font \",30\";";
            commands += "set key font \",30\";";
            commands += "set lmargin 15;";
#endif
            commands += "set term qt noraise; plot ";
        }

        bool isFirst = true;
        int start = mode2D ? 1 : 0;
        for (unsigned int n=start; n<signals.size(); n++) {
            auto &signal = signals[n];
            if (!replot) {
                if (!isFirst) {
                    commands += ", ";
                }
                isFirst = false;
                if (mode2D && signals.size() == 2) {
                    commands += "'-' u 1:2:3 palette w l";
                } else {
                    commands += "'-' u 1:2 w l";
                }
#ifdef PLOT_ACCESSIBILITY_MODE
                commands += " lw 8";
#endif
                commands += " title '" + signal->name + "' ";
            }

            std::ostringstream oss;
            for (size_t k=0; k<timeRef.size(); k++) {
                if (mode2D) {
                    oss << signals[0]->values[k] << " " << signal->values[k];
                    if (signals.size() == 2) {
                        oss << " " << (timeRef[k]/1000.0);
                    };
                    oss << std::endl;
                } else {
                    oss << (timeRef[k]/1000.0) << " " << signal->values[k] << std::endl;
                }
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

    void GnuPlot::changeHistory()
    {
        history = (history+1)%HISTORIES;
        printf("History set to %gs\n", histories[history]);
    }
}
