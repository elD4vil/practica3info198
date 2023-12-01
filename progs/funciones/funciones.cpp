#include "funciones.h"

//##########################################################################
void crearVariablesEntorno(){
    ifstream envFile(".env");
    string line;

    if (envFile.is_open()) {
        while (getline(envFile, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != string::npos) {
                string key = line.substr(0, delimiterPos);
                string value = line.substr(delimiterPos + 1);


                if(key == "AMOUNT_THREADS" || key == "EXTENTION" || key == "TOPK"){
                    if(key == "AMOUNT_THREADS"){
                        try{
                            int valor = atoi(value.c_str());
                            if(valor <= 0 || valor > 10 || valor > thread::hardware_concurrency()){
                                cout << "\nEl valor de " << key << " no puede ser un valor negativo, 0 ni mayor a 10.\n" << endl;
                                exit(1);      
                            }
                        }catch (const exception e){
                            cout << "\nEl valor de " << key << " debe ser un entero.\n" << endl;
                            exit(1);
                        }
                    }else if(key == "TOPK"){
                        try{
                            int valor = atoi(value.c_str());
                            if(valor <= 4){
                                cout << "\nEl valor de " << key << " debe ser un número entero mayor a 4.\n" << endl;
                                exit(1);      
                            }
                        }catch (const exception e){
                            cout << "\nEl valor de " << key << " debe ser un entero.\n" << endl;
                            exit(1);
                        }                   
                    }

                    setenv(key.c_str(), value.c_str(), 1);

                }else{
                    ifstream existencia(value);

                    if(existencia){
                        setenv(key.c_str(), value.c_str(), 1);
                    }else{
                        cout << "Se encontro una variable de entorno con una ruta que no existe; " << value << "\nCERRANDO..." << endl;
                        exit(1);
                    }
                }
            }
        }
        envFile.close();

        const char* path_files_in = getenv("PATH_FILES_IN");
        const char* path_files_out = getenv("PATH_FILES_OUT");

        if (path_files_in != nullptr && path_files_out != nullptr) {
            string str_path_files_in(path_files_in);
            string str_path_files_out(path_files_out);

            if(str_path_files_out == str_path_files_in){
                cout << "Las rutas de PATH_FILES_IN y PATH_FILES_OUT deben ser diferentes." << endl;
                exit(1);
            }
        }

    }
}

//##########################################################################
map<int, string> crearMenu(vector<int> &opUsu, map<string, string> users, string user){

    ifstream file;
    string linea;

    map<int, string> funcionesMap;

    const char* menuPath = getenv("MENUS");
    const char* profilePath = getenv("PERFILES");

    map<string, vector<string>> profiles;

    file.open(profilePath,ios::in);

    while(getline(file,linea)){
        istringstream ss(linea);
        string nombre;
        string funciones;

        getline(ss,nombre,';');
        
        while(getline(ss,funciones,',')){
            profiles[nombre].push_back(funciones);   
        }
    }
    file.close();

    file.open(menuPath,ios::in);
    int acc;
    int opcMenu;
    string labelMenu;
    string funcionMenu;
    while(getline(file,linea)){

        istringstream ss(linea);
        string segmento;

        acc = 1;
        while(getline(ss,segmento,',')){

            if(acc == 1){
                try{
                    opcMenu = stoi(segmento);
                }catch (const exception& e){
                    cout << "\nSe ingresó " << opcMenu <<" un valor no entero como opción de menú asociada\nCERRANDO..." << endl;
                    exit(1);
                }
            }
            if(acc == 2) labelMenu = segmento;
            if(acc == 3) funcionMenu = segmento;

            acc++;
        }

        if(funcionesMap.find(opcMenu) != funcionesMap.end() || acc != 4){
            cout << "\nSe ingresaron incorrectamente las opciones del menú\nCERRANDO..." << endl;
            exit(1);
        }

        funcionesMap[opcMenu] = funcionMenu;

        if(find(profiles[users[user]].begin(), profiles[users[user]].end(), funcionMenu) != profiles[users[user]].end() || opcMenu == 0){
            opUsu.push_back(opcMenu);
            cout << opcMenu << ". " << labelMenu << endl;
        }else{
            cout << "\x1B[9m" << opcMenu << ". " << labelMenu << "\x1B[0m" << endl;
        }
    }
    cout << endl;
    
    file.close();

    return funcionesMap;
}

