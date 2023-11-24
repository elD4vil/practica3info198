#include "../../funciones/funciones.h"

void crearMap(map<string, vector<pair<string,int>>>& palabrasData, const string& path){

    ifstream file(path);
    string line, palabra;

    while(getline(file, line)){

        istringstream iss(line);
        string segmento;

        int count = 1;
        while(getline(iss, segmento, ':')){

            if(count == 1){
                palabra = segmento;
                palabrasData[palabra] = vector<pair<string, int>>();

            }else{

                istringstream issArchivos(segmento);
                string tupla;

                while(getline(issArchivos, tupla, ';')){

                    istringstream issTupla(tupla);
                    string valor, archivo;
                    int cantidad;

                    int acc = 1;
                    while(getline(issTupla, valor, ',')){
                        if(acc == 1) archivo = valor.substr(1);
                        else cantidad = stoi(valor.substr(0, valor.size() - 1));
                        acc++;
                    }
                    
                    palabrasData[palabra].push_back(make_pair(archivo, cantidad));
                }
            }
            count++;
        }
    }

    file.close();
}


vector<pair<string, int>> buscarDocumentosTopK(const map<string, vector<pair<string, int>>>& data, const string& texto, int topk) {

    vector<pair<string, int>> resultados;

    string textoMinusculas = texto;
    istringstream iss(textoMinusculas);
    vector<string> words;

    string word;
    while (iss >> word) {
        words.push_back(word);
    }

    // erase para eliminar los elementos duplicados del vector
    auto it = unique(words.begin(), words.end());    
    words.erase(it, words.end());

    auto toLower = [](string &word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
    };
    
    for_each(words.begin(), words.end(), toLower);

    transform(textoMinusculas.begin(), textoMinusculas.end(), textoMinusculas.begin(), ::tolower);
    for (const auto &word : words) {
        for (const auto& palabraData : data) {
            string palabra = palabraData.first;
            string palabraMinusculas = palabra;
            transform(palabraMinusculas.begin(), palabraMinusculas.end(), palabraMinusculas.begin(), ::tolower);
            if (!palabraMinusculas.empty() && palabraMinusculas.back() == ':') {
                palabraMinusculas.erase(palabraMinusculas.size() - 1);
            }
            
            if (palabraMinusculas == word) {
                for (const auto& par : palabraData.second) {
                    resultados.push_back(par);
                }
            }
        }
    }

    unordered_map<string, int> sumaResultados;
    unordered_map<string, int> cantidad;


    for (const auto& par : resultados) {
        sumaResultados[par.first] += par.second;
        cantidad[par.first] += 1;
    }

    vector<pair<string, int>> resultadosFinales;
    vector<string> cantidadFinales;


    for (const auto& par : cantidad) {
        if (par.second == words.size()) {
            cantidadFinales.emplace_back(par.first);
        }
    }

    for (const auto& par : sumaResultados) {
        for(int i = 0; i < cantidadFinales.size(); i++){
            if(par.first == cantidadFinales[i]){
                resultadosFinales.emplace_back(par.first, par.second);
            }
        }
    }

    sort(resultadosFinales.begin(), resultadosFinales.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    if (resultadosFinales.size() > topk) {
        resultadosFinales.resize(topk);
    }

    return resultadosFinales;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <archivoSalida> <cantidad_topk> " << endl;
        return 1;
    }

    string FILE = argv[1];
    int topk = stoi(argv[2]);

    map<string, vector<pair<string, int>>> data; //estructura de datos para cargar en memoria los datos del file.idx
    crearMap(data, FILE); //carga los datos de file.idx en memoria

    bool sigue = true;
    string texto;
    string seguir;

    pid_t pid = getpid();

    while (sigue) {
        system("clear");
        cout << "BUSCADOR BASADO EN INDICE INVERTIDO (" << pid << ")\n" << endl;
        cout << "Los top " << topk << " documentos serán mostrados.\n" << endl;
        cout << "Escriba texto a buscar: ";
        getline(cin, texto);

        // Realizar la búsqueda y mostrar los resultados
        vector<pair<string, int>> documentosOrdenados = buscarDocumentosTopK(data, texto, topk);
        cout << "\nRespuesta:" << endl;
        int i = 1;

        for (const auto& documento : documentosOrdenados) {
            cout << " ";
            cout << i << ") " << documento.first << ", " << documento.second << endl;
            ++i;
        }

        cout << "\n¿Desea Salir? (S/N): ";
        getline(cin, seguir);

        // Verificar la opción de salida del usuario
        if (seguir == "S" || seguir == "s") {
            sigue = false;
        } else if (seguir != "N" && seguir != "n") {
            sigue = true;
        }
    }

    return 0;
}