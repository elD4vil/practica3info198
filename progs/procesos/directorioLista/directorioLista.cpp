#include "../../funciones/funciones.h"

struct NodoDirectorio {
    std::string nombre;
    std::vector<std::string> archivos;
    NodoDirectorio* siguiente;

    NodoDirectorio(const std::string& nombre) : nombre(nombre), siguiente(nullptr) {}
};

class DirectorioCircular {
private:
    NodoDirectorio* inicio;
    fs::path base_path;

public:
    DirectorioCircular() : inicio(nullptr) {}

    void agregarDirectorio(const std::string& nombre, const std::vector<std::string>& archivos) {
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
            std::cout << dir_path << std::endl;
            fs::remove_all(dir_path); // Elimina el directorio si ya existe
        }
        fs::create_directory(dir_path); 

        for (const auto& archivo : temp->archivos) {
            std::ofstream ofs(dir_path / archivo);
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
            std::cout << "El directorio está vacío." << std::endl;
            return;
        }

        NodoDirectorio* temp = inicio;
        do {
            std::cout << temp->nombre << std::endl;
            temp = temp->siguiente;
        } while (temp != inicio);
    }

    void leerArchivo(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "No se pudo abrir el archivo." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key;
            std::getline(iss, key, '=');
            std::string value;
            std::getline(iss, value);

            if (key == "dirbase") {
                base_path = value;
            } else if (key == "dirs") {
                std::istringstream iss_dirs(value);
                std::string dir;
                while (std::getline(iss_dirs, dir, ';')) {
                    agregarDirectorio(dir, {});
                }
            } else {
                NodoDirectorio* temp = inicio;
                do {
                    if (temp->nombre == key) {
                        std::istringstream iss_files(value);
                        std::string file;
                        while (std::getline(iss_files, file, ';')) {
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
            std::cout << "La lista está vacía." << std::endl;
            return;
        }

        NodoDirectorio* temp = inicio;
        do {
            std::cout << "Directorio: " << temp->nombre << std::endl;
            std::cout << "Archivos: ";
            for (const auto& archivo : temp->archivos) {
                std::cout << archivo << " ";
            }
            std::cout << std::endl;
            temp = temp->siguiente;
        } while (temp != inicio);
    }

};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " path \n";
        return 1;
    }
    string path = argv[1];
    cout<<path<<endl;
    DirectorioCircular directorio;

    directorio.leerArchivo(path);
    directorio.crearJerarquiaDirectorios();

    std::cout << "Contenido del directorio:" << std::endl;
    directorio.mostrarDirectorios();
    directorio.recorrerNodos();
    return 0;
}