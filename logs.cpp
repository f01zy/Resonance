#include <cstring>
#include <curses.h>

using namespace std;

void errorLog(const char *error) {
  WINDOW *win = newwin(3, strlen(error) + 4, 1, 1);
  attron(COLOR_PAIR(1));
  box(win, 0, 0);
  mvwprintw(win, 1, 2, error);
  attroff(COLOR_PAIR(1));
  wrefresh(win);
};
