#include <string>
#include <fstream>
#include <string>
#include <iostream>

#include <curses.h>
#include "globals.h"

using namespace std;

void textarea(WINDOW* win, string path) {
    wrefresh(win);
    curs_set(1);

    string inputText;
    ifstream fileRead(path);

    if (fileRead.is_open()) {
        string line;
        while (getline(fileRead, line)) {
            inputText += line += "\n";
        }
        fileRead.close();
    }

    mvwprintw(win, 1, 0, inputText.c_str());
    wrefresh(win);

    bool done = false;
    int ch;

    ofstream fileWrite(path);
    while (!done) {
        ch = wgetch(win);
        switch (ch) {
        case 27:
            if (fileWrite.is_open())
            {
                fileWrite << inputText << endl;
            }
            return;
        case 8:
            if(!inputText.empty())
                inputText.pop_back();
            break;
        default:
            inputText += ch;
            break;
        }
        wclear(win);
        wrefresh(win);
        mvwprintw(win, 1, 0, inputText.c_str());
        wrefresh(win);
    }
};