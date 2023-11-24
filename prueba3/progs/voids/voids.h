#ifndef VOIDS_H
#define VOIDS_H

#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <unistd.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>
#include <cctype>

using namespace std;

void sumatoria(const vector<int>& vec);
void promedio(vector<int> vec);
void moda(vector<int> vec);
void contar(vector<int> vec);
void crearArchivo(string path);
void agregarTexto(string texto, string path);
void generico(int opcion);
void conteoPalabras(string iput, string oput);

#endif