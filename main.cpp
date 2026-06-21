#include <string>
#include "Streams/IReadOnlyStream.h"
#include "Streams/FileReadOnlyStream.h"
#include "OnlineStatistic.h"
#include <ncursesw/curses.h>

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    wbkgd(stdscr, COLOR_PAIR(1));

    attron(A_BOLD);
    mvprintw(1, 2, "online statistic");
    attroff(A_BOLD);
    
    mvprintw(3, 2, "file: t.txt");
    mvprintw(4, 2, "You can use: [space] - next object | [q] - exit");
    refresh();

    try {
        ReadOnlyStream<int> stream("t.txt");
        Statistics<int> stats;

        while (true) {
            int ch = getch();
            if (ch == 'q' || ch == 'Q') {
                break;
            }
            if (ch == ' ') {
                auto opt_val = stream.try_read();
                if (!opt_val.has_value()) {
                    attron(COLOR_PAIR(3) | A_BOLD);
                    mvprintw(6, 2, "the end file. Press [q] for exit");
                    attroff(COLOR_PAIR(3) | A_BOLD);
                    refresh();
                    continue;
                }
                stats.add(opt_val.value());
                move(6, 0);
                clrtobot();
                mvprintw(6, 2, "new number: %d", opt_val.value());
                mvprintw(8, 2, "count: %zu", stats.count());
                mvprintw(9, 2, "sum: %d", stats.sum());
                mvprintw(10, 2, "min: %d", stats.min());
                mvprintw(11, 2, "max: %d", stats.max());
                mvprintw(12, 2, "median: %.2f", stats.get_median());
                refresh();
            }
        }
    } catch (const std::exception& e) {
        clear();
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(2, 2, "exception!");
        attroff(COLOR_PAIR(2) | A_BOLD);
        
        mvprintw(4, 2, "%s", e.what());
        mvprintw(6, 2, "press any boutons");
        refresh();
        getch();
    }

    endwin(); 
    return 0;
}