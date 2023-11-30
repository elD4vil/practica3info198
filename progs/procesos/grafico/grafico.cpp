#include "../../funciones/funciones.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_POINTS 100

struct Point {
    int x;
    int y;
};

int main() {
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "No se puede abrir la pantalla\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    unsigned long white = WhitePixel(display, screen);
    unsigned long black = BlackPixel(display, screen);

    Window window = XCreateSimpleWindow(display, root, 10, 10, 500, 500, 1, black, white);

    XSelectInput(display, window, ExposureMask | KeyPressMask);

    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, NULL);
    XSetForeground(display, gc, black);

    Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1);

    XFontStruct *font_info;
    const char *font_name = "fixed";  // Cambié la fuente a "fixed"
    font_info = XLoadQueryFont(display, font_name);

    if (!font_info) {
        fprintf(stderr, "No se puede cargar la fuente %s\n", font_name);
        return 1;
    }

    XSetFont(display, gc, font_info->fid);

    FILE *file = fopen("grafico.gra", "r");
    if (!file) {
        fprintf(stderr, "No se puede abrir el archivo\n");
        return 1;
    }

    char line[100];
    char title[100];
    Point points[MAX_POINTS];
    int pointCount = 0;

    XEvent event;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "titulo:", 7) == 0) {
            sscanf(line, "titulo:\"%[^\"]\"", title);
        } else {
            sscanf(line, "x:%d,y:%d", &points[pointCount].x, &points[pointCount].y);
            pointCount++;
        }
    }

    fclose(file);

    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            // Dibujar plano cartesiano
            XDrawLine(display, window, gc, 50, 450, 50, 50);
            XDrawLine(display, window, gc, 50, 450, 450, 450);

            // Dibujar puntos y líneas
            for (int i = 0; i < pointCount - 1; ++i) {
                XDrawLine(display, window, gc, 50 + points[i].x, 450 - points[i].y, 50 + points[i + 1].x, 450 - points[i + 1].y);
            }

            // Mostrar título
            XDrawString(display, window, gc, 200, 30, title, strlen(title));
        }

        if (event.type == KeyPress) {
            break;
        }

        if (event.type == ClientMessage) {
            if (event.xclient.data.l[0] == WM_DELETE_WINDOW) {
                break;
            }
        }
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}


