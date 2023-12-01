#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <filesystem>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <mutex>
#include <chrono>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace std;
namespace fs = filesystem;

map<int, string> crearMenu(vector<int> &opUsu, map<string, string> users, string user);
map<string, string> leerUsuarios();
vector<int> crearVector(string vec);
void crearVariablesEntorno();
string rutaNom(string pathNom);
void printMenu(vector<int> opUsu, map<string, string> users, string user);
vector<string> txtFile(const char* pathIn, const char* extension);
bool validarExtension(string ext, string path);
bool validarExistenciaArchivo(string path);
bool validarFormatoMensaje(const json& mensaje);
bool validarObjeto(const json& objeto);
bool validarArchivoGrafico(const string& graphPath);

#endif