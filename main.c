#include <curses.h>
#include <math.h>
#include <unistd.h>

#define PI acos(-1)

int centerX;
int centerY;
int radius;
int frame_step;
int eye_radius;


int inCircle(int x, int y, int radius, int pointX, int pointY);
void draw_yingyang(int x, int y, int radius, int eye_radius, int angle);

int main(int argc, char** args) {
    initscr();
    curs_set(0);
    cbreak();
    noecho();

    centerX = COLS / 2;
    centerY = LINES / 2;
    radius = LINES / 2;
    frame_step = 9;
    eye_radius = radius / 4;

    int i = 0;
    while (1) {
        draw_yingyang(centerX, centerY, radius, eye_radius, i);
        i += frame_step;
        if (i >=360) {
            i = 0;
        }
        refresh();
        usleep(100000);
        clear();
    }
    getch();
    endwin();
}

void draw_yingyang(int x, int y, int radius, int eye_radius, int angle) {
    float slope = (float) angle * PI / 180;
    int yangX = x + radius / 2 * cos(slope);
    int yangY = y + radius / 2 * sin(slope);
    int yingX = x - radius / 2 * cos(slope);
    int yingY = y - radius / 2 * sin(slope);
    for (int j = y - radius; j <= y + radius; j++) {
        for (int i = x - radius; i <= x + radius; i++) {
            if (inCircle(yangX, yangY, radius / 2, i, j) >= 0) {
                mvaddch(j, i, '*');
            }
            if (inCircle(yingX, yingY, radius / 2, i, j) < 0 && inCircle(x, y, radius, i, j) > 0) {
                if ( (yangX-yingX)*(j - y) - (i - x)*(yangY - yingY) <= 0 ) {
                    mvaddch(j, i, '*');
                }
            }
            // draw yang eye
            if (inCircle(yangX, yangY, eye_radius, i, j) >= 0 ) {
                mvaddch(j, i, ' ');
            }
            // draw ying eye
            if (inCircle(yingX, yingY, eye_radius, i, j) >= 0 ) {
                mvaddch(j, i, '*');
            }
        }
    }
}

int inCircle(int x, int y, int radius, int pointX, int pointY) {
    int a = pow((pointX - x), 2) + pow((pointY - y), 2);
    int b = pow(radius, 2);
    return b - a;
}
