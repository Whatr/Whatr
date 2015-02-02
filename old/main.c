#include <X11/Xlib.h>

int main(int argc, char** argv) {
    Display* display = XOpenDisplay(0);
    XCloseDisplay(display);
    return 0;
}
