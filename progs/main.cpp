#include "funciones/funciones.h"
#include "voids/voids.h"

int main(int argc, char **argv){

    system("clear");
    crearVariablesEntorno();
    
    map<string,string> usuarios;
    usuarios = leerUsuarios();

    vector<int> vec;   
    string usuarioSeleccionado;
    string path, texto, input, output;

    int c;
    while ((c = getopt(argc, argv, "u:v:f:t:i:o:")) != -1) {
        switch (c) {
            case 'u':
                usuarioSeleccionado = optarg;
                break;
            case 'v':
                vec = crearVector(optarg);
                break;
            case 'f':
                path = optarg;
                break;
            case 't':
                texto = optarg;
                break;
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case '?':
                cout << "Argumento/s desconocido/s\nCERRANDO..." << endl;
                return 1;
        }
    }

    if (usuarios.find(usuarioSeleccionado) == usuarios.end()){
        cout << "\nUsuario no disponible" << endl;
        return 1;
    }

    if(vec.size() == 0){
        cout << "\nIngreso erroneo del flag -v\nCERRANDO..." << endl;
        return 1;
    }
    
    cout << "\n     MENU PID=" << getpid() << endl;
    cout << "      [" << usuarios[usuarioSeleccionado] << "] " << usuarioSeleccionado << endl;
    cout << "-----------------------------" << endl;

    map<int, string> menu;
    vector<int> opcionesUsuario;
    menu = crearMenu(opcionesUsuario, usuarios, usuarioSeleccionado);
    int menusize = menu.size() - 1;

    string ruta;
    ruta = rutaNom(path);

    int opc;
    int preparar_datos = 0, crear_indice = 0;
    string entrada, treePath, listPath, graphPath;
    json dirArJSON;

    cout << "-----------------------------" << endl;

    do{
        cout << "Selecciona una opción: ";
        cin >> entrada;

        system("clear");
        printMenu(opcionesUsuario,usuarios,usuarioSeleccionado);

        try{
            opc = stoi(entrada);
            break;
        }catch (const exception& e){
            cout << "\n-----------------------------" << endl;
            cout << "Entrada inválida" << endl;
        }
        cout << endl;
    }while(true);

    cout << endl;

    while(true){
        cout << "-----------------------------" << endl;

        if(menu[opc] == "salir") break;
        else if(menu[opc] == "sumatoria" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()) sumatoria(vec);
        else if(menu[opc] == "promedio" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()) promedio(vec);
        else if(menu[opc] == "moda" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()) moda(vec);
        else if(menu[opc] == "contar" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()) contar(vec);
        else if(menu[opc] == "crearArchivo" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()) crearArchivo(ruta);
        else if(menu[opc] == "agregarTexto" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()) agregarTexto(texto, ruta);
        else if(menu[opc] == "generico" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()) generico(opc);
        else if(menu[opc] == "conteoPalabras" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()) conteoPalabras(input,output);
        else if(menu[opc] == "prepDatos" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()){

            string ext = getenv("EXTENTION");
            string in = getenv("PATH_FILES_IN");
            string out = getenv("PATH_FILES_OUT");
            string threads = getenv("AMOUNT_THREADS");

            string prepDatos = "./procesos/prepDatos/app " + ext + " " + in + " " + out + " " + threads;
            preparar_datos = system(prepDatos.c_str());

        }
        else if(menu[opc] == "crearIndice" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()){

            if(preparar_datos == 0) cout << "Para utilizar la opción para CREAR ÍNDICE INVERTIDO primero seleccione PREPARAR DATOS PARA ÍNDICE INVERTIDO." << endl;
            else{

                string out = getenv("PATH_FILES_OUT");
                string idx = getenv("INVERTED_INDEX_FILE");

                string crearIndice = "./procesos/crearIndice/app " + idx + " " + out;
                crear_indice = system(crearIndice.c_str());
            }
        }
        else if(menu[opc] == "llamarBuscador" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()){

            if(crear_indice == 0) cout << "Para utilizar la opción LLAMAR BUSCADOR primero ejecute CREAR ÍNDICE INVERTIDO." << endl;
            else{

                string idx = getenv("INVERTED_INDEX_FILE");
                string topk = getenv("TOPK");

                string llamarBuscador = "./procesos/buscador/app " + idx + " " + topk;
                system(llamarBuscador.c_str());

                system("clear");
                printMenu(opcionesUsuario,usuarios,usuarioSeleccionado);
            }
        }
        else if(menu[opc] == "directorioArbol" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()){
            
            cout << "Ingrese la ruta del archivo a procesar en base a un árbol jerárquico: ";
            cin >> treePath;
            cout << endl;

            if(validarExistenciaArchivo(treePath)){
                if(validarExtension("dit", treePath)){

                    ifstream archivoStream(treePath);

                    if (archivoStream.is_open()) {
                        string dirArSTR((istreambuf_iterator<char>(archivoStream)), istreambuf_iterator<char>());                                        

                        dirArJSON = json::parse(dirArSTR);

                        if(validarFormatoMensaje(dirArJSON)){

                            string procesoArbol = getenv("PROCESO_ARBOL");
                            string directorioArbol = procesoArbol + " '" + dirArSTR + "'";
                            system(directorioArbol.c_str());
                        }

                        archivoStream.close();
                    } else {
                        cerr << "No se pudo abrir el archivo." << endl;
                    }

                }
            }
        }
        else if(menu[opc] == "directorioLista" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()){

            cout << "Ingrese la ruta del archivo a procesar en base a una lista circular: ";
            cin >> listPath;
            cout << endl;

            if(validarExistenciaArchivo(listPath)){
                if(validarExtension("dre", listPath)){

                    string procesoLista = getenv("PROCESO_LISTA");
                    string directorioLista = procesoLista + " " + listPath;
                    system(directorioLista.c_str());

                }
            }


        }
        else if(menu[opc] == "grafico" && find(opcionesUsuario.begin(), opcionesUsuario.end(), opc) != opcionesUsuario.end()){

            cout << "Ingrese la ruta del archivo a graficar: ";
            cin >> graphPath;
            cout << endl;

            if(validarExistenciaArchivo(graphPath)){
                if(validarExtension("gra", graphPath)){
                    if(validarArchivoGrafico(graphPath)){

                        string procesoGrafico = getenv("PROCESO_GRAFICO");
                        string procesamientoGrafico = procesoGrafico + " " + graphPath;
                        system(procesamientoGrafico.c_str());
                    }

                }
            }
        }

        else if(opc<0 || opc>menusize) cout << "Opción fuera de rango" << endl;
        else cout << "No disponible para el usuario " << usuarioSeleccionado << endl;

        do{
            cout << "\nSelecciona una opción: ";
            cin >> entrada;

            system("clear");
            printMenu(opcionesUsuario,usuarios,usuarioSeleccionado);

            try{
                opc = stoi(entrada);
                break;
            }catch (const exception& e){
                cout << "\n-----------------------------" << endl;
                cout << "Entrada inválida" << endl;
            }

        }while(true);

        system("clear");
        printMenu(opcionesUsuario,usuarios,usuarioSeleccionado);
        cout << endl;
    }

    // string base = dirArJSON["dirBase"];
    // string comando = "rm -r " + base;
    // system(comando.c_str());

    cout << "\n--------------------" << endl;
    cout << "GRACIAS POR PREFERIR" << endl;
    cout << "--------------------" << endl;
    return 0;
}