#pragma once

#include <map>
#include <vector>
#include <deque>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

namespace RhIO
{
    class GnuPlotSignal
    {
        public:
            GnuPlotSignal(std::string name);
            std::string name;
            std::deque<double> values;
    };

    /**
     * Plot
     *
     * Programming interface with
     * the Linux ploting utility 
     * Gnuplot using VectorLabel
     */
    class GnuPlot
    {
        public:
            GnuPlot();
            ~GnuPlot();

            /**
             * Change the X
             */
            void setX(int x);

            /**
             * Push a value
             */
            void push(std::string name, double value);

            /**
             * Render the plot
             * Wait until plot window is closed
             */
            void render();

            /**
             * Close gnuplot
             */
            void closeWindow();

            /**
             * Change the history size
             */
            void changeHistory();

        private:
            /**
             * Replot?
             */
             bool replot;

            /**
             * Current history size
             */
            int history;

            /**
             * Gnuplot pipe file descriptor
             */
            int plotFd;

            /**
             * Time reference offset
             */
            int timeRefOffset;

            /**
             * Time reference
             */
            std::deque<int> timeRef;

            /**
             * Values to plot
             */
            std::vector<GnuPlotSignal*> signals;
            std::map<std::string, GnuPlotSignal*> signalsByName;

            /**
             * Fork current process to create a new GnuPlot window
             */
            void createGnuplotInstance();

            /**
             * Wait for end of gnuplot session and
             * close the openned pipe to GnuPlot window instance
             */
            void waitCloseGnuplotInstance();

            /**
             * Generate and return Gnuplot commands and data
             */
            std::string generatePlotting();
    };
}
