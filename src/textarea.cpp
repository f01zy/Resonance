#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>

using namespace std;

void textarea(WINDOW *win, string path) {
  wrefresh(win);
  curs_set(1);

  string inputText;
  ifstream fileRead(path);

  if (fileRead.is_open()) {
    string line;
    while (getline(fileRead, line)) {
      inputText += line + "\n";
    }
    fileRead.close();
  }

  mvwprintw(win, 1, 0, inputText.c_str());
  wrefresh(win);

  bool done = false;
  int ch;

  while (!done) {
    ch = wgetch(win);
    switch (ch) {
    case 27: // ESC
      done = true;
      break;
    case 8:
    case 127:
      if (!inputText.empty()) {
        inputText.pop_back();
      }
      break;
    case 10:
      inputText += '\n';
      break;
    default:
      if (isprint(ch)) {
        inputText += static_cast<char>(ch);
      }
      break;
    }

    wclear(win);
    mvwprintw(win, 1, 0, inputText.c_str());
    wrefresh(win);
  }

  ofstream fileWrite(path);
  if (fileWrite.is_open()) {
    fileWrite << inputText;
    fileWrite.close();
  }
}
