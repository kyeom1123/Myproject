#if defined(_WIN32)
// Windows 용 getch
#include <conio.h>
#define my_getch _getch

#elif defined(__linux__) || defined(__unix__)
#include <termios.h>
#include <unistd.h>

char _getch() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

#else
#error "Unsupported platform"
#endif