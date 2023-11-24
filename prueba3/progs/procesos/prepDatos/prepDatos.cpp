#include "../../funciones/funciones.h"

mutex mtx;
// Función para contar palabras en un archivo y almacenar los resultados en un mapa
void contarPalabrasEnArchivo(const string& filePath, const string& outPath,int threadID){

    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filePath << endl;
    }

    file.seekg(0, ios::end);
    double fileSize = file.tellg();
    file.seekg(0,ios::beg);

    if(fileSize/1048576.0 < 1){
        cout << "El archivo de texto entregado con dirección " << filePath << " tiene tamaño " << fileSize/1048576.0 << "MB" << endl;
        cout << "Debe pesar a lo menos 1MB" << endl;
        exit(1);
    }

    map<string, int> wordCountMap;

    string word;
    while (file >> word) {
        // Eliminar caracteres de puntuación al inicio y al final de la palabra
        word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());

        // Convertir la palabra a minúsculas
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        // Verificar si la palabra contiene solo caracteres alfabéticos
        if (!word.empty() && all_of(word.begin(), word.end(), ::isalpha)) {
            wordCountMap[word]++;
        }
    }

    file.close();

    // Escribir los resultados en archivos de salida
    fs::path inputFilePath(filePath);
    fs::path outputPath(outPath);
    outputPath /= inputFilePath.filename();

    ofstream outputFile(outputPath);
    if (outputFile.is_open()) {
        outputFile << "Palabra, Cantidad\n";
        for (const auto& entry : wordCountMap) {
            outputFile << entry.first << ", " << entry.second << '\n';
        }
        outputFile.close();
    } else {
        cerr << "No se pudo crear el archivo de salida: " << outputPath << endl;
    }

    mtx.lock();
    cout << "proceso pid=" << getpid() << " - archivo procesado " << filePath << " por el thread " << threadID << endl;
    mtx.unlock();
}

int main(int argc, char* argv[]){

    const char* extension = argv[1];
    const char* path_in = argv[2];
    const char* path_out = argv[3];
    unsigned int amount_threads = atoi(argv[4]);

    vector<string> txtFiles = txtFile(path_in, extension);

    if(txtFiles.size() < 20) {
        cout << "Deben haber por lo menos 20 archivo para poder ejecutar el índice invertido." << endl;
        return 0;
    }

    vector<thread> threads;

    for (int threadID = 0; threadID < amount_threads; ++threadID) {
        threads.push_back(thread([&, threadID]() {
            for (size_t i = threadID; i < txtFiles.size(); i += amount_threads) {
                contarPalabrasEnArchivo(txtFiles[i], path_out, threadID);
            }
        }));
    }

    // Esperar a que todos los hilos terminen
    for (thread& thread : threads) {
        thread.join();
    }

    cout << "Cantidad de threads: " << threads.size() << endl;

    return 1;
}