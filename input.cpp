#include <vector>
#include <iostream>
#include <curses.h>

using namespace std;

void inputField(vector<int> position, int maxLength, int width, int heigth, const char* message, char* text)
{
    WINDOW* win = newwin(heigth, width, position[1], position[0]);
    box(win, 0, 0);
    keypad(win, TRUE);

    mvwprintw(win, 1, 1, message);
    wrefresh(win);

    int ch;
    int i = 0;
    do {
        ch = wgetch(win);
        if (ch == 10) {
            delwin(win);
            break;
        }
        else if (ch == 8 && i > 0) {
            mvwaddch(win, 1, i + strlen(message), ' ');
            text[i] = ' ';
            i--;
            wrefresh(win);
        }
        else if (isprint(ch) && i <= maxLength) {
            text[i] = ch;
            mvwaddch(win, 1, i + strlen(message) + 1, ch);
            refresh();
            i++;
        }
    } while (1);
};