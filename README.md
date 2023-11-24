# Trabajo 4 S.O. 

Esta aplicación es un menú personalizado que tiene como finalidad realizar diferentes operaciones, ya sea con un vector ingresado por el usuario, diferentes archivos ingresado por el usuario, variados procesos con bases de datos o la creación del índice invertido de varios archivos de texto.  
Cuenta con 3 carpetas; **db**, **files** y **progs**.  
- Puede guardar sus archivos de texto en la carpeta $\textit{files/newFiles}$ (recomendado).
- La carpeta $\textit{files/conteo}$ cuenta con un archivo **input.txt** y **output.txt** con los que puede trabajar.
- Las opciones **Preparar datos para índice invertido** y **Crear índice invertido** llaman a procesos externos (guardados en $\textit{progs/procesos}$) cuales realizan las funciones.

## Compilador
Para compilar
> $ make

## Ejecutable
Para ejecutar:  
--> **./prog -u (usuario) -v (vector separado por comas) -f (path,nombre del archivo) -t (texto) -i (path del archivo a leer) -o (path del archivo a escribir)**
> ejemplo: ./prog -u "sebe" -v "2,3,1,4,5" -f "../files/newFiles,newFile001.txt" -t "texto" -i "../files/conteo/input.txt" -o "../files/conteo/output.txt"

## Bases de Datos
Cada base de datos esta guardada en la carpeta **db**.
- usuarios.txt  
    - Guarda los usuarios que están registrados y disponibles para ocupar la aplicación.
        - cada usuario está registrado como **nombre_de_usuario;perfil**. 
        > ejemplo: sebe;admin
- menus.txt
    - Contiene el menú personalizado que será desplegado en la aplicación.
        - Cada opción está registrada como **entero_opción_de_menú,nombre_de_opción,función_asociada**.
        > ejemplo: 5,Crear archiv,crearArchivo
- perfiles.txt
    - Contiene todos los perfiles de usuario disponibles en la aplicación con sus funciones respectivas.  
        - Cada perfil está registrado como **nombre_del_perfil;funcion1,funcion2...**
        > ejemplo: rookie;sumatoria,contar
- caracteres.txt
    - Contiene todos los carácteres especiales que no se quieren tomar al contar las palabras del archivo $\textit{input.txt}$ por línea

## Configuración de ambiente $\textit{'environment'}$
El programa cuenta con un archivo de configuración **.env** el cual guarda en $\textit{variables de entorno}$ todos los path de las bases de datos usadas en el código de esta.  
Cada variable esta guardada en el .env de la forma **NOMBRE_VARIABLE=path_database**
>**USUARIOS**=../db/usuarios.txt - **PERFILES**=../db/perfiles.txt - **MENUS**=../db/menus.txt - **CARAC**=../db/caracteres.txt - **PATH_FILES_IN**=../files/in -  
>**PATH_FILES_OUT**=../files/out - **INVERTED_INDEX_FILE**=../files/file.idx - **AMOUNT_THREADS**=2 - **EXTENTION**=.txt