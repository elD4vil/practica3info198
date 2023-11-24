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
#include <algorithm>
#include <mutex>
#include <chrono>

using namespace std;
namespace fs = filesystem;

map<int, string> crearMenu(vector<int> &opUsu, map<string, string> users, string user);
map<string, string> leerUsuarios();
vector<int> crearVector(string vec);
void crearVariablesEntorno();
string rutaNom(string pathNom);
void printMenu(vector<int> opUsu, map<string, string> users, string user);
vector<string> txtFile(const char* pathIn, const char* extension);

#endif