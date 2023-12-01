#include "../../funciones/funciones.h"

#define MAX_POINTS 100
#define GRAPH_SIZE 380
#define POINT_SIZE 10

struct Point {
    int x;
    int y;
};

void scalePoint(Point *point, int windowSize, int graphSize) {
    point->x = (point->x * (windowSize)) / graphSize;
    point->y = (point->y * (windowSize )) / graphSize;
}

int main(int argc, char* argv[]){

    const char* graphPath = argv[1];

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "No se puede abrir la pantalla\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    unsigned long white = WhitePixel(display, screen);
    unsigned long black = BlackPixel(display, screen);

    Window window = XCreateSimpleWindow(display, root, 10, 10, 510, 500, 1, black, white);

    XSelectInput(display, window, ExposureMask | KeyPressMask);

    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, NULL);
    XSetForeground(display, gc, black);

    Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1);

    XFontStruct *font_info;
    const char *font_name = "10x20";  // Cambié la fuente a "fixed"
    font_info = XLoadQueryFont(display, font_name);

    if (!font_info) {
        fprintf(stderr, "No se puede cargar la fuente %s\n", font_name);
        return 1;
    }

    XSetFont(display, gc, font_info->fid);

    FILE *file = fopen(graphPath, "r");
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
            scalePoint(&points[pointCount], 1500, GRAPH_SIZE);
            pointCount++;
        }
    }

    fclose(file);

    XSetLineAttributes(display, gc, 2, LineSolid, CapButt, JoinRound);

    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            // Dibujar plano cartesiano
            XDrawLine(display, window, gc, 50, 70, 50, 70 + GRAPH_SIZE);
            XDrawLine(display, window, gc, 50, 70 + GRAPH_SIZE, 50 + GRAPH_SIZE, 70 + GRAPH_SIZE);

            XDrawString(display, window, gc, 50 + GRAPH_SIZE, 60 + GRAPH_SIZE + 10, "X = 100", 7);
            XDrawString(display, window, gc, 18, 60, "Y = 100", 8);

            // Dibujar puntos y líneas

            for (int i = 0; i < pointCount; ++i) {
                // Dibujar el punto
                XFillArc(display, window, gc, 50 + points[i].x - POINT_SIZE / 2,
                         70 + GRAPH_SIZE - points[i].y - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE, 0, 360 * 64);

                // Dibujar la línea al siguiente punto
                if (i < pointCount - 1) {
                    XDrawLine(display, window, gc, 50 + points[i].x, 70 + GRAPH_SIZE - points[i].y,
                              50 + points[i + 1].x, 70 + GRAPH_SIZE - points[i + 1].y);
                }
            }

            // Mostrar título
            XDrawString(display, window, gc, 75, 50 - 10, title, strlen(title));
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


