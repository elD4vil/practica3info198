#include "voids.h"

//##########################################################################
void sumatoria(const vector<int>& vec){
    
    int suma = 0;

    for(int val : vec){
        suma += val;
    }

    cout << "La sumatoria de elementos del vector ingresado es " << suma << endl;
}

//##########################################################################
void promedio(vector<int> vec){

    double prom = 0;
    double cant = vec.size();

    for(int val : vec){
        prom += val;
    }

    cout << "El promedio de elementos del vector ingresado es " << prom/cant << endl;
}

//##########################################################################
void moda(vector<int> vec){

    map<int, int> modas;

    for(int val : vec){
        modas[val] += 1;
    }

    int maxKey;
    int maxValue = -1;

    for(const auto &mod : modas){
        cout << mod.first << ": ";
        cout << mod.second << " " << endl;
        if(mod.second > maxValue){
            maxValue = mod.second;
            maxKey = mod.first;
        }
    }

    cout << "La moda del vector ingresado es el " << maxKey << " (" << maxValue << " repeticiones)" << endl;
}

//##########################################################################
void contar(vector<int> vec){
    int cant = vec.size();
    cout << "La cantidad de elementos del vector ingresado es " << cant << endl;
}

//##########################################################################
void crearArchivo(string path){

    ifstream archivo(path);

    if(archivo.is_open()){
        cout << "El archivo con ruta " << path << " ya existe" << endl;
        archivo.close();

    }else{
        ofstream crear(path);

        if(crear){
            crear.close();
            cout << "Archivo creado en " << path << endl;
        }else{
            cout << "No existe el directorio " << path << endl;
        }
    }
}

//##########################################################################
void agregarTexto(string texto, string path){

    ifstream archivo(path);

    if(!archivo.is_open()){
        cout << "El archivo con ruta " << path << " no existe" << endl;
        cout << "Ocupe opción 5" << endl;

        archivo.close();
        return;
    }

    string linea;

    int cont = 1;
    while(getline(archivo,linea)) cont++;

    archivo.close();

    ofstream escribir;
    escribir.open(path,ios::app);


    if(escribir){
        escribir << texto << endl;
        escribir.close();
        cout << "Texto '"<< texto << "' ingresado en la línea " << cont << " en el archivo con ruta " << path << endl;
        cont++;
    }else{
        cout << "\nNo se pudo abrir el archivo\nCERRANDO..." << endl;
        exit(1);
    }
}

//##########################################################################
void conteoPalabras(string iput, string oput){
    set<char> caracteres;

    const char* caracPath = getenv("CARAC");
    ifstream carac(caracPath);

    char caracter;
    while(carac >> caracter){
        caracteres.insert(caracter);
    }
    carac.close();

    ifstream archivo(iput);
    if(!archivo.is_open()){
        cout << "El archivo en dirección " << iput << " no existe" << endl;
        archivo.close();
        return;
    }

    archivo.seekg(0, ios::end);
    double fileSize = archivo.tellg();
    archivo.seekg(0,ios::beg);

    if(fileSize/1048576.0 < 1){
        cout << "El archivo de texto entregado con dirección " << iput << " tiene tamaño " << fileSize/1048576.0 << "MB" << endl;
        cout << "Debe pesar a lo menos 1MB" << endl;
        return;
    }

    string palabra, palabraDepurada;
    unordered_map<string,int> mapPalabras;

    while (archivo >> palabra) {
        palabraDepurada = "";
        for (char c : palabra) {
            if (caracteres.find(c) == caracteres.end()) palabraDepurada += tolower(c);
        }
        if (!palabraDepurada.empty()) {
            mapPalabras[palabraDepurada]++;
        }
    }

    archivo.close();

    ofstream escribir(oput, ios::binary);
    if(escribir){
        for(const auto &mape : mapPalabras){
            escribir << mape.first << ";" << mape.second << endl;
        }
    }else{
        cout << "\nNo se pudo abrir el archivo en dirección" << oput << "\nCERRANDO..." << endl;
        exit(1);
    }
    escribir.close();
    
    cout << "Palabaras del archivo con path " << iput << " y tamaño " << fileSize/1048576.0 << "MB contadas exitosamente," << endl;
    cout << "guardadas en " << oput << endl;
}

//##########################################################################
void generico(int opcion){
    cout << "La opción " << opcion << " aún no ha sido implementada" << endl;
}