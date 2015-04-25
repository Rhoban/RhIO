#include <form.h>
#include <panel.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <ncurses.h>
#include "Curse.h"
#include "Shell.h"

#define SLIDER_WIDTH    12

static float granularities[] = {100, 1000, 10000};

#define GRANULARITIES   (sizeof(granularities)/sizeof(float))

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
        granularity = 0;
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
            shell->getFromServer(value);
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
            val->value += delta*((max-min)/granularities[granularity]);
        }

        bound(value);
    }

    void Curse::loop()
    {
        FIELD *field[2] = {NULL, NULL};
        FORM *form = NULL;
        int names = row-3;

        while(1) {   
            attron(COLOR_PAIR(1));
            int pos = 0;

            if (form == NULL) {
                clear();
            }

            // Drawing parameters
            for (auto nodeValue : values) {
                auto value = nodeValue.value;
                if (pos == selected && form==NULL) {
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
                    
                if (form == NULL || pos != selected) {
                    std::string strVal = Node::toString(value);
                    draw(names+1, SLIDER_WIDTH*pos+1, strVal.c_str());
                }

                if (Node::asInt(value) || Node::asFloat(value)) {
                    float min, max, cvalue;
                    getMinMax(value, &min, &max);

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
            mvwprintw(stdscr, 0, 0, "Parameters tuner, granularity: 1/%g", granularities[granularity]);

            auto nodeValue = values[selected];
            auto value = nodeValue.value;
            int c = wgetch(stdscr);

            if (form != NULL) {
                if (c == 10 || c == 27) {
                    if (c == 10) {
                        form_driver(form, REQ_VALIDATION);
                        char *buf = field_buffer(field[0], 0);
                        shell->setFromString(nodeValue, std::string(buf));
                    }

                    unpost_form(form);
                    free_form(form);
                    free_field(field[0]);
                    form = NULL;
                } else {
                    form_driver(form, c);
                }
            } else {
                if (c == KEY_LEFT) {
                    selected--;
                    if (selected < 0) selected = 0;
                }
                if (c == KEY_RIGHT) {
                    selected++;
                    if (selected >= values.size()) selected = values.size()-1;
                }
                if (c == 'g') {
                    granularity = (granularity+1)%GRANULARITIES;
                }

                if (c == 'v') {
                    field[0] = new_field(1, SLIDER_WIDTH-2, names+1, 1+SLIDER_WIDTH*selected, 0, 0);
                    set_field_back(field[0], A_UNDERLINE);
                    form = new_form(field);
                    post_form(form);
                    refresh();
                }
                
                if (Node::asInt(value) || Node::asFloat(value)) {
                    float min, max;
                    getMinMax(value, &min, &max);

                    if (c == '0') {
                        if (auto v = Node::asInt(value)) {
                            v->value = 0;
                        }
                        if (auto v = Node::asFloat(value)) {
                            v->value = 0;
                        }
                        bound(value);
                        shell->setToServer(nodeValue);
                    }
                    if (c == KEY_DOWN) {
                        increment(value, -1);
                        shell->setToServer(nodeValue);
                    }
                    if (c == KEY_UP) {
                        increment(value, 1);
                        shell->setToServer(nodeValue);
                    }
                }
                if (auto val = Node::asBool(value)) {
                    if (c == ' ') {
                        val->value = !val->value;
                        shell->setToServer(nodeValue);
                    }
                    if (c == '0') {
                        val->value = false;
                        shell->setToServer(nodeValue);
                    }
                    if (c == '1') {
                        val->value = true;
                        shell->setToServer(nodeValue);
                    }
                }
                if (c == 'q' || c == '\n') {
                    break;
                }
            }
        }
    }

    void Curse::end()
    {
        curs_set(1);
        endwin();
        echo();
        clrtoeol();
    }

    void Curse::draw(int x, int y, std::string s)
    {
        mvwprintw(stdscr, x, y, s.c_str());
    }

    int row, col;
}