//##########################################################################
void printMenu(vector<int> opUsu, map<string, string> users, string user){
    ifstream file;
    string linea;
    int acc;

    const char* menuPath = getenv("MENUS");

    cout << "\n     MENU PID=" << getpid() << endl;
    cout << "      [" << users[user] << "] " << user << endl;
    cout << "-----------------------------" << endl;

    file.open(menuPath,ios::in);

    while(getline(file,linea)){

        istringstream ss(linea);
        string segmento;
        int menu;

        acc = 1;
        while(getline(ss,segmento,',')){

            if(acc == 1){
                menu = stoi(segmento);
                if(menu == 0) cout << segmento << ". ";
                else if(find(opUsu.begin(), opUsu.end(), menu) != opUsu.end()) cout << segmento << ". ";      
                else cout << "\x1B[9m" << segmento << ". \x1B[0m";

            }else if(acc == 2){

                if(menu == 0) cout << segmento << endl;
                else if(find(opUsu.begin(), opUsu.end(), menu) != opUsu.end()) cout << segmento << endl;      
                else cout << "\x1B[9m" << segmento << "\x1B[0m" << endl;
            }
            acc++;
        }
    }

    file.close();
}

//##########################################################################
map<string, string> leerUsuarios(){

    ifstream file;
    string linea;
    string user;
    int acc;

    map<string,string> usuarios;

    const char* varName = "USUARIOS";
    const char* userPath = getenv(varName);

    file.open(userPath,ios::in);

    while(getline(file,linea)){
        istringstream ss(linea);
        string segmento;

        acc = 1;
        while(getline(ss,segmento,';')){
            if(acc==1) user = segmento;
            if(acc==2) usuarios[user] = segmento;
            if(acc>3){
                cout << "\nSe ingresaron incorrectamente los usuarios\nCERRANDO..." << endl;
                exit(1);
            }
            acc++;
        }
    }
    
    file.close();

    return usuarios;
}

//##########################################################################
vector<int> crearVector(string vec){

    vector<int> vectorCreado;

    stringstream ss(vec);
    string numero;

    while(getline(ss,numero,',')){
        int valor;
        try{
            valor = stoi(numero);
        }catch (const exception& e){
            cout << "\nIngreso erroneo del flag -v\nCERRANDO..." << endl;
            exit(1);
        }
        vectorCreado.push_back(valor);
    }

    return vectorCreado;
}

//##########################################################################
string rutaNom(string pathNom){
    istringstream ss(pathNom);
    string path;
    string nombre;

    string segmento;

    int pos = 1;
    while(getline(ss,segmento,',')){

        if(pos>2){
            cout << "\nIngreso erroneo del flag -f\nCERRANDO..." << endl;
            exit(1);
        }
        if(pos == 1) path = segmento;
        else nombre = segmento;

        pos++;
    }

    return path + "/" + nombre;
}

//##########################################################################
vector<string> txtFile(const char* pathIn, const char* extension){

    vector<string> txtFiles;

    // Bucle para buscar archivos .txt en la carpeta y sus subcarpetas
    for (const auto& entry : fs::recursive_directory_iterator(pathIn)) {
        if (!fs::is_directory(entry)) {
            if (entry.path().extension() == extension) {
                txtFiles.push_back(entry.path().string());
            }else{
                cout << "El archivo con path " << entry.path() << " no tiene la extension " << extension << endl;
            }
        }
    }

    return txtFiles;
}

