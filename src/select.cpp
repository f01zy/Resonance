#include <ncurses.h>
#include <vector>

using namespace std;

int selectMenu(const char *options[], int length, vector<int> position) {
  int highlight = 0;
  int choice;

  while (1) {
    for (int i = 0; i < length; i++) {
      if (i == highlight) {
        attron(A_REVERSE);
      }
      mvprintw(position[1] + i, position[0], options[i]);
      attroff(A_REVERSE);
    }
    choice = getch();

    switch (choice) {
    case KEY_UP:
      highlight--;
      if (highlight == -1) {
        highlight = 0;
      }
      break;
    case KEY_DOWN:
      highlight++;
      if (highlight == length) {
        highlight = length - 1;
      }
      break;
    default:
      break;
    }

    if (choice == 10) {
      return highlight;
    }
  }
};
