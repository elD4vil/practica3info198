#include "../../funciones/funciones.h"

//  /home/benjaminc/info198/practica3info198/progs/procesos/directorioLista/basedatos.dre


vector<string> obtenerDirs(const string& path);
string obtenerDirBase(const string& path);
bool validarArchivo(const string& path);
void crearDirectorioBase(const string& dirbase, const map<string, vector<string>>& directoriosConArchivos);
map<string, vector<string>> obtenerDirectoriosConArchivos(const string& path);

int main(int argc, char* argv[]){
    system("clear");
    string texto;
    cout << "Ingrese Path de archivo .dre: ";
    getline(cin, texto);
    bool archivoExiste = validarArchivo(texto);
    while(archivoExiste == false){
        cout << "Ingrese Path de archivo .dre: ";
        getline(cin, texto);
        archivoExiste = validarArchivo(texto);
    }
    string obtenerdirBase = obtenerDirBase(texto);
    vector<string> vectordeDirs = obtenerDirs(texto);
    map<string, vector<string>> directoriosConArchivos = obtenerDirectoriosConArchivos(texto);
    
    crearDirectorioBase(obtenerdirBase, directoriosConArchivos);
    
    string comandoLn = "ln -s " + obtenerdirBase + "/" + vectordeDirs.front() + " " + obtenerdirBase + "/" + vectordeDirs.back();
    system(comandoLn.c_str());
}


bool validarArchivo(const string& path) {
    // Validar existencia
    ifstream archivo(path);
    if (!archivo.good()) {
        cerr << "Error: El archivo no existe." << endl;
        return false;
    }

    // Validar extensión
    if (path.find(".dre") == string::npos) {
        cerr << "Error: Extensión de archivo incorrecta." << endl;
        return false;
    }

    return true;
}

string obtenerDirBase(const string& path) {
    ifstream archivo(path);
    string linea;

    string dirbase;

    while (getline(archivo, linea)) {
        istringstream iss(linea);
        string clave, valor;
        getline(iss, clave, '=');
        getline(iss, valor);

        if (clave == "dirbase") {
            dirbase = valor;
            break;  // Terminamos la búsqueda después de encontrar "dirbase"
        }
    }

    return dirbase;
}

void crearDirectorioBase(const string& dirbase, const map<string, vector<string>>& directoriosConArchivos) {
    try {
        // Crear el directorio base
        fs::create_directory(dirbase);

        // Crear subdirectorios dentro del directorio base
        for (const auto& [nombreDirectorio, archivos] : directoriosConArchivos) {
            string rutaDirectorio = dirbase + "/" + nombreDirectorio;

            // Verificar si el directorio ya existe
            if (!fs::exists(rutaDirectorio)) {
                fs::create_directory(rutaDirectorio);

                // Crear archivos en el directorio actual
                for (const auto& archivo : archivos) {
                    ofstream archivoStream(rutaDirectorio + "/" + archivo);
                    archivoStream << "Contenido del archivo " << archivo << endl;
                    archivoStream.close();
                }
            }
        }

        // Crear enlace virtual del último al primer directorio
        if (!directoriosConArchivos.empty()) {
            string rutaEnlace = dirbase + "/" + directoriosConArchivos.rbegin()->first;
            string rutaDestino = dirbase + "/" + directoriosConArchivos.begin()->first;

            // Verificar si el enlace simbólico ya existe
            if (!fs::exists(rutaDestino)) {
                fs::create_symlink(rutaEnlace, rutaDestino);
            }
        }

        cout << "Directorio base y subdirectorios creados con éxito." << endl;
    } catch (const fs::filesystem_error& e) {
        cerr << "Error al crear directorio base: " << e.what() << endl;
    }
    string comandoTree = "tree " + dirbase;
    system(comandoTree.c_str());

    
}

vector<string> obtenerDirs(const string& path) {
    ifstream archivo(path);
    string linea;

    vector<string> dirs;

    while (getline(archivo, linea)) {
        istringstream iss(linea);
        string clave, valor;
        getline(iss, clave, '=');
        getline(iss, valor);

        if (clave == "dirs") {
            istringstream dirsStream(valor);
            string dir;
            while (getline(dirsStream, dir, ';')) {
                dirs.push_back(dir);
            }
            break;  // Terminamos la búsqueda después de encontrar "dirs"
        }
    }

    return dirs;
}


map<string, vector<string>> obtenerDirectoriosConArchivos(const string& path) {
    ifstream archivo(path);
    string linea;

    map<string, vector<string>> directoriosConArchivos;

    while (getline(archivo, linea)) {
        istringstream iss(linea);
        string clave, valor;
        getline(iss, clave, '=');
        getline(iss, valor);

        if (clave.substr(0, 3) == "dir" && isdigit(clave.back())) { // Verificar si el último carácter es un número
            istringstream archivosStream(valor);
            string archivo;
            string nombreDirectorio = clave;
            vector<string> archivos;
            while (getline(archivosStream, archivo, ';')) {
                archivos.push_back(archivo);
            }

            directoriosConArchivos[nombreDirectorio] = archivos;
        }
    }

    return directoriosConArchivos;
}
