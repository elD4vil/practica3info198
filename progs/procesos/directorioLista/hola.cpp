#include <iostream>
#include <string>

struct NodoDirectorio {
    std::string nombre;
    NodoDirectorio* siguiente;

    NodoDirectorio(const std::string& nombre) : nombre(nombre), siguiente(nullptr) {}
};

class ListaEnlazadaDirectorios {
private:
    NodoDirectorio* primero;
    NodoDirectorio* ultimo;

public:
    ListaEnlazadaDirectorios() : primero(nullptr), ultimo(nullptr) {}

    void agregarDirectorio(const std::string& nombreDirectorio) {
        NodoDirectorio* nuevoNodo = new NodoDirectorio(nombreDirectorio);

        if (!primero) {
            // Lista vacía, el nuevo nodo es el primero y el último
            primero = ultimo = nuevoNodo;
        } else {
            // Agregar el nuevo nodo al final de la lista
            ultimo->siguiente = nuevoNodo;
            ultimo = nuevoNodo;
        }
    }

    void mostrarLista() {
        NodoDirectorio* actual = primero;
        while (actual) {
            std::cout << actual->nombre << " ";
            actual = actual->siguiente;
        }
        std::cout << std::endl;
    }

    void cerrarCirculo() {
        if (ultimo) {
            ultimo->siguiente = primero;
        }
    }
};

int main() {
    ListaEnlazadaDirectorios lista;

    // Agregar algunos directorios
    lista.agregarDirectorio("dir1");
    lista.agregarDirectorio("dir2");
    lista.agregarDirectorio("dir3");

    // Mostrar la lista
    lista.mostrarLista();

    return 0;
}

