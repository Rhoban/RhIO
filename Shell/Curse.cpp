#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <ncurses.h>
#include "Curse.h"
#include "Shell.h"

#define SLIDER_WIDTH    12

namespace RhIO
{
    void Curse::run()
    {
        init();
        loop();
        end();
    }

    void Curse::init()
    {
        initscr();
        clear();
        noecho();
        cbreak();
        getmaxyx(stdscr, row, col);
        keypad(stdscr, TRUE);
        refresh();
        selected = 0;
        curs_set(0);
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_BLACK, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLUE);
        init_pair(4, COLOR_GREEN, COLOR_GREEN);
        wbkgd(stdscr, COLOR_PAIR(1));

        // Getting fresh values for variables
        for (auto value : values) {
            Node::get(shell, value);
        }
    }

    void Curse::getMinMax(ValueBase *value, float *min, float *max)
    {
        *min = -10;
        *max = 10;
        
        if (auto val = Node::asInt(value)) {
            if (val->hasMin) *min = val->min;
            if (val->hasMax) *max = val->max;
        } else if (auto val = Node::asFloat(value)) {
            if (val->hasMin) *min = val->min;
            if (val->hasMax) *max = val->max;
        }
    }

    void Curse::bound(ValueBase *value)
    {
        float min, max;
        getMinMax(value, &min, &max);

        if (auto val = Node::asInt(value)) {
            if (val->value < min) val->value = min;
            if (val->value > max) val->value = max;
        } else if (auto val = Node::asFloat(value)) {
            if (val->value < min) val->value = min;
            if (val->value > max) val->value = max;
        }
    }

    void Curse::increment(ValueBase *value, int delta)
    {
        if (auto val = Node::asInt(value)) {
            val->value += delta;
        } else if (auto val = Node::asFloat(value)) {
            float min, max;
            getMinMax(val, &min, &max);
            val->value += delta*((max-min)/100.0);
        }

        bound(value);
    }

    void Curse::loop()
    {
        int names = row-3;

        while(1) {   
            attron(COLOR_PAIR(1));
            int pos = 0;
            clear();

            // Drawing parameters
            for (auto nodeValue : values) {
                auto value = nodeValue.value;
                if (pos == selected) {
                    attron(COLOR_PAIR(3));
                    for (int k=0; k<SLIDER_WIDTH; k++) {
                        for (int r=0; r<row; r++) {
                            draw(r, k+SLIDER_WIDTH*pos, " ");
                        }
                    }
                } else {
                    attron(COLOR_PAIR(1));
                }

                char buffer[SLIDER_WIDTH+2];
                sprintf(buffer, " %-10s", value->name.c_str());
                draw(names, SLIDER_WIDTH*pos, buffer);

                int center_x = SLIDER_WIDTH*(pos+1)-(SLIDER_WIDTH/2)-2;

                if (Node::asInt(value) || Node::asFloat(value)) {
                    float min, max, cvalue;
                    getMinMax(value, &min, &max);

                    if (auto val = Node::asInt(value)) {
                        sprintf(buffer, " %d", val->value);
                        cvalue = (float)val->value;
                    } else if (auto val = Node::asFloat(value)) {
                        sprintf(buffer, " %.2f", val->value);
                        cvalue = val->value;
                    }

                    draw(names+1, SLIDER_WIDTH*pos, buffer);

                    int kmin = 2;
                    int kmax = names-1;
                    int kdelta = kmax-kmin;
                    for (int k=kmin; k<kmax; k++) {
                        float kvalue = (1-(k-kmin)/(float)kdelta)*(max-min)+min;
                        if (cvalue >= kvalue) {
                            attron(COLOR_PAIR(4));
                        } else {
                            attron(COLOR_PAIR(2));
                        }
                        draw(k, center_x, "***");
                    }
                }

                if (auto val = Node::asBool(value)) {
                    draw(names+1, SLIDER_WIDTH*pos, (char*)(val->value ? " True" : " False"));

                    int kpos = row-6;
                    if (val->value) {
                        attron(COLOR_PAIR(4));
                    } else {
                        attron(COLOR_PAIR(2));
                    }
                    draw(kpos-1, center_x-1, "******");
                    draw(kpos, center_x-1, "******");
                    draw(kpos+1, center_x-1, "******");
                }

                pos++;
            }
            attron(COLOR_PAIR(1));
            draw(0, 0, "Parameters tuner");

            auto nodeValue = values[selected];
            auto value = nodeValue.value;
            int c = wgetch(stdscr);
            if (c == KEY_LEFT) {
                selected--;
                if (selected < 0) selected = 0;
            }
            if (c == KEY_RIGHT) {
                selected++;
                if (selected >= values.size()) selected = values.size()-1;
            }
            
            if (Node::asInt(value) || Node::asFloat(value)) {
                float min, max;
                getMinMax(value, &min, &max);

                if (c == KEY_DOWN) {
                    increment(value, -1);
                    Node::set(shell, nodeValue);
                }
                if (c == KEY_UP) {
                    increment(value, 1);
                    Node::set(shell, nodeValue);
                }
            }
            if (auto val = Node::asBool(value)) {
                if (c == ' ') {
                    val->value = !val->value;
                    Node::set(shell, nodeValue);
                }
            }
            if (c == 'q' || c == '\n') {
                break;
            }
        }
    }

    void Curse::end()
    {
        curs_set(1);
        echo();
        clrtoeol();
        endwin();
    }

    void Curse::draw(int x, int y, char *s)
    {
        mvwprintw(stdscr, x, y, s);
    }

    int row, col;
}
