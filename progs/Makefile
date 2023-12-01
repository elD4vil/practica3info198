CC = g++ -std=c++17

SOURCES = main.cpp voids/voids.cpp funciones/funciones.cpp
HEADERS = voids/voids.h funciones/funciones.h
EXECUTABLE = prog

PREPIDX_SOURCES = procesos/prepDatos/prepDatos.cpp funciones/funciones.cpp
PREPIDX_HEADERS = funciones/funciones.h
PREPIDX_EXECUTABLE = procesos/prepDatos/app

CREARIDX_SOURCES = procesos/crearIndice/crearIndice.cpp funciones/funciones.cpp
CREARIDX_HEADERS = funciones/funciones.h
CREARIDX_EXECUTABLE = procesos/crearIndice/app

BUSCADOR_SOURCES = procesos/buscador/llamarBuscador.cpp funciones/funciones.cpp
BUSCADOR_HEADERS = funciones/funciones.h
BUSCADOR_EXECUTABLE = procesos/buscador/app

ARBOL_SOURCES = procesos/directorioArbol/directorioArbol.cpp funciones/funciones.cpp
ARBOL_HEADERS = funciones/funciones.h
ARBOL_EXECUTABLE = procesos/directorioArbol/app

LISTA_SOURCES = procesos/directorioLista/directorioLista.cpp funciones/funciones.cpp
LISTA_HEADERS = funciones/funciones.h
LISTA_EXECUTABLE = procesos/directorioLista/app

GRAFICO_SOURCES = procesos/grafico/grafico.cpp funciones/funciones.cpp
GRAFICO_HEADERS = funciones/funciones.h
GRAFICO_EXECUTABLE = procesos/grafico/app

all: $(EXECUTABLE) $(PREPIDX_EXECUTABLE) $(CREARIDX_EXECUTABLE) $(BUSCADOR_EXECUTABLE) $(ARBOL_EXECUTABLE) $(LISTA_EXECUTABLE) $(GRAFICO_EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) -o $(EXECUTABLE)

$(PREPIDX_EXECUTABLE): $(PREPIDX_SOURCES) $(PREPIDX_HEADERS)
	$(CC) $(PREPIDX_SOURCES) -o $(PREPIDX_EXECUTABLE)

$(CREARIDX_EXECUTABLE): $(CREARIDX_SOURCES) $(CREARIDX_HEADERS)
	$(CC) $(CREARIDX_SOURCES) -o $(CREARIDX_EXECUTABLE)

$(BUSCADOR_EXECUTABLE): $(BUSCADOR_SOURCES) $(BUSCADOR_HEADERS)
	$(CC) $(BUSCADOR_SOURCES) -o $(BUSCADOR_EXECUTABLE)

$(ARBOL_EXECUTABLE): $(ARBOL_SOURCES) $(ARBOL_HEADERS)
	$(CC) $(ARBOL_SOURCES) -o $(ARBOL_EXECUTABLE)

$(LISTA_EXECUTABLE): $(LISTA_SOURCES) $(LISTA_HEADERS)
	$(CC) $(LISTA_SOURCES) -o $(LISTA_EXECUTABLE)

$(GRAFICO_EXECUTABLE): $(GRAFICO_SOURCES) $(GRAFICO_HEADERS)
	$(CC) $(GRAFICO_SOURCES) -o $(GRAFICO_EXECUTABLE) -lX11

clean:
	rm -rf $(EXECUTABLE) $(PREPIDX_EXECUTABLE) $(CREARIDX_EXECUTABLE) $(BUSCADOR_EXECUTABLE) $(ARBOL_EXECUTABLE) $(LISTA_EXECUTABLE) $(GRAFICO_EXECUTABLE)