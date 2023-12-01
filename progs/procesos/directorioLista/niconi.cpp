#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <sstream>

namespace fs = filesystem;
using namespace std;
struct NodoDirectorio {
    string nombre;
    vector<string> archivos;
    NodoDirectorio* siguiente;

    NodoDirectorio(const string& nombre) : nombre(nombre), siguiente(nullptr) {}
};

class DirectorioCircular {
private:
    NodoDirectorio* inicio;
    fs::path base_path;

public:
    DirectorioCircular() : inicio(nullptr) {}

    void agregarDirectorio(const string& nombre, const vector<string>& archivos) {
        NodoDirectorio* nuevoDirectorio = new NodoDirectorio(nombre);
        nuevoDirectorio->archivos = archivos;

        if (inicio == nullptr) {
            inicio = nuevoDirectorio;
            inicio->siguiente = inicio; 
        } else {
            NodoDirectorio* temp = inicio;
            while (temp->siguiente != inicio) {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoDirectorio; 
            nuevoDirectorio->siguiente = inicio; 
        }
    }

    void crearJerarquiaDirectorios() {
    NodoDirectorio* temp = inicio; 
    fs::path dir_path = base_path;
    if (!fs::exists(dir_path)) {
        fs::create_directories(dir_path); // Crea la ruta base si no existe
    }
    do {
        dir_path /= temp->nombre; 

        if (fs::exists(dir_path)) {
            cout << dir_path << endl;
            fs::remove_all(dir_path); // Elimina el directorio si ya existe
        }
        fs::create_directory(dir_path); 

        for (const auto& archivo : temp->archivos) {
            ofstream ofs(dir_path / archivo);
            ofs.close();
        }

        temp = temp->siguiente; 
    } while (temp != inicio);

    // Crear el enlace simbólico en el último directorio
    fs::path link_path = dir_path / inicio->nombre; 
    if (fs::exists(link_path)) {
        fs::remove(link_path); // Elimina el enlace si ya existe
    }
    fs::create_directory_symlink(base_path / inicio->nombre, link_path);
}



    void mostrarDirectorios() {
        if (inicio == nullptr) {
            cout << "El directorio está vacío." << endl;
            return;
        }

        NodoDirectorio* temp = inicio;
        do {
            cout << temp->nombre << endl;
            temp = temp->siguiente;
        } while (temp != inicio);
    }

    void leerArchivo(const string& path) {
        ifstream file(path);
        if (!file.is_open()) {
            cout << "No se pudo abrir el archivo." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string key;
            getline(iss, key, '=');
            string value;
            getline(iss, value);

            if (key == "dirbase") {
                base_path = value;
            } else if (key == "dirs") {
                istringstream iss_dirs(value);
                string dir;
                while (getline(iss_dirs, dir, ';')) {
                    agregarDirectorio(dir, {});
                }
            } else {
                NodoDirectorio* temp = inicio;
                do {
                    if (temp->nombre == key) {
                        istringstream iss_files(value);
                        string file;
                        while (getline(iss_files, file, ';')) {
                            temp->archivos.push_back(file);
                        }
                        break;
                    }
                    temp = temp->siguiente;
                } while (temp != inicio);
            }
        }

        file.close();
    }
    void recorrerNodos() {
        if (inicio == nullptr) {
            cout << "La lista está vacía." << endl;
            return;
        }

        NodoDirectorio* temp = inicio;
        do {
            cout << "Directorio: " << temp->nombre << endl;
            cout << "Archivos: ";
            for (const auto& archivo : temp->archivos) {
                cout << archivo << " ";
            }
            cout << endl;
            temp = temp->siguiente;
        } while (temp != inicio);
    }

};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " path \n";
        return 1;
    }
    string path = argv[1];
    cout<<path<<endl;
    DirectorioCircular directorio;

    directorio.leerArchivo(path);
    directorio.crearJerarquiaDirectorios();

    cout << "Contenido del directorio:" << endl;
    directorio.mostrarDirectorios();
    directorio.recorrerNodos();
    return 0;
}