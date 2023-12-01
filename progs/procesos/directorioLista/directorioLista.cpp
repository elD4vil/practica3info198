#include "../../funciones/funciones.h"

vector<string> obtenerDirs(const string& path);
string obtenerDirBase(const string& path);
map<string, vector<string>> obtenerDirectoriosConArchivos(const string& path);
void agregarArchivosaDirectorios(const string& rutaBase, const vector<string>& dirs, const map<string, vector<string>>& archivosPorDirectorio);


int main(int argc, char* argv[]){
    
    string texto = argv[1];

    string obtenerdirBase = obtenerDirBase(texto);
    fs::create_directory(obtenerdirBase);
    vector<string> vectordeDirs = obtenerDirs(texto);
    map<string, vector<string>> directoriosConArchivos = obtenerDirectoriosConArchivos(texto);

    agregarArchivosaDirectorios(obtenerdirBase,vectordeDirs,directoriosConArchivos);

    cout << "Sistema de directorio basado en lista circular creado exitosamente con directorio base en " << obtenerdirBase << endl;
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
            break;
        }
    }

    return dirbase;
}


void agregarArchivosaDirectorios(const string& rutaBase, const vector<string>& dirs, const map<string, vector<string>>& archivosPorDirectorio) {
    try {
        string rutaActual = rutaBase;
        for (const auto& dir : dirs) {
            rutaActual = rutaActual + "/" + dir;
            if (fs::exists(rutaActual)) {
                fs::remove_all(rutaActual); 
            }
            fs::create_directory(rutaActual);
            auto it = archivosPorDirectorio.find(dir);
            if (it != archivosPorDirectorio.end()) {
                for (const auto& archivo : it->second) {
                    string rutaArchivo = rutaActual + "/" + archivo;
                    ofstream archivoStream(rutaArchivo);
                    archivoStream.close();
                }
            }
        }

        string comandoLn = "ln -s " + rutaBase + "/" + dirs.front() + " " + rutaActual;
        system(comandoLn.c_str());

    } catch (const fs::filesystem_error& e) {
        cerr << "Error al agregar archivos a directorios: " << e.what() << endl;
    }
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