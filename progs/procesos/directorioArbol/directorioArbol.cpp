#include "../../funciones/funciones.h"

int crearArbolDirectorio(const json& objetos, string path){
    for (const auto& objeto : objetos) {
        if(objeto["tipo"] == "directorio"){
            string nombre = objeto["nombre"];
            string nuevoPath = path + nombre + "/";
            string comando = "mkdir " + nuevoPath;
            system(comando.c_str());

            crearArbolDirectorio(objeto["objetos"], nuevoPath);

        }else{
            string nombre = objeto["nombre"];
            string nuevoPath = path + nombre;
            string contenido = objeto["contenido"];
            auto permisos = objeto["permisos"];

            ofstream archivo(nuevoPath);
            archivo << contenido;
            archivo.close();

            mode_t permisosNum = 0;
            for (const auto& permiso : permisos) {
                if (permiso == "lectura") {
                    permisosNum |= S_IRUSR | S_IRGRP | S_IROTH;
                } else if (permiso == "escritura") {
                    permisosNum |= S_IWUSR | S_IWGRP | S_IWOTH;
                }
            }

            if (chmod(nuevoPath.c_str(), permisosNum) != 0) {
                cerr << "Error al establecer los permisos." << endl;
                exit(1);
            }
        }
    }

    return 1;

}

int main(int argc, char* argv[]){

    json dirArJSON = json::parse(argv[1]);

    string base = dirArJSON["dirBase"];
    string comando = "mkdir " + base;
    system(comando.c_str());

    crearArbolDirectorio(dirArJSON["objetos"], base);

    cout << "Árbol de directorios creado exitosamente con directorio base en " << base << endl;

    return 1;
}