#include "../../funciones/funciones.h"

void crearIDX(map<string, vector<pair<string,int>>>& palabrasData, const string& filePath){

    ifstream inputFile(filePath);
    string line;

    while(getline(inputFile, line)){
        istringstream iss(line);
        string word;
        int count;

        if(iss >> word >> count){
            if(!word.empty() && word.back() == ',') word.pop_back();
            size_t found = filePath.find_last_of("/\\");
            palabrasData[word].push_back(make_pair(filePath.substr(found+1), count));
        }
    }

    inputFile.close();
}

void escribirIDX(map<string, vector<pair<string,int>>>& palabrasData, const string& outPath){

    ofstream outputFile(outPath);

    for(const auto &entry : palabrasData){
        outputFile << entry.first << ":";
        
        for(const auto &data : entry.second){
            outputFile << "(" << data.first << "," << data.second << ");";
        }

        outputFile << endl;
    }

    outputFile.close();

}

int main(int argc, char* argv[]){

    const char* path_idx = argv[1];
    const char* path_out = argv[2];

    vector<string> txtFiles = txtFile(path_out, ".txt");
    map<string, vector<pair<string,int>>> palabrasData;

    for(const string &filePath : txtFiles){
        crearIDX(palabrasData, filePath);
    }
    
    escribirIDX(palabrasData, path_idx);

    cout << "El proceso pid=" << getpid() << " generó el archivo=" << path_idx << endl;

    return 1;
}