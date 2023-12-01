#include "../../funciones/funciones.h"

vector<string> obtenerDirs(const string& path);
string obtenerDirBase(const string& path);
bool validarArchivo(const string& path);
map<string, vector<string>> obtenerDirectoriosConArchivos(const string& path);
void agregarArchivosaDirectorios(const string& rutaBase, const vector<string>& dirs, const map<string, vector<string>>& archivosPorDirectorio);


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

    agregarArchivosaDirectorios(obtenerdirBase,vectordeDirs, directoriosConArchivos);
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
            break;
        }
    }

    return dirbase;
}


void agregarArchivosaDirectorios(const string& rutaBase, const vector<string>& dirs, const map<string, vector<string>>& archivosPorDirectorio) {
    string rutaActual = rutaBase;
    try {
        for (const auto& dir : dirs) {
            rutaActual = rutaBase + "/" + dir;
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

        string comandoLn = "ln -s " + rutaBase + "/" + dirs.front() + " " + rutaBase + "/" + dirs.back();
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