bool validarExtension(string ext, string path){

    size_t dotIndex = path.find_last_of('.');

    if (dotIndex == string::npos || path.substr(dotIndex + 1) != ext) {
        cout << "El archivo no tiene la extensión correcta (" << ext << ").\n";
        return false;
    }

    return true;
}

bool validarExistenciaArchivo(string path){
    if (!fs::exists(path)) {
        cout << "El archivo con path " << path << " no existe.\n";
        return false;
    }
    return true;
}

//##########################################################################
bool validarFormatoMensaje(const json& mensaje) {

    if (!mensaje.contains("dirBase") || !mensaje["dirBase"].is_string()) {
        cerr << "Error: La clave 'dirBase' no está presente o no es de tipo string.\n";
        return false;
    }

    for (const auto& subobjeto : mensaje["objetos"]) {
        if (!validarObjeto(subobjeto)) {
            return false;
        }
    }
    
    return true;
}

bool validarObjeto(const json& objeto) {
    if (!objeto.contains("tipo") || !objeto["tipo"].is_string()) {
        cerr << "Error: La clave 'tipo' no está presente o no es de tipo string.\n";
        return false;
    }
    if (objeto["tipo"] != "directorio" && objeto["tipo"] != "archivo"){
        cerr << "Error: La clave 'tipo' no tiene el formato correcto.\n";
        return false;
    }

    if (!objeto.contains("nombre") || !objeto["nombre"].is_string()) {
        cerr << "Error: La clave 'nombre' no está presente o no es de tipo string.\n";
        return false;
    }

    if (objeto["tipo"] == "directorio" && (!objeto.contains("objetos") || !objeto["objetos"].is_array())) {
        cerr << "Error: La clave 'objetos' no está presente o no es de tipo array en un directorio.\n";
        return false;
    }

    if (objeto["tipo"] == "archivo" && (!objeto.contains("contenido") || !objeto["contenido"].is_string())) {
        cerr << "Error: La clave 'contenido' no está presente o no es de tipo string en un archivo.\n";
        return false;
    }

    if (objeto["tipo"] == "archivo" && (!objeto.contains("permisos") || !objeto["permisos"].is_array())) {
        cerr << "Error: La clave 'permisos' no está presente o no es de tipo array en un archivo.\n";
        return false;
    }

    if (objeto["tipo"] == "directorio") {
        for (const auto& subobjeto : objeto["objetos"]) {
            if (!validarObjeto(subobjeto)) {
                return false;
            }
        }
    }

    return true;
}
//##########################################################################

bool validarArchivoGrafico(const string& graphPath){
    ifstream file(graphPath);
    string linea;
    int acc, subAcc;
    bool primera = true;

    while(getline(file,linea)){
        istringstream ss(linea);
        string segmento;

        if(primera){
            acc = 0;
            while(getline(ss,segmento,':')){
                if(acc == 0){
                    if(segmento != "titulo"){
                        cout << "La primera línea no tiene formato 'titulo:<string>'" << endl;
                        return false;
                    }
                }
                acc++;
            }
            primera = false;

        }else{
            acc = 0;
            while(getline(ss,segmento,',')){
                istringstream sss(segmento);
                string punto;

                subAcc = 0;
                while(getline(sss,punto,':')){
                    // verificación coordenada
                    if(subAcc == 0){
                        if(acc == 0){
                            if(punto != "x"){
                                cout << "Existe un punto que no tiene coordenada 'x'" << endl;
                                return false;
                            }
                        }else if(acc == 1){
                            if(punto != "y"){
                                cout << "Existe un punto que no tiene coordenada 'y'" << endl;
                                return false;
                            }
                        }
                    }
                    // verificación punto
                    else if(subAcc == 1){
                        try{
                            int valor = stoi(punto);
                        }catch (const exception& e){
                            cout << "Existe un punto que no tiene valor entero" << endl;
                            return false;
                        }
                    }
                    subAcc++;
                }
                acc++;
            }
        }
    }
    return true;
}
